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
			out_string =  '%5d %2s px % 10.8E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,d)
		elif a==0. and b==1. and c==0.:
			out_string =  '%5d %2s py % 10.8E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,d)
		elif a==0. and b==0. and c==1.:
			out_string =  '%5d %2s pz % 10.8E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,d)
		else:
			out_string =  '%5d %2s p % 10.8E % 10.8E % 10.8E % 10.8E'%(mcnp_cell_helper.plane_surface_number,mcnp_cell_helper.surface_transform_string,a,b,c,d)
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
			out_string = '%5d %2s cx   % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		else:
			out_string = '%5d %2s c/x  % 10.8E % 10.8E % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,y0,z0,r)
		# 
		mcnp_cell_helper.cyl_surface_number = mcnp_cell_helper.cyl_surface_number + 1
		return mcnp_cell_helper.cyl_surface_number-1, out_string+'\n'

	def _make_cyly(self, x0, z0, r):
		assert(r>0.)
		if x0==0. and z0==0.:
			out_string = '%5d %2s cy   % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		else:
			out_string = '%5d %2s c/y  % 10.8E % 10.8E % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,z0,r)
		# 
		mcnp_cell_helper.cyl_surface_number = mcnp_cell_helper.cyl_surface_number + 1
		return mcnp_cell_helper.cyl_surface_number-1, out_string+'\n'

	def _make_cylz(self, x0, y0, r):
		assert(r>0.)
		if x0==0. and y0==0.:
			out_string = '%5d %2s cz   % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		else:
			out_string = '%5d %2s c/z  % 10.8E % 10.8E % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,y0,r)
		# 
		mcnp_cell_helper.cyl_surface_number = mcnp_cell_helper.cyl_surface_number + 1
		return mcnp_cell_helper.cyl_surface_number-1, out_string+'\n'

	def _make_sphere(self, x0, y0, z0, r):
		assert(r>0.)
		if x0==0. and y0==0. and z0==0.:
			out_string = '%5d %2s so   % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,r)
		elif y0==0. and z0==0.:
			out_string = '%5d %2s sx   % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,r)
		elif x0==0. and z0==0.:
			out_string = '%5d %2s sy   % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,y0,r)
		elif x0==0. and y0==0.:
			out_string = '%5d %2s sz   % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,z0,r)
		else:
			out_string = '%5d %2s s    % 10.8E % 10.8E % 10.8E % 10.8E'%(mcnp_cell_helper.cyl_surface_number,mcnp_cell_helper.surface_transform_string,x0,y0,z0,r)
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
		string1=' % 10.8E'%vector_vars[k]
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
		string1=' %10.8E'%0.0
		total_len = total_len + len(string1)
		file_obj.write(string1)
		for k in range(0,len(vector_probs)):
			#if vector_probs[k]>0.0:
			string1=' %10.8E'%vector_probs[k]
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
	if datatype == 'float':
		data_string=' % 10.8E'
	elif datatype == 'int':
		data_string = ' %d'
	for k in range(0,len(vector_probs)):
		#if probs[k]>0.0:
		# SI card first
		string0 = 'SI%d %s '%(k+secondary_dist_start,option)
		file_obj.write(string0)
		total_len = len(string0)  
		for j in range(0,len(vector_vars[k])):
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
		string1=' %10.8E'%0.0
		total_len = total_len + len(string1)
		file_obj.write(string1)  
		for j in range(0,len(vector_probs[k])):
			string1=' %10.8E'%vector_probs[k][j]
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
		data_string=' %10.8E %10.8E %10.8E'
	if datatype == 'float':
		data_string=' %10.8E'
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

particle_symbols = {}
particle_symbols[0] = ' '
particle_symbols[1] = 'n'
particle_symbols[2] = 'p'


### load the dist file
fname = sys.argv[1][:-8]+'spec.bin'
spec_present= True
try:
	spec = numpy.fromfile(fname,dtype=numpy.float64)
except IOError:
    spec_present = False

if spec_present:

	cm  = plt.get_cmap('jet') 

	E_min		= spec[ 0]
	E_max		= spec[ 1]
	E_bins		= int(spec[ 2])
	theta_bins  = int(spec[ 3])
	x_min		= spec[ 4]
	x_max		= spec[ 5]
	y_min		= spec[ 6]
	y_max		= spec[ 7]
	theta_start =       8
	spec_start	=       8+theta_bins+1
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
	theta_edges = spec[theta_start:spec_start] 
	spec = spec[spec_start:]
	spec = numpy.reshape(spec,(theta_bins,E_bins))
	
	### constants
	charge_per_amp = 6.241e18
	charge_per_milliamp = charge_per_amp/1000.0
	
	### images
	fig  = plt.figure()
	ene = numpy.power(10,numpy.linspace(numpy.log10(E_min),numpy.log10(E_max),E_bins+1))
	ax1 = fig.add_subplot(111)
	cNorm  = colors.Normalize(vmin=0, vmax=theta_bins)
	scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)
	for j in range(0,theta_bins):
		colorVal = scalarMap.to_rgba(j)
		#print len(ene),len(spec[j,:])
		make_steps(ax1,ene,[0],spec[j,:],options=['log'],linewidth=2, color=colorVal,label=fname+' %5.2f-%5.2f deg'%(theta_edges[j],theta_edges[j+1]))
	ax1.grid(1)
	ax1.set_xlabel(r'Energy (MeV)')
	ax1.set_ylabel(r'Current (particles/source)')
	ax1.legend(loc='best')
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
else:
	print '4 or fewer arguments please'
	exit()

### first 18 values are the lengths, xy params, surface params
E_len			=   int(dist[ 0])
theta_len		=   int(dist[ 1])
phi_len			=   int(dist[ 2])
y_len			=   int(dist[ 3])
y_min			=   int(dist[ 4])
y_max			=   int(dist[ 5])
y_res			=   int(dist[ 6])
x_len			=   int(dist[ 7])
x_min			=   int(dist[ 8])
x_max			=   int(dist[ 9])
x_res			=   int(dist[10])
surf_a			= float(dist[11])
surf_b			= float(dist[12])
surf_c			= float(dist[13])
surf_d			= float(dist[14])
this_sc			=   int(dist[15])
surf_cx			= float(dist[16])
surf_cy			= float(dist[17])
surf_cz			= float(dist[18])
this_particle	=   int(dist[19])
dist_start		=            20

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
surface_center = numpy.array([surf_cx,surf_cy,surf_cz]) 
surface_normal = numpy.array([surf_a,surf_b,surf_c]) 
surface_vec1 = numpy.array([surf_b,surf_a,0.0]) 
surface_vec2 = numpy.array([0.0,0.0,1.0]) 
surface_vec3 = numpy.cross(surface_vec1,surface_vec2) 
#surface_rotation_xy = numpy.arctan(surface_normal[1]/surface_normal[0])*180.0/numpy.pi
#surface_rotation_yz = numpy.arccos(surface_normal[2])*180.0/numpy.pi  # not implemented yet

offset_factor=1e-6
xform_num = 999

# figure out angular probabilities
weight_totals=[]
for k in range(0,len(cosine_bins)-1):
    weight_totals.append(numpy.sum(dist_reduced[k][0]))
probs = numpy.array(weight_totals)/numpy.sum(weight_totals)
# files
sdef_name='%s.sdef'%sys.argv[1][:-9]
cell_name='%s.sdef.cell'%sys.argv[1][:-9]
surf_name='%s.sdef.surf'%sys.argv[1][:-9]
print "\nWriting MCNP SDEF to '"+sdef_name+"'..."
print "\nWriting MCNP SDEF CCC CELLS to '"+cell_name+"'..."
print "\nWriting MCNP SDEF CCC SURFS to '"+surf_name+"'..."
if sphere:
	pass
else:
	print "\nSDEF plane offset by % 3.2E...\n"%offset_factor
fsdef=open(sdef_name,'w')
fcell=open(cell_name,'w')
fsurf=open(surf_name,'w')
# write easy stuff
fsdef.write('c\n')
fsdef.write('c SDEF from %s, '%sys.argv[1]+time.strftime("%d.%m.%Y, %H:%M")+'\n')
fsdef.write('c\n')
fsdef.write('sdef    par=%s\n'%particle_symbols[this_particle])
fsdef.write('c        sur=%5d\n'%this_sc)
fsdef.write('        axs=1 0 0\n')
fsdef.write('        vec=1 0 0\n')
fsdef.write('        ext=0\n')
fsdef.write('        tr=999\n')
fsdef.write('        rad=d1\n')
fsdef.write('        dir=d2\n')
fsdef.write('        erg=fdir=d3\n')
fsdef.write('        ccc=fdir=d4\n')
fsdef.write('        pos=fccc=d5\n')
fsdef.write('        wgt=%10.8E\n'%numpy.sum(weight_totals))
fsdef.write('c \n')
fsdef.write('c TRANSFORM\n')
fsdef.write('c \n')
fsdef.write('tr%3d   % 6.7E  % 6.7E  % 6.7E\n'%(xform_num,(1.0+offset_factor)*surface_center[0],(1.0+offset_factor)*surface_center[1],(1.0+offset_factor)*surface_center[2]))
fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec1[0],surface_vec1[1],surface_vec1[2])) # (surface_rotation_xy,90-surface_rotation_xy,90))
fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec2[0],surface_vec2[1],surface_vec2[2])) # (90+surface_rotation_xy,surface_rotation_xy,90))
fsdef.write('        % 6.7E  % 6.7E  % 6.7E\n'%(surface_vec3[0],surface_vec3[1],surface_vec3[2])) # (90,90,surface_rotation_yz))
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
	fsurf.write(this_card)
	x_nums.append(this_number)
for i in range(0,len(y_bins)):
	this_number, this_card = helper._make_plane(0,1,0,y_bins[i])
	fsurf.write(this_card)
	y_nums.append(this_number)
this_number, this_card = helper._make_plane(0,0,1,-0.5)
fsurf.write(this_card)
z_nums.append(this_number)
this_number, this_card = helper._make_plane(0,0,1, 0.5)
fsurf.write(this_card)
z_nums.append(this_number)
fsurf.close()
# cells
for j in range(0,len(y_bins)-1):
	for i in range(0,len(x_bins)-1):
		this_number, this_card = helper._make_cell(0,0,[[x_nums[i],-x_nums[i+1],y_nums[j],-y_nums[j+1],z_nums[0],-z_nums[1]]],universe=1)
		fcell.write(this_card)
		ccc_nums.append(this_number)
		pos_coord.append([(x_nums[i]+x_nums[i+1])/2.,(y_nums[j]+y_nums[j+1])/2.,0.])
fcell.close()
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
# ccc
bins=[]
values=[]
for i in range(0,(len(cosine_bins)-1)):
	bins.append(      ccc_nums)
	values.append(numpy.reshape( dist_reduced[i], dist_reduced[i].size, order='C'))
fsdef.write('c \n')
fsdef.write('c CCC\n')
fsdef.write('c \n')
make_dependent_distribution(fsdef,4,indexing_start+(len(cosine_bins)-1)*1,bins[::-1],values[::-1],option='L',datatype='int')
#
# pos
fsdef.write('c \n')
fsdef.write('c POS\n')
fsdef.write('c \n')
make_dependent_variable(fsdef,5,pos_coord,datatype='3float')
#
fsdef.write('c \n')
fsdef.close()
print "\nDONE.\n"
