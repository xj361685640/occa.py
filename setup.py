import os
from setuptools import setup
from subprocess import check_call
from distutils.command.sdist import sdist

DIR = os.path.dirname(__file__)

def build_occa():
    if not os.path.exists('.git'):
        return
    # Pull occa
    check_call('git submodule update --init'.split())
    # Compile occa
    check_call('make CXXFLAGS=-O3 -j4'.split(),
               cwd=os.path.join(DIR, 'submodules/occa'))

def build_c_wrapper():
    # Compile python occa c module
    check_call('make'.split())

class sdist_build(sdist):
    def run(self):
        build_occa()
        sdist.run(self)

def setup_package():
    # Import after compiling occa.c
    from occa import version
    setup(
        name='occa',
        version=str(version),

        description='OCCA: Portable Approach for Parallel Architectures',
        long_description="""OCCA is an open-source (MIT license) library used to program current multi-core/many-core architectures. Devices (such as CPUs, GPUs, Intel's Xeon Phi, FPGAs, etc) are abstracted using an offload-model for application development and programming for the devices is done through a C-based (OKL) kernel. OCCA gives developers the ability to target devices at run-time by using run-time compilation for device kernels.""",

        url='http://occa.readthedocs.io',

        author='David Medina',
        author_email='dmed256@gmail.com',

        license='MIT',

        classifiers=[
            'Development Status :: 4 - Beta',

            'License :: OSI Approved :: MIT License',

            'Operating System :: Unix',
            'Operating System :: POSIX',
            'Operating System :: MacOS',

            'Intended Audience :: Developers',
            'Intended Audience :: Science/Research',

            'Topic :: Scientific/Engineering',
            'Topic :: Software Development',

            'Programming Language :: C',
            'Programming Language :: Python :: 2',
            'Programming Language :: Python :: 2.6',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: 3.3',
            'Programming Language :: Python :: 3.4',
            'Programming Language :: Python :: 3.5',
            'Programming Language :: Python :: 3.6',
        ],

        keywords='occa hpc gpu jit openmp opencl cuda',

        install_requires=['numpy'],

        cmdclass={
            'sdist': sdist_build,
        },
    )

if __name__ == '__main__':
    setup_package()