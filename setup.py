from distutils.core import setup, Extension
import os
import numpy as np

ss2py_module = Extension('ss2py',   sources=['ss2py/ss2py.cpp'],
                                    libraries = ['ss2lib'],
                                    library_dirs = ['/home/l_bergmann/Software/lib/'],
                                    extra_compile_args=['-std=c++11']
                                    )

setup(name='ss2py',
      version='1.0',
      description='Module for loading a MCNP wssa surface source file en masse.',
      extra_compile_args=['-std=c++11'],
      include_dirs = [  np.get_include(),
                        os.getcwd(),
                        os.getcwd()+'/src/SurfaceSource/',
                        os.getcwd()+'/src/Histogram/',
                        os.getcwd()+'/src/InputFile/',
                        os.getcwd()+'/src/helpers/'],
      ext_modules=[ss2py_module]
      )
