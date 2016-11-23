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





### load the dist file
fname = sys.argv[1][:-8]+'spec.bin'
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
	fig  = plt.figure()
	ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
	print ene
	ax1 = fig.add_subplot(111)
	make_steps(ax1,ene,[0],dist,options=['log'],linewidth=2)
	ax1.grid(1)
	ax1.set_xlabel(r'Energy (MeV)')
	ax1.set_ylabel(r'Current (n/p)')
	plt.show()
else:
    print "No file '"+fname+"' present."





### load the dist file
try:
	dist = numpy.fromfile(sys.argv[1],dtype=numpy.float64)
except IOError:
    print "No file '"+sys.argv[1]+"' present."
    exit()

### option
if len(sys.argv) == 2:
	logplot = False
elif len(sys.argv) == 3:
	if sys.argv[2] == 'log':
		logplot = True
	elif sys.argv[2] == 'lin':
		logplot = False
	else:
		logplot = False
else:
	print '2 or fewer arguments please'
	exit()

### first 11 values are the lengths, xy params
E_len		= int(dist[ 0])
theta_len	= int(dist[ 1])
phi_len		= int(dist[ 2])
y_len		= int(dist[ 3])
y_min		= int(dist[ 4])
y_max		= int(dist[ 5])
y_res		= int(dist[ 6])
x_len		= int(dist[ 7])
x_min		= int(dist[ 8])
x_max		= int(dist[ 9])
x_res		= int(dist[10])
dist_start	=          11

# copy vectors 
E_bins		= dist[dist_start:dist_start+E_len] 
dist_start	= dist_start + E_len
theta_bins	= dist[dist_start:dist_start+theta_len] 
dist_start	= dist_start + theta_len
phi_bins	= dist[dist_start:dist_start+phi_len] 
dist_start	= dist_start + phi_len

# print lengths, vectors
print "%10s %5d"%("E_len",		E_len		)
print "%10s %5d"%("theta_len",	theta_len	)
print "%10s %5d"%("phi_len",	phi_len		)
print "%10s %5d"%("y_len",		y_len		)
print "%10s %5d"%("y_min",		y_min		)
print "%10s %5d"%("y_max",		y_max		)
print "%10s %5d"%("y_res",		y_res		)
print "%10s %5d"%("x_len",		x_len		)
print "%10s %5d"%("x_min",		x_min		)
print "%10s %5d"%("x_max",		x_max		)
print "%10s %5d"%("x_res",		x_res		)
print "E_bins",E_bins
print "theta_bin",theta_bins
print "phi_bins",phi_bins

### remove the header info and reshape for easier indexing
dist = dist[dist_start:]
dist = numpy.resize(dist,(E_len-1,theta_len-1,phi_len-1,y_len-1,x_len-1))

### constants
charge_per_amp = 6.241e18
charge_per_milliamp = charge_per_amp/1000.0

### calculate xy bins based on parameters
x_bins   	= numpy.linspace(x_min,x_max,x_len+1)
y_bins   	= numpy.linspace(y_min,y_max,y_len+1)

### flags
fluxflag = False
sphere = False

### images

#upper_lim=[5e9,1e10,100]
phi_bin=0
for theta_bin in range(0,len(theta_bins)-1):
	for E_bin in range(0,len(E_bins)-1):
		f = plt.figure()
		ax = f.add_subplot(111)
		if logplot:
			imgplot = ax.imshow(dist[E_bin][theta_bin][phi_bin][:][:]*charge_per_milliamp          ,extent=[x_bins[0],x_bins[-1],y_bins[0],y_bins[-1]],origin='lower',cmap=plt.get_cmap('jet'),norm=LogNorm())#vmax=upper_lim[E_bin]))
		else:
			imgplot = ax.imshow(dist[E_bin][theta_bin][phi_bin][:][:]*charge_per_milliamp          ,extent=[x_bins[0],x_bins[-1],y_bins[0],y_bins[-1]],origin='lower',cmap=plt.get_cmap('jet'))#,vmax=upper_lim[E_bin])
		this_weight = numpy.sum(dist[E_bin][theta_bin][phi_bin][:][:]*charge_per_milliamp)
		imgplot.set_interpolation('nearest')
		theta_deg = theta_bins[theta_bin:theta_bin+2]*180.0/numpy.pi
		phi_deg = phi_bins[phi_bin:phi_bin+2]*180.0/numpy.pi
		E_meV   = E_bins[E_bin:E_bin+2]*1.0e9
		E_eV   = E_bins[E_bin:E_bin+2]*1.0e6
		ax.grid()
		cbar=plt.colorbar(imgplot)
		ax.set_xlim([x_bins[0],x_bins[-1]])
		ax.set_ylim([y_bins[0],y_bins[-1]])
		#
		#  plot where the spectrum (if present) was calculated
		#
		if spec:
			ax.plot(spec_area_x,spec_area_y,linewidth=2,color=[0.8,0.8,0.8])
		#
		#
		plt.show()