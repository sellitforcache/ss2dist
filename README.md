# ss2dist

c++ program to read tracks from MCNP6 surface source files.  Performs spatial binning within several energy bounds and well as creating a single independently-binned energy spectrum within a spatial limit.  See sample inputs.  Plotting script 'dist2plot.py' reads in the resulting binaries outputs and plots them using matplotlib.

building:  make 

usage:  ./ss2dist [wssa_file] [input_file]
