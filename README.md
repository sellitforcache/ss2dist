# ss2dist

Contains:
* C++ library to read tracks from MCNP6 surface source files, create histograms, and parse some simple input files
* Executables to create angle-dependent spatial distributions and energy spectra.  
* Plotting script 'dist2plot.py' that reads in the resulting binary outputs and plots them using matplotlib.  At successful plotting, dist2plot.py also writes a MCNP sdef card based on the histograms it got from ss2dist.

## Installing C++ library and and executables

Building uses cmake (>3.1).  Recommended way to build (you can always use 'ccmake' instead of 'cmake' to use an interactive GUI to set install variables):

```
$ mkdir BUILD
$ cd BUILD
$ cmake .. -DCMAKE_INSTALL_PREFIX:PATH=[/path/where/you/want/to/install]
$ make
$ make install
```

## Installing Python module

After the library and executables are built and installed, the Python module can be built and installed (requires ss2lib, so it must be already be installed).

```
$ python setup.py install
```


## ss2dist Usage

See example inputs and scripts.  Generally it goes like this:

```
$ ss2dist [wssa_file] [input_file]
```

This will produce two files, named '[surface_number]\_[particle]\_dist.bin' and '[surface_number]\_[particle]\_spec.bin' which contain the histogram data.  They are then post-processed with dist2plot.py (which means the wssa files doesn't need to be read if you simply want to change some plot parameters):

```
$ dist2plot.py [surface_number]_[particle]_dist.bin [OPTIONS]
```

Where [OPTIONS] can be any combination/order of:
* plot : Make popup plots
* png  : Write .png files for all plots
* log  : Logarithmic scale for spatial distributions
* vmin=[number] : set the minimum value for the spatial distribution colormap
* vmax=[number] : set the maximum value for the spatial distribution colormap
* smooth=[integer] : number of bins to smooth over in spectral plots

If all the plotting completes successfully, then the script writes a MCNP sdef card into a file named '[surface_number]\_[particle].sdef'
