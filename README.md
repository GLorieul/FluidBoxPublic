
Logo: a whirlpool inside a box

# What is fluid box

FluidBox is a sandbox for fluid mechanics.

It is not meant to help you build the most efficient solver ever (if that is what you are looking for, then consider PETsc or Balisisk (?) instead). Instead it is a library that encourages the writing of human-comprehensible code with as little performance penalty as possible. As such it is well suited for experimenting with numerical methods, either for researching new numerical methods, or for prototyping before writing a solver with the highest efficiency possible.


# Compilation & install

## Step 1: make sure all dependencies are met

First make sure all dependencies are met. The dependencies are:
 - Python3: python3 (require both for plotting and (soon) performing compilation)
 - HDF5: libhdf5-dev libhdf5-cpp (Required for saving/loading data to/from files)
 - (soon) HYPRE: libhypre-dev (Elliptic solver)
 - (soon) FFTW: libfftw3-dev (Fourier transform library)
 - (soon) libconfig++: libconfig++ (Library for parsing configuration files).
 - (soon) google-test: google-test (Unit testing utility, must be built from the gitfor your system)
 - Python3 modules: matplotlib, numpy, scipy, argparse (Required for plotting scripts)

## Step 2: create/edit Makefile.cfg

Fluidbox does not use cmake, nor does it use a `./configuration` script.
Instead, you need to manual write/edit a `Makefile.cfg` file. You can inspire yourself from `Makefile.cfg.template` that is provided as an example (actually you should copy it), and adapt the variables' values to match your system.

## Step 3: compile!

Run the usual:

    make clean
    make

## Step 4: Install

At the current stage, fluidbox does not compile into a library (e.g. "*.so" or "*.a" file). Hence there is no `make install` to be invoked.

