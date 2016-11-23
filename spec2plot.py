#! /home/l_bergmann/anaconda/bin/python -W ignore
# /usr/local/bin/python 
#
# ss2dist, the MCNP surface source to histogram distribution maker
# Ryan M. Bergmann, March 2015 
# ryan.bergmann@psi.ch, ryanmbergmann@gmail.com

import numpy, sys
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cmx
from matplotlib import gridspec
from matplotlib.colors import LogNorm


#
#
#
#  spectrum plot
#
#

def coarsen(values,bins,bin_red=2):
	import numpy
	v_out=[]
	b_out=[]
	for i in range(0,len(values)/bin_red):
		v = 0.0
		for j in range(0,bin_red):
			v = v + values[i*bin_red+j]
		v_out.append(v)
		b_out.append(bins[i*bin_red])
	b_out.append(bins[-1])
	return numpy.array(v_out),numpy.array(b_out)


def make_steps(ax,bins_in,avg_in,values_in,options=['log'],color=None,label='',ylim=False,linewidth=1):
	import numpy, re
	assert(len(bins_in)==len(values_in)+1)

	### make copies
	bins=bins_in[:]
	values=values_in[:]
	avg=avg_in[:]
	#err=err_in[:]

	### smooth data?  parse format
	for opt in options:
		res = re.match('smooth',opt)
		if res:
			smooth_opts = opt.split('=')
			if len(smooth_opts)==1:
				wlen = 7
			elif len(smooth_opts)==2:
				wlen = int(smooth_opts[1])
			else:
				wlen = int(smooth_opts[1])
				print "MULTIPLE = SIGNS IN SMOOTH.  WHY?  ACCEPTING FIRST VALUE."
			if wlen%2==0:
				print "WINDOW LENGTH EVEN, ADDING 1..."
				wlen = wlen + 1
			print "smoothing %d bins..."%wlen
			label = label + ' SMOOTHED %d BINS'%wlen
			values = self._smooth(numpy.array(values),window_len=wlen)
			values = values[(wlen-1)/2:-(wlen-1)/2]   # trim to original length

	### coarsen data?  parse format
	for opt in options:
		res = re.match('coarsen',opt)
		if res:
			coarsen_opts = opt.split('=')
			if len(coarsen_opts)==1:
				bin_red = 2
			elif len(coarsen_opts)==2:
				bin_red = int(coarsen_opts[1])
			else:
				bin_red = int(coarsen_opts[1])
				print "MULTIPLE = SIGNS IN SMOOTH.  WHY?  ACCEPTING FIRST VALUE."
			if len(values)%bin_red==0:
				print "Reducing bins by factor of %d ..."%bin_red
				label = label + ' COMBINED %d BINS'%bin_red
				values,bins = self._coarsen(numpy.array(values),numpy.array(bins),bin_red=bin_red)
			else:
				print "DATA LENGHTH NOT EVENLY DIVISIBLE BY COARSEN FACTOR, IGNORING..."

	### make rectangles
	x=[]
	y=[]
	x.append(bins[0])
	y.append(0.0)
	for n in range(len(values)):
		x.append(bins[n])
		x.append(bins[n+1])
		y.append(values[n])
		y.append(values[n])
	x.append(bins[len(values)])
	y.append(0.0)

	### plot with correct scale
	if 'lin' in options:
		if 'logy' in options:
			ax.semilogy(x,y,color=color,label=label,linewidth=linewidth)
		else:
			ax.plot(x,y,color=color,label=label,linewidth=linewidth)
	else:   #default to log if lin not present
		if 'logy' in options:
			ax.loglog(x,y,color=color,label=label,linewidth=linewidth)
		else:
			ax.semilogx(x,y,color=color,label=label,linewidth=linewidth)


fig  = plt.figure()
ax1 = fig.add_subplot(111)
cm  = plt.get_cmap('jet') 
cNorm  = colors.Normalize(vmin=0, vmax=len(sys.argv[1:]))
scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)

### load the dist file
for i in range(0,len(sys.argv[1:])):
	fname = sys.argv[i+1]
	spec= True
	try:
		dist = numpy.fromfile(fname,dtype=numpy.float64)
	except IOError:
	    spec = False
	
	if spec:
		### load params
		E_min		= dist[ 0]
		E_max		= dist[ 1]
		E_bins		= dist[ 2]
		x_min		= dist[ 3]
		x_max		= dist[ 4]
		y_min		= dist[ 5]
		y_max		= dist[ 6]
		dist_start	=       7
		spec_area_x=[x_min,x_max,x_max,x_min,x_min]
		spec_area_y=[y_min,y_min,y_max,y_max,y_min]
		
		# print lengths, vectors
		print "%10s %6.4E"%("E_min",	E_min	)
		print "%10s %6.4E"%("E_max",	E_max	)
		print "%10s %5d"%("E_bins",	E_bins	)
		print "%10s %6.4E"%("x_min",	x_min	)
		print "%10s %6.4E"%("x_max",	x_max	)
		print "%10s %6.4E"%("y_min",	y_min	)
		print "%10s %6.4E"%("y_max",	y_max	)
		
		### remove the header info and reshape for easier indexing
		dist = dist[dist_start:]
		
		### constants
		charge_per_amp = 6.241e18
		charge_per_milliamp = charge_per_amp/1000.0
		
		### images
		ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
		colorVal = scalarMap.to_rgba(i)
		make_steps(ax1,ene,[0],dist,options=['log'],linewidth=2, color=colorVal,label=fname)
		ax1.grid(1)
		ax1.set_xlabel(r'Energy (MeV)')
		ax1.set_ylabel(r'Current (n/p)')
	else:
	    print "No file '"+fname+"' present."


plt.legend()
plt.show()
