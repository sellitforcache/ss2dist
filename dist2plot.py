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


### load the dist file
dist = numpy.fromfile(sys.argv[1],dtype=numpy.float64)

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
#zap_x1=[-6.6, -19.1, -19.1, -6.6, -6.6]
#zap_x2=[4.6,19.1 ,19.1 ,4.6, 4.6]
#zap_y=[7.05, 7.05 ,-7.05, -7.05, 7.05]
#x_AMOR=[2.5,2.5,-2.5,-2.5,2.5]
#y_AMOR=[-6,6,6,-6,-6]
#x_FOCUS=[-1.76,-1.76,-6.76,-6.76,-1.76]
#y_FOCUS=[-6,6,6,-6,-6]
upper_lim=[5e9,1e10,100]
phi_bin=0
for theta_bin in range(0,len(theta_bins)-1):
	for E_bin in range(0,len(E_bins)-1):
		f = plt.figure()
		ax = f.add_subplot(111)
		if logplot:
			imgplot = ax.imshow(dist[E_bin][theta_bin][phi_bin][:][:]*charge_per_milliamp          ,extent=[x_bins[0],x_bins[-1],y_bins[0],y_bins[-1]],origin='lower',cmap=plt.get_cmap('jet'),norm=LogNorm(vmax=upper_lim[E_bin]))
		else:
			imgplot = ax.imshow(dist[E_bin][theta_bin][phi_bin][:][:]*charge_per_milliamp          ,extent=[x_bins[0],x_bins[-1],y_bins[0],y_bins[-1]],origin='lower',cmap=plt.get_cmap('jet'),vmax=upper_lim[E_bin])
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
		f.savefig('dist_e%d_theta%d'%(E_bin,theta_bin))
		plt.show()
