from setuptools import Extension, setup

module = Extension("spkmeansEx",
                  sources=[
                    'spkmeans.c',
                    'spkmeansmodule.c',
                    'spkmeanslogic.c'
                    # 'Utils.c'
                  ])
setup(name='spkmeansEx',
     version='1.0',
     description='Python wrapper for kmeans C extension',
     ext_modules=[module])