from distutils.core import setup, Extension
import os, re
import numpy as np

# get cmake prefix from install
# apparently LD_LIBRARY_PATH is not visible, so ss2lib must be explicity located
cmake_prefix=False
result = [os.path.join(dp, f) for dp, dn, filenames in os.walk('.') for f in filenames if os.path.splitext(f)[1] == '.txt']
for file in result:
    file_string = open(file,'r').read()
    match = re.search('CMAKE_INSTALL_PREFIX[A-Z:]*=(.+)\n',file_string)
    if match:
        cmake_prefix = match.group(1)
if cmake_prefix:
    print "\ncmake installation cache files detected..."
    print "CMAKE_INSTALL_PREFIX=%s"%cmake_prefix
    print "assuming library is in:"
    print "CMAKE_INSTALL_PREFIX=%s/lib\n"%cmake_prefix
else:
    print "\ncmake installation cache files not detected..."
    print "!!! YOU PROBABLY NEED TO INSTALL THE LIBRARY WITH CMAKE !!!\n"
    exit(0)


#
# distutils stuff
#
ss2py_module = Extension('ss2py',   sources=['ss2py/ss2py.cpp'],
                                    libraries = ['ss2lib'],
                                    library_dirs = [cmake_prefix+'/lib'],
                                    extra_compile_args=['-std=c++11']
                                    )

setup(name='ss2py',
      version='1.0',
      description='Module for loading a MCNP wssa surface source file en masse.',
      #extra_compile_args=['-std=c++11'],
      include_dirs = [  np.get_include(), os.getcwd()+'/src'],
      ext_modules=[ss2py_module]
      )
