from setuptools import Extension, setup

module = Extension("mykmeanssp",
                  sources=[
                    'spkmeans.c',
                    'spkmeansmodule.c'
                  ])
setup(name='mykmeanssp',
     version='1.0',
     description='Python wrapper for kmeans C extension',
     ext_modules=[module])