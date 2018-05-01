#! /home/l_bergmann/anaconda/bin/python -W ignore
# /usr/local/bin/python 
#
# ss2dist, the MCNP surface source to histogram distribution maker
# Ryan M. Bergmann, March 2015 
# ryan.bergmann@psi.ch, ryanmbergmann@gmail.com

import numpy, sys, time
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cmx
from matplotlib import gridspec
from matplotlib.colors import LogNorm

class mcnp_cell_helper(object):

	# global
	#
	# starting numbers for
	# surfaces
	plane_surface_number		= 80000
	cyl_surface_number			= 81000
	hex_surface_number			= 82000
	# cells
	cell_number 				= 80000
	# boundary surface, should be set
	boundary_surface 			= 20
	# transform for surfaces 
	surface_transform			= 1
	surface_transform_string	= ''
	# density maps
	den = {}
	# material maps
	mat_map = {}


	def __new__(cls,*args):
		return super(mcnp_cell_helper,cls).__new__(cls)

	def __init__(self,*args):
		import copy
		if isinstance(args[0],str):
			self.name		= copy.deepcopy(args[0].strip())
		else:
			print "name must be a string.  rejected."
			return None
		#
		if mcnp_cell_helper.surface_transform:
			mcnp_cell_helper.surface_transform_string = '%2d'%mcnp_cell_helper.surface_transform

	def _make_plane(self, a, b, c, d):
		#surf     97 plane        0.000     0.000    1.000   -10 
		if a==1. and b==0. and c==0.:
			out_string =  '%5d %2s px % 14.12E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,d)
		elif a==0. and b==1. and c==0.:
			out_string =  '%5d %2s py % 14.12E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,d)
		elif a==0. and b==0. and c==1.:
			out_string =  '%5d %2s pz % 14.12E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,d)
		else:
			out_string =  '%5d %2s p % 14.12E % 14.12E % 14.12E % 14.12E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,a,b,c,d)
		mcnp_cell_helper.plane_surface_number = mcnp_cell_helper.plane_surface_number + 1
		return mcnp_cell_helper.plane_surface_number-1, out_string+'\n'

	def _make_cell(self, material, density, surface_lists_list, exclude=None, universe=0):  # list will be union'ed
		string_list=[]
		if material!=0:
			out_string = '%-5d %5d % 8.6E '%(mcnp_cell_helper.cell_number,material,-density)
		else:
			out_string = '%-5d %5d        '%(mcnp_cell_helper.cell_number,material)
		leader = '      '
		# make union of each list in lists_list
		for surface_list in surface_lists_list:
			if len(surface_list)>0:
				out_string = out_string + '('
				for s in surface_list:
					out_string = out_string + ' % d'%s
					if len(out_string)>=70:
						string_list.append(out_string)
						out_string = leader
				out_string = out_string + '):'
		out_string = out_string[:-1] # remove trailing colon
		#
		if len(string_list)>0:
			string_list.append(out_string)
			out_string = '\n'.join(string_list)
		#
		if exclude:
			out_string = out_string + ' #%d '%exclude
		if universe:
			out_string = out_string + ' u=%d '%universe
		#
		mcnp_cell_helper.cell_number = mcnp_cell_helper.cell_number + 1
		return mcnp_cell_helper.cell_number-1, out_string+'\n'

	def _make_cylx(self, y0, z0, r):
		assert(r>0.)
		if y0==0. and z0==0.:
			out_string = '%5d %2s cx   % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		else:
			out_string = '%5d %2s c/x  % 14.12E % 14.12E % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,y0,z0,r)
		# 
		mcnp_cell_helper.cyl_surface_number = mcnp_cell_helper.cyl_surface_number + 1
		return mcnp_cell_helper.cyl_surface_number-1, out_string+'\n'

	def _make_cyly(self, x0, z0, r):
		assert(r>0.)
		if x0==0. and z0==0.:
			out_string = '%5d %2s cy   % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		else:
			out_string = '%5d %2s c/y  % 14.12E % 14.12E % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,z0,r)
		# 
		mcnp_cell_helper.cyl_surface_number = mcnp_cell_helper.cyl_surface_number + 1
		return mcnp_cell_helper.cyl_surface_number-1, out_string+'\n'

	def _make_cylz(self, x0, y0, r):
		assert(r>0.)
		if x0==0. and y0==0.:
			out_string = '%5d %2s cz   % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		else:
			out_string = '%5d %2s c/z  % 14.12E % 14.12E % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,y0,r)
		# 
		mcnp_cell_helper.cyl_surface_number = mcnp_cell_helper.cyl_surface_number + 1
		return mcnp_cell_helper.cyl_surface_number-1, out_string+'\n'

	def _make_sphere(self, x0, y0, z0, r):
		assert(r>0.)
		if x0==0. and y0==0. and z0==0.:
			out_string = '%5d %2s so   % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		elif y0==0. and z0==0.:
			out_string = '%5d %2s sx   % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,r)
		elif x0==0. and z0==0.:
			out_string = '%5d %2s sy   % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,y0,r)
		elif x0==0. and y0==0.:
			out_string = '%5d %2s sz   % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,z0,r)
		else:
			out_string = '%5d %2s s    % 14.12E % 14.12E % 14.12E % 14.12E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,y0,z0,r)
		# 
		mcnp_cell_helper.cyl_surface_number = mcnp_cell_helper.cyl_surface_number + 1
		return mcnp_cell_helper.cyl_surface_number-1, out_string+'\n'

def make_independent_distribution(file_obj,dist_number,*args):
	if len(args)>=1:
		vector_vars = args[0]
	if len(args)==2:
		vector_probs = args[1]
		assert(len(vector_vars)==len(vector_probs)+1)
	if len(args)==0 or len(args)>2:
		print "TOO MANY ARGUMENTS TO make_independent_distribution"
		return
	string0 = 'SI%d      '%dist_number
	file_obj.write(string0)
	total_len = len(string0)
	for k in range(0,len(vector_vars)):
		#if vector_probs[k]>0.0:
		string1=' % 14.12E'%vector_vars[k]
		total_len = total_len + len(string1)
		if total_len > 80:
			file_obj.write('\n'+' '*max(5,len(string0)))
			total_len = len(string1)+max(5,len(string0))
		file_obj.write(string1)
	file_obj.write('\n')
	if len(args)==2:
		string0 = 'SP%d      '%dist_number
		file_obj.write(string0)
		total_len = len(string0)
		string1=' % 14.12E'%0.0
		total_len = total_len + len(string1)
		file_obj.write(string1)
		for k in range(0,len(vector_probs)):
			#if vector_probs[k]>0.0:
			string1=' % 14.12E'%vector_probs[k]
			total_len = total_len + len(string1)
			if total_len > 80:
				file_obj.write('\n'+' '*max(5,len(string0)))
				total_len = len(string1)+max(5,len(string0))
			file_obj.write(string1)
		file_obj.write('\n')


def make_dependent_distribution(file_obj,dist_number,secondary_dist_start,vector_vars,vector_probs,option='H',datatype='float'):
	
	#write distribution of distributions card
	string0 = 'DS%d   S '%dist_number
	file_obj.write(string0)
	total_len = len(string0)
	for k in range(0,len(vector_probs)):
		#if probs[k]>0.0:
		string1=' D%d'%(k+secondary_dist_start)
		total_len = total_len + len(string1)
		if total_len > 80:
			file_obj.write('\n'+' '*max(5,len(string0)))
			total_len = len(string1)+max(5,len(string0))
		file_obj.write(string1)
	file_obj.write('\n')
	file_obj.write('c\nc\nc\n')

	# write secondary distributions themselves
	if datatype == '3float':
		data_string=' % 14.12E % 14.12E % 14.12E'
	elif datatype == 'float':
		data_string=' % 14.12E'
	elif datatype == 'int':
		data_string = ' %d'
	for k in range(0,len(vector_probs)):
		#if probs[k]>0.0:
		# SI card first
		string0 = 'SI%d %s '%(k+secondary_dist_start,option)
		file_obj.write(string0)
		total_len = len(string0)  
		for j in range(0,len(vector_vars[k])):
			if datatype == '3float':
				string1=data_string%(vector_vars[k][j][0],vector_vars[k][j][1],vector_vars[k][j][2])
			else:
				string1=data_string%vector_vars[k][j]
			total_len = total_len + len(string1)
			if total_len > 80:
				file_obj.write('\n'+' '*max(5,len(string0)))
				total_len = len(string1)+max(5,len(string0))
			file_obj.write(string1)
		file_obj.write('\n')
		# SP card second
		string0 = 'SP%d    '%(k+secondary_dist_start)
		file_obj.write(string0)
		total_len = len(string0)
		if option=='H':
			string1=' % 14.12E'%0.0
		else:
			string1=''
		total_len = total_len + len(string1)
		file_obj.write(string1)
		for j in range(0,len(vector_probs[k])):
			string1=' % 14.12E'%vector_probs[k][j]
			total_len = total_len + len(string1)
			if total_len > 80:
				file_obj.write('\n'+' '*max(5,len(string0)))
				total_len = len(string1)+max(5,len(string0))
			file_obj.write(string1)
		file_obj.write('\n')
		file_obj.write('c \n')



def make_dependent_variable(file_obj,dist_number,vector_vars,option='H',datatype='3float'):

	# write distributions 
	if datatype == '3float':
		data_string=' % 14.12E % 14.12E % 14.12E'
	if datatype == 'float':
		data_string=' % 14.12E'
	elif datatype == 'int':
		data_string = ' %d'
	# SI card first
	string0 = 'DS%d %s '%(dist_number,option)
	file_obj.write(string0)
	total_len = len(string0)  
	for j in range(0,len(vector_vars)):
		if datatype == '3float':
			string1=data_string%(vector_vars[j][0],vector_vars[j][1],vector_vars[j][2])
		else:
			string1=data_string%vector_vars[j]
		total_len = total_len + len(string1)
		if total_len > 80:
			file_obj.write('\n'+' '*max(5,len(string0)))
			total_len = len(string1)+max(5,len(string0))
		file_obj.write(string1)
	file_obj.write('\n')



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

def _smooth(x,window_len=11,window='flat'):
	# take from stackexchange
	import numpy

	if x.ndim != 1:
		raise ValueError, "smooth only accepts 1 dimension arrays."

	if x.size < window_len:
		raise ValueError, "Input vector needs to be bigger than window size."


	if window_len<3:
		return x

	if not window in ['flat', 'hanning', 'hamming', 'bartlett', 'blackman']:
		raise ValueError, "Window is on of 'flat', 'hanning', 'hamming', 'bartlett', 'blackman'"


	s=numpy.r_[x[window_len-1:0:-1],x,x[-1:-window_len:-1]]
	#print(len(s))
	if window == 'flat': #moving average
		w=numpy.ones(window_len,'d')
	else:
		w=eval('numpy.'+window+'(window_len)')

	y=numpy.convolve(w/w.sum(),s,mode='valid')

	# trim to range where data is nonzero
	nonzeros = numpy.where(numpy.array(x)!=0.0)[0]
	firstdex = nonzeros[ 0]
	lastdex  = nonzeros[-1]
	y=y[(window_len-1)/2:-(window_len-1)/2]
	y[:firstdex] = 0.0
	y[lastdex+1:] = 0.0
	
	return y

def make_steps(ax,bins_in,avg_in,values_in,options=['log'],color=None,label='',ylim=False,linewidth=1):
	import numpy, re
	assert(len(bins_in)==len(values_in)+1)

	### make copies
	bins=bins_in[:]
	values=values_in[:]
	avg=avg_in[:]
	#err=err_in[:]

	### color
	if color==None:
		color='b'

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
			values = _smooth(numpy.array(values),window_len=wlen)
			#values = values[(wlen-1)/2:-(wlen-1)/2]   # trim to original length

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

particle_symbols = {}
particle_symbols[ 0] = ' '
particle_symbols[ 1] = 'n'
particle_symbols[ 5] = 'q'
particle_symbols[ 2] = 'p'
particle_symbols[ 3] = 'e'
particle_symbols[ 8] = 'f'
particle_symbols[ 4] = '|'
particle_symbols[16] = '!'
particle_symbols[ 6] = 'u'
particle_symbols[17] = '<'
particle_symbols[ 7] = 'v'
particle_symbols[18] = '>'
particle_symbols[ 9] = 'h'
particle_symbols[19] = 'g'
particle_symbols[10] = 'l'
particle_symbols[25] = 'b'
particle_symbols[11] = '+'
particle_symbols[26] = '_'
particle_symbols[12] = '-'
particle_symbols[27] = '~'
particle_symbols[13] = 'x'
particle_symbols[28] = 'c'
particle_symbols[14] = 'y'
particle_symbols[29] = 'w'
particle_symbols[15] = 'o'
particle_symbols[30] = '@'
particle_symbols[20] = 'pi0'
particle_symbols[35] = '*'
particle_symbols[21] = 'z'
particle_symbols[22] = 'k'
particle_symbols[36] = '?'
particle_symbols[23] = '%'
particle_symbols[24] = '^'
particle_symbols[31] = 'd'
particle_symbols[32] = 't'
particle_symbols[33] = 's'
particle_symbols[34] = 'a'
particle_symbols[37] = '#'

### option
plot=False
png=False
smooth=0
if len(sys.argv) == 2:
	logplot = False
elif len(sys.argv) == 3:
	if sys.argv[2] == 'log':
		logplot = True
	elif sys.argv[2] == 'lin':
		logplot = False
	else:
		logplot = False
	vmax_in = 1e6
	vmin_in = 1e0
elif len(sys.argv) == 4:
	if sys.argv[2] == 'log':
		logplot = True
	elif sys.argv[2] == 'lin':
		logplot = False
	else:
		logplot = False
	#
	vmax_in = float(sys.argv[3])
	vmin_in = 1e0
elif len(sys.argv) == 5:
	if sys.argv[2] == 'log':
		logplot = True
	elif sys.argv[2] == 'lin':
		logplot = False
	else:
		logplot = False
	#
	vmax_in = float(sys.argv[4])
	vmin_in = float(sys.argv[3])
elif len(sys.argv) >= 6:
	if sys.argv[2] == 'log':
		logplot = True
	elif sys.argv[2] == 'lin':
		logplot = False
	else:
		logplot = False
	#
	vmax_in = float(sys.argv[4])
	vmin_in = float(sys.argv[3])
	#
	for i in range(5,len(sys.argv)):
		if sys.argv[i] == 'plot':
			plot=True
		if sys.argv[i] == 'png':
			png=True
		sline=sys.argv[i].split('=')
		if sline[0]=='smooth':
			smooth=int(sline[1])

### load the spec file
fname = sys.argv[1][:-8]+'spec.bin'
spec_present= True
try:
	spec_data = numpy.fromfile(fname,dtype=numpy.float64)
except IOError:
    spec_present = False

if spec_present:

	cm  = plt.get_cmap('jet') 

	E_min			=     spec_data[ 0]
	E_max			=     spec_data[ 1]
	E_bins			= int(spec_data[ 2])
	theta_bins  	= int(spec_data[ 3])
	x_min			=     spec_data[ 4]
	x_max			=     spec_data[ 5]
	y_min			=     spec_data[ 6]
	y_max			=     spec_data[ 7]
	this_sc			= int(spec_data[ 8])
	this_particle	= int(spec_data[ 9])
	angle_spec_bins	= int(spec_data[10])
	theta_start		=      11
	spec_start		=      11+theta_bins+1
	spec_end 		=spec_start+theta_bins*E_bins
	angle_spec_start=spec_end
	spec_area_x=[x_min,x_max,x_max,x_min,x_min]
	spec_area_y=[y_min,y_min,y_max,y_max,y_min]
	
	# print lengths, vectors
	print "%10s %6.4E"%("E_min",	E_min	)
	print "%10s %6.4E"%("E_max",	E_max	)
	print "%10s %5d"%(  "E_bins",	E_bins	)
	print "%10s %6.4E"%("x_min",	x_min	)
	print "%10s %6.4E"%("x_max",	x_max	)	
	print "%10s %6.4E"%("y_min",	y_min	)
	print "%10s %6.4E"%("y_max",	y_max	)
	
	### remove the header info and reshape for easier indexing
	theta_edges = spec_data[theta_start:spec_start] 
	spec = spec_data[spec_start:spec_end]
	spec = numpy.reshape(spec,(theta_bins,E_bins))

	### also for angle histogram
	angle_spec_edges = spec_data[angle_spec_start:angle_spec_start+angle_spec_bins+1] 
	angle_spec_value = spec_data[angle_spec_start+angle_spec_bins+1:]
	
	### constants
	charge_per_amp = 6.241e18
	charge_per_milliamp = charge_per_amp/1000.0

	### angle histogram
	fig  = plt.figure()
	ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
	ax1 = fig.add_subplot(111)
	cNorm  = colors.Normalize(vmin=0, vmax=theta_bins)
	scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)
	sa = -2.*numpy.pi*numpy.diff(numpy.cos(angle_spec_edges/180.*numpy.pi))
	angle_spec_value_normed = numpy.divide(angle_spec_value,sa)
	if smooth:
		print "SMOOTHING ANGLE SPECTRUM DATA BY %d BINS..."%smooth
		this_spec = _smooth(angle_spec_value_normed,window_len=smooth)
		angle_spec_value_normed = this_spec
	maxdex = numpy.argmax(angle_spec_value_normed)
	maxval = angle_spec_value_normed[maxdex]
	maxx   = (angle_spec_edges[maxdex]+angle_spec_edges[maxdex+1])/2.
	make_steps(ax1,angle_spec_edges,[0],angle_spec_value_normed,options=['lin'],linewidth=2, color='b')
	ax1.annotate('x %6.4E y %6.4E'%(maxx,maxval), xy=(maxx,maxval), xytext=(maxx*1.1,maxval*1.1), arrowprops=dict(facecolor='black', shrink=0.5))
	ax1.grid(1)
	ax1.set_xlabel(r'Angle from Principle Vector (deg)')
	ax1.set_ylabel(r'Number Density (particles/source/sa)')
	#ax1.legend(loc=2)#'best')
	if png:
		fig.savefig('angular-spec.png')
	if plot:
		plt.show()
	
	### smoothing
	if smooth:
		print "SMOOTHING SPECTRAL DATA BY %d BINS..."%smooth
		for j in range(0,theta_bins):
			this_spec = _smooth(spec[j,:],window_len=smooth)
			spec[j,:] = this_spec

	### images
	fig  = plt.figure()
	ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
	ax1 = fig.add_subplot(111)
	cNorm  = colors.Normalize(vmin=0, vmax=theta_bins)
	scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)
	for j in range(0,theta_bins):
		colorVal = scalarMap.to_rgba(j)
		#print len(ene),len(spec[j,:])
		make_steps(ax1,ene,[0],spec[j,:],options=['log'],linewidth=2, color=colorVal,label='%d : %s : %5.2f-%5.2f deg'%(this_sc,particle_symbols[this_particle],theta_edges[j],theta_edges[j+1]))
	ax1.grid(1)
	ax1.set_xlabel(r'Energy (MeV)')
	ax1.set_ylabel(r'Current (particles/source)')
	ax1.legend(loc=2)#'best')
	if png:
		fig.savefig('%d-%s-specs.png'%(this_sc,particle_symbols[this_particle]))
	if plot:
		plt.show()

	### images, normalized to theta bin
	fig  = plt.figure()
	ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
	ax1 = fig.add_subplot(111)
	cNorm  = colors.Normalize(vmin=0, vmax=theta_bins)
	scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)
	for j in range(0,theta_bins):
		colorVal = scalarMap.to_rgba(j)
		#print len(ene),len(spec[j,:])
		sa = 2.0*numpy.pi*(numpy.cos(theta_edges[j]/180*numpy.pi)-numpy.cos(theta_edges[j+1]/180*numpy.pi))
		make_steps(ax1,ene,[0],spec[j,:]/sa,options=['log'],linewidth=2, color=colorVal,label='%d : %s : %5.2f-%5.2f deg'%(this_sc,particle_symbols[this_particle],theta_edges[j],theta_edges[j+1]))
	ax1.grid(1)
	ax1.set_xlabel(r'Energy (MeV)')
	ax1.set_ylabel(r'Current (particles/source/sterad)')
	ax1.legend(loc=2)#'best')
	if png:
		fig.savefig('%d-%s-specs-sa_normed.png'%(this_sc,particle_symbols[this_particle]))
	if plot:
		plt.show()
		
	fig  = plt.figure()
	ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
	ax1 = fig.add_subplot(111)
	cNorm  = colors.Normalize(vmin=0, vmax=theta_bins)
	scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)
	for j in range(0,theta_bins):
		colorVal = scalarMap.to_rgba(j)
		#print len(ene),len(spec[j,:])
		make_steps(ax1,ene,[0],spec[j,:],options=['log','logy'],linewidth=2, color=colorVal,label='%d : %s : %5.2f-%5.2f deg'%(this_sc,particle_symbols[this_particle],theta_edges[j],theta_edges[j+1]))
	ax1.grid(1)
	ax1.set_xlabel(r'Energy (MeV)')
	ax1.set_ylabel(r'Current (particles/source)')
	ax1.legend(loc=2)#'best')
	if png:
		fig.savefig('%d-%s-specs_loglog.png'%(this_sc,particle_symbols[this_particle]))
	if plot:
		plt.show()

	### images, normalized to theta bin
	fig  = plt.figure()
	ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
	ax1 = fig.add_subplot(111)
	cNorm  = colors.Normalize(vmin=0, vmax=theta_bins)
	scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)
	for j in range(0,theta_bins):
		colorVal = scalarMap.to_rgba(j)
		#print len(ene),len(spec[j,:])
		sa = 2.0*numpy.pi*(numpy.cos(theta_edges[j]/180*numpy.pi)-numpy.cos(theta_edges[j+1]/180*numpy.pi))
		make_steps(ax1,ene,[0],spec[j,:]/sa,options=['log','logy'],linewidth=2, color=colorVal,label='%d : %s : %5.2f-%5.2f deg'%(this_sc,particle_symbols[this_particle],theta_edges[j],theta_edges[j+1]))
	ax1.grid(1)
	ax1.set_xlabel(r'Energy (MeV)')
	ax1.set_ylabel(r'Current (particles/source/sterad)')
	ax1.legend(loc=2)#'best')
	if png:
		fig.savefig('%d-%s-specs-sa_normed_loglog.png'%(this_sc,particle_symbols[this_particle]))
	if plot:
		plt.show()
		
else:
    print "No file '"+fname+"' present."





### load the dist file
try:
	dist = numpy.fromfile(sys.argv[1],dtype=numpy.float64)
except IOError:
    print "No file '"+sys.argv[1]+"' present."
    exit()

### first 18 values are the lengths, xy params, surface params
E_len				=   int(dist[ 0])
theta_len			=   int(dist[ 1])
phi_len				=   int(dist[ 2])
y_len				=   int(dist[ 3])
y_min				=   int(dist[ 4])
y_max				=   int(dist[ 5])
y_res				=   int(dist[ 6])
x_len				=   int(dist[ 7])
x_min				=   int(dist[ 8])
x_max				=   int(dist[ 9])
x_res				=   int(dist[10])
surf_a				= float(dist[11])
surf_b				= float(dist[12])
surf_c				= float(dist[13])
surf_d				= float(dist[14])
this_sc				=   int(dist[15])
surf_cx				= float(dist[16])
surf_cy				= float(dist[17])
surf_cz				= float(dist[18])
this_particle		=   int(dist[19])
surface_vector1_1	= float(dist[20])
surface_vector1_2	= float(dist[21])
surface_vector1_3	= float(dist[22])
surface_vector2_1	= float(dist[23])
surface_vector2_2	= float(dist[24])
surface_vector2_3	= float(dist[25])
surface_vector3_1	= float(dist[26])
surface_vector3_2	= float(dist[27])
surface_vector3_3	= float(dist[28])
principle_vector_1	= float(dist[29])
principle_vector_2	= float(dist[30])
principle_vector_3	= float(dist[31])
dist_start			=            32

# copy vectors 
E_bins			= dist[dist_start:dist_start+E_len] 
dist_start		= dist_start + E_len
theta_bins_deg	= dist[dist_start:dist_start+theta_len] 
theta_bins		= numpy.pi*numpy.array(theta_bins_deg)/180.
cosine_bins		= numpy.cos(theta_bins)
dist_start		= dist_start + theta_len
phi_bins		= dist[dist_start:dist_start+phi_len] 
dist_start		= dist_start + phi_len

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
print "Surface: A=% 5.4E B=% 5.4E C=% 5.4E D=% 5.4E"%(surf_a,surf_b,surf_c,surf_d)
print "E_bins:",E_bins
print "theta_bins:",theta_bins
print "phi_bins:",phi_bins

### remove the header info and reshape for easier indexing
dist = dist[dist_start:]
dist = numpy.resize(dist,(E_len-1,theta_len-1,phi_len-1,y_len-1,x_len-1))

### constants
charge_per_amp = 6.241e18
charge_per_milliamp = charge_per_amp/1000.0

### calculate xy bins based on parameters
x_bins   	= numpy.linspace(x_min,x_max,x_len)
y_bins   	= numpy.linspace(y_min,y_max,y_len)

### flags
fluxflag = False
sphere = False

### images
phi_bin=0
for theta_bin in range(0,len(theta_bins)-1):
	for E_bin in range(0,len(E_bins)-1):
		f = plt.figure()
		ax = f.add_subplot(111)
		if logplot:
			imgplot = ax.imshow(dist[E_bin][theta_bin][phi_bin][:][:]          ,extent=[x_bins[0],x_bins[-1],y_bins[0],y_bins[-1]],origin='lower',cmap=plt.get_cmap('spectral'),norm=LogNorm(vmin=vmin_in,vmax=vmax_in))
		else:
			imgplot = ax.imshow(dist[E_bin][theta_bin][phi_bin][:][:]          ,extent=[x_bins[0],x_bins[-1],y_bins[0],y_bins[-1]],origin='lower',cmap=plt.get_cmap('spectral'),vmin=vmin_in,vmax=vmax_in)
		this_weight = numpy.sum(dist[E_bin][theta_bin][phi_bin][:][:])
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
		if spec_present:
			ax.plot(spec_area_x,spec_area_y,linewidth=2,color=[0.8,0.8,0.8])
		#
		#
		ax.set_title('%d : %s : %5.2f-%5.2f MeV : %5.2f-%5.2f deg : wgt %10.8E'%(this_sc,particle_symbols[this_particle],E_bins[E_bin],E_bins[E_bin+1],theta_bins_deg[theta_bin],theta_bins_deg[theta_bin+1],this_weight))
		if png:
			fig.savefig('%d-%s-dist-E%d-Theta%d.png'%(this_sc,particle_symbols[this_particle],E_bin,theta_bin))
		if plot:
			plt.show()


# sum over any energy bins for spatial dists
dist_reduced  = numpy.zeros(( len(theta_bins)-1 , len(phi_bins)-1 , len(y_bins)-1 , len(x_bins)-1 ),dtype=numpy.float64)
phi_bin=0
for theta_bin in range(0,len(theta_bins)-1):
	for E_bin in range(0,len(E_bins)-1):
		dist_reduced[theta_bin][phi_bin][:][:] = dist_reduced[theta_bin][phi_bin][:][:] + dist[E_bin][theta_bin][phi_bin][:][:]

#
#
# write mcnp sdef
#
surface_center 	= numpy.array([surf_cx,surf_cy,surf_cz]) 
surface_normal 	= numpy.array([surf_a,surf_b,surf_c]) 
surface_vec1 	= numpy.array([surface_vector1_1,surface_vector1_2,surface_vector1_3]) 
surface_vec2 	= numpy.array([surface_vector2_1,surface_vector2_2,surface_vector2_3]) 
surface_vec3 	= numpy.array([surface_vector3_1,surface_vector3_2,surface_vector3_3]) 
principle_vector= numpy.array([principle_vector_1,principle_vector_2,principle_vector_3])

# rotate central vector for source into reference frame
pvec = numpy.array([0.,0.,0.])
pvec[0]	= surface_vec2[0]*principle_vector[0]+surface_vec2[1]*principle_vector[1]+surface_vec2[2]*principle_vector[2]
pvec[1]	= surface_vec3[0]*principle_vector[0]+surface_vec3[1]*principle_vector[1]+surface_vec3[2]*principle_vector[2]
pvec[2]	= surface_vec1[0]*principle_vector[0]+surface_vec1[1]*principle_vector[1]+surface_vec1[2]*principle_vector[2]

offset_factor=1e-6
xform_num = 999

# figure out angular probabilities
weight_totals=[]
for k in range(0,len(cosine_bins)-1):
    weight_totals.append(numpy.sum(dist_reduced[k]))
print "total weight in dist",numpy.sum(weight_totals)
probs = numpy.array(weight_totals)/numpy.sum(weight_totals)
# files
sdef_name='%s.sdef'%sys.argv[1][:-9]
print "\nWriting MCNP SDEF to '"+sdef_name+"'..."
if sphere:
	pass
else:
	print "\nSDEF plane offset by % 3.2E...\n"%offset_factor
fsdef=open(sdef_name,'w')




# write easy stuff
fsdef.write('c\n')
fsdef.write('c SDEF from %s, '%sys.argv[1]+time.strftime("%d.%m.%Y, %H:%M")+'\n')
fsdef.write('c\n')
fsdef.write('sdef    par=%s\n'%particle_symbols[this_particle])
fsdef.write('c        sur=%5d\n'%this_sc)
fsdef.write('        axs=0 0 1\n')
fsdef.write('        vec=% 14.12E % 14.12E % 14.12E\n'%(pvec[0],pvec[1],pvec[2]))
fsdef.write('        ext=0\n')
fsdef.write('        tr=%3d\n'%xform_num)
fsdef.write('        rad=d1\n')
fsdef.write('        dir=d2\n')
fsdef.write('        erg=fdir=d3\n')
fsdef.write('        pos=fdir=d4\n')
fsdef.write('        wgt=%14.12E\n'%numpy.sum(weight_totals))
fsdef.write('c \n')
fsdef.write('c TRANSFORM\n')
fsdef.write('c \n')
fsdef.write('tr%3d   % 6.7E  % 6.7E  % 6.7E\n'%(xform_num,(1.0+offset_factor)*surface_center[0],(1.0+offset_factor)*surface_center[1],(1.0+offset_factor)*surface_center[2]))
fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec2[0],surface_vec2[1],surface_vec2[2])) # (surface_rotation_xy,90-surface_rotation_xy,90))
fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec3[0],surface_vec3[1],surface_vec3[2])) # (90+surface_rotation_xy,surface_rotation_xy,90))
fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec1[0],surface_vec1[1],surface_vec1[2])) # (90,90,surface_rotation_yz))
#
# 
# make CCC surfaces/cells
#
# surfs
mcnp_cell_helper.surface_transform = xform_num
helper = mcnp_cell_helper('helper')
x_nums = []
y_nums = []
z_nums = []
ccc_nums = []
pos_coord = []
for i in range(0,len(x_bins)):
	this_number, this_card = helper._make_plane(1,0,0,x_bins[i])
#	fsurf.write(this_card)
	x_nums.append(this_number)
for i in range(0,len(y_bins)):
	this_number, this_card = helper._make_plane(0,1,0,y_bins[i])
#	fsurf.write(this_card)
	y_nums.append(this_number)
this_number, this_card = helper._make_plane(0,0,1,-0.5)
#fsurf.write(this_card)
z_nums.append(this_number)
this_number, this_card = helper._make_plane(0,0,1, 0.5)
#fsurf.write(this_card)
z_nums.append(this_number)
#fsurf.close()
# cells
for j in range(0,len(y_bins)-1):
	for i in range(0,len(x_bins)-1):
		this_number, this_card = helper._make_cell(0,0,[[x_nums[i],-x_nums[i+1],y_nums[j],-y_nums[j+1],z_nums[0],-z_nums[1]]],universe=1)
#		fcell.write(this_card)
		ccc_nums.append(this_number)
		pos_coord.append([(x_bins[i]+x_bins[i+1])/2.,(y_bins[j]+y_bins[j+1])/2.,0.])
#fcell.close()
#
#
#
# write dist cards
#
# radius
values=[]
for i in range(0,(len(cosine_bins)-1)):
    values.append(weight_totals[i])
fsdef.write('c \n')
fsdef.write('c PIXEL "RADIUS"\n')
fsdef.write('c \n')
radius = numpy.linalg.norm( [ x_bins[1]-x_bins[0] , y_bins[1]-y_bins[0] ], ord=2)
make_independent_distribution(fsdef,1,[0.,radius])
indexing_start = 100
additive = 0.0
#
# angle
values=[]
for i in range(0,(len(cosine_bins)-1)):
    values.append(weight_totals[i])
fsdef.write('c \n')
fsdef.write('c ANGULAR DISTRIBUTION (COSINES => REVERSED ORDER FROM ANGLE)\n')
fsdef.write('c \n')
make_independent_distribution(fsdef,2,cosine_bins[::-1],values[::-1])
indexing_start = 100
additive = 0.0
#
# energy
bins=[]
values=[]
for i in range(0,(len(cosine_bins)-1)):
	if weight_totals[i] > 0.0:
		additive = 0.0
	else:
		additive = 1e-30
	bins.append(  ene)
	values.append(spec[i]+additive)
fsdef.write('c \n')
fsdef.write('c ENERGY DISTRIBUTIONS\n')
fsdef.write('c \n')
make_dependent_distribution(fsdef,3,indexing_start+(len(cosine_bins)-1)*0,bins[::-1],values[::-1])
#
# pos
bins=[]
values=[]
for i in range(0,(len(cosine_bins)-1)):
	bins.append(pos_coord)
	values.append(numpy.reshape( dist_reduced[i], dist_reduced[i].size, order='C'))
fsdef.write('c \n')
fsdef.write('c POS\n')
fsdef.write('c \n')
make_dependent_distribution(fsdef,4,indexing_start+(len(cosine_bins)-1)*1,bins[::-1],values[::-1],datatype='3float',option='L')
#
fsdef.write('c \n')
fsdef.close()
print "\nDONE.\n"

#  
#
# THIS IS THE OLD ONE WHERE I THOUGHT THE POS COULD DEPEND ON ON THE FINAL CCC (IT CAN'T -> MCNP COMPLAINS WHEN DEPENDENT ON A DEPENDENT)
# 
#
# write easy stuff
#fsdef.write('c\n')
#fsdef.write('c SDEF from %s, '%sys.argv[1]+time.strftime("%d.%m.%Y, %H:%M")+'\n')
#fsdef.write('c\n')
#fsdef.write('sdef    par=%s\n'%particle_symbols[this_particle])
#fsdef.write('c        sur=%5d\n'%this_sc)
#fsdef.write('        axs=1 0 0\n')
#fsdef.write('        vec=1 0 0\n')
#fsdef.write('        ext=0\n')
#fsdef.write('        tr=999\n')
#fsdef.write('        rad=d1\n')
#fsdef.write('        dir=d2\n')
#fsdef.write('        erg=fdir=d3\n')
#fsdef.write('        ccc=fdir=d4\n')
#fsdef.write('        pos=fccc=d5\n')
#fsdef.write('        wgt=%10.8E\n'%numpy.sum(weight_totals))
#fsdef.write('c \n')
#fsdef.write('c TRANSFORM\n')
#fsdef.write('c \n')
#fsdef.write('tr%3d   % 6.7E  % 6.7E  % 6.7E\n'%(xform_num,(1.0+offset_factor)*surface_center[0],(1.0+offset_factor)*surface_center[1],(1.0+offset_factor)*surface_center[2]))
#fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec1[0],surface_vec1[1],surface_vec1[2])) # (surface_rotation_xy,90-surface_rotation_xy,90))
#fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec2[0],surface_vec2[1],surface_vec2[2])) # (90+surface_rotation_xy,surface_rotation_xy,90))
#fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec3[0],surface_vec3[1],surface_vec3[2])) # (90,90,surface_rotation_yz))
##
## 
## make CCC surfaces/cells
##
## surfs
#mcnp_cell_helper.surface_transform = xform_num
#helper = mcnp_cell_helper('helper')
#x_nums = []
#y_nums = []
#z_nums = []
#ccc_nums = []
#pos_coord = []
#for i in range(0,len(x_bins)):
#	this_number, this_card = helper._make_plane(1,0,0,x_bins[i])
#	fsurf.write(this_card)
#	x_nums.append(this_number)
#for i in range(0,len(y_bins)):
#	this_number, this_card = helper._make_plane(0,1,0,y_bins[i])
#	fsurf.write(this_card)
#	y_nums.append(this_number)
#this_number, this_card = helper._make_plane(0,0,1,-0.5)
#fsurf.write(this_card)
#z_nums.append(this_number)
#this_number, this_card = helper._make_plane(0,0,1, 0.5)
#fsurf.write(this_card)
#z_nums.append(this_number)
#fsurf.close()
## cells
#for j in range(0,len(y_bins)-1):
#	for i in range(0,len(x_bins)-1):
#		this_number, this_card = helper._make_cell(0,0,[[x_nums[i],-x_nums[i+1],y_nums[j],-y_nums[j+1],z_nums[0],-z_nums[1]]],universe=1)
#		fcell.write(this_card)
#		ccc_nums.append(this_number)
#		pos_coord.append([(x_nums[i]+x_nums[i+1])/2.,(y_nums[j]+y_nums[j+1])/2.,0.])
#fcell.close()
##
##
##
## write dist cards
##
## radius
#values=[]
#for i in range(0,(len(cosine_bins)-1)):
#    values.append(weight_totals[i])
#fsdef.write('c \n')
#fsdef.write('c PIXEL "RADIUS"\n')
#fsdef.write('c \n')
#radius = numpy.linalg.norm( [ x_bins[1]-x_bins[0] , y_bins[1]-y_bins[0] ], ord=2)
#make_independent_distribution(fsdef,1,[0.,radius])
#indexing_start = 100
#additive = 0.0
##
## angle
#values=[]
#for i in range(0,(len(cosine_bins)-1)):
#    values.append(weight_totals[i])
#fsdef.write('c \n')
#fsdef.write('c ANGULAR DISTRIBUTION (COSINES => REVERSED ORDER FROM ANGLE)\n')
#fsdef.write('c \n')
#make_independent_distribution(fsdef,2,cosine_bins[::-1],values[::-1])
#indexing_start = 100
#additive = 0.0
##
## energy
#bins=[]
#values=[]
#for i in range(0,(len(cosine_bins)-1)):
#	if weight_totals[i] > 0.0:
#		additive = 0.0
#	else:
#		additive = 1e-30
#	bins.append(  ene)
#	values.append(spec[i]+additive)
#fsdef.write('c \n')
#fsdef.write('c ENERGY DISTRIBUTIONS\n')
#fsdef.write('c \n')
#make_dependent_distribution(fsdef,3,indexing_start+(len(cosine_bins)-1)*0,bins[::-1],values[::-1])
##
## ccc
#bins=[]
#values=[]
#for i in range(0,(len(cosine_bins)-1)):
#	bins.append(      ccc_nums)
#	values.append(numpy.reshape( dist_reduced[i], dist_reduced[i].size, order='C'))
#fsdef.write('c \n')
#fsdef.write('c CCC\n')
#fsdef.write('c \n')
#make_dependent_distribution(fsdef,4,indexing_start+(len(cosine_bins)-1)*1,bins[::-1],values[::-1],option='L',datatype='int')
##
## pos
#fsdef.write('c \n')
#fsdef.write('c POS\n')
#fsdef.write('c \n')
#make_dependent_variable(fsdef,5,pos_coord,datatype='3float')
##
#fsdef.write('c \n')
#fsdef.close()
#print "\nDONE.\n"
#
