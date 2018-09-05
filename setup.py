from distutils.core import setup, Extension
import os
import numpy as np

ss2py_module = Extension('ss2py', sources=['ss2py/ss2py.cpp','src/SurfaceSource.cpp','src/helpers.cpp'])
print np.get_include(),os.getcwd()+'/src'
setup(name='ss2py',
      version='1.0',
      description='Module for loading a MCNP wssa surface source file en masse.',
      include_dirs = [np.get_include(),os.getcwd()+'/src'],
      ext_modules=[ss2py_module])
