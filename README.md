
# icepack-py

This repository contains python bindings and helper scripts for the glacier flow modelling library [icepack](https://github.com/icepack/icepack).
The core icepack library is written in C++; this repository provides wrappers that allow you to use icepack from python 3.

### Installation

You will first need a working install of icepack in your path, along with:

* Python 3
* [pybind11](https://github.com/pybind/pybind11): provides functions for interoperating between C++ and python

To install the python bindings for icepack, run the command

    python3 setup.py install

at the terminal.
If you don't want to install icepack-py globally, you can also use

    python3 setup.py install --user

to install it in your home directory (`$HOME/.local` on Linux, `$HOME/Library/Python` on OSX).

Additional dependences for the helper scripts:

* numpy, scipy
* matplotlib

