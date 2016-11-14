#! /usr/local/bin/python 
# /home/l_bergmann/anaconda/bin/python -W ignore
#
# ss2dist, the MCNP surface source to histogram distribution maker
# Ryan M. Bergmann, March 2015 
# ryan.bergmann@psi.ch, ryanmbergmann@gmail.com

import numpy
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cmx
from matplotlib import gridspec
from matplotlib.colors import LogNorm


### load the dist file
dist = numpy.fromfile("dist.bin",dtype=numpy.float64)

### first 5 values are the lengths
E_len		= int(dist[0])
theta_len	= int(dist[1])
phi_len		= int(dist[2])
y_len		= int(dist[3])
x_len		= int(dist[4])

#
print "%10s %5d"%("E_len",		E_len		)
print "%10s %5d"%("theta_len",	theta_len	)
print "%10s %5d"%("phi_len",	phi_len		)
print "%10s %5d"%("y_len",		y_len		)
print "%10s %5d"%("x_len",		x_len		)

### remove the header info and reshape for easier indexing
dist = dist[5:]
dist = numpy.resize(dist,(E_len,theta_len,phi_len,y_len,x_len))


charge_per_amp = 6.241e18
charge_per_milliamp = charge_per_amp/1000.0


E_bins   	= numpy.array([1e-11,1e-6,1.0,600.])
x_bins   	= numpy.linspace(-425,425,1701)
y_bins   	= numpy.linspace(-153.5,286.0,880)
theta_bins 	= numpy.array([0,90])*numpy.pi/180.0
phi_bins 	= numpy.linspace(0,2*numpy.pi,2) 

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
upper_lim=[5e9,1e10,1e10]
phi_bin=0
for theta_bin in range(0,len(theta_bins)-1):
	for E_bin in range(0,len(E_bins)-1):
		f = plt.figure()
		ax = f.add_subplot(111)
		imgplot = ax.imshow(dist[E_bin][theta_bin][phi_bin][:][:]*charge_per_milliamp          ,extent=[x_bins[0],x_bins[-1],y_bins[0],y_bins[-1]],origin='lower',cmap=plt.get_cmap('jet'),norm=LogNorm(vmin=1e8, vmax=upper_lim[E_bin]))
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
#		#
#		#
#		# plot weight histogram
#		f2 = plt.figure()
#		ax2 = f2.add_subplot(111)
#		make_steps(ax2,histograms_wght[theta_bin].bins,[0],histograms_wght[theta_bin].values,linewidth=1,label='',options=['log'])
#		ax2.set_ylabel(r'Number')
#		ax2.set_xlabel(r'Weight')
#		ax2.grid(1)
#		plt.show()
