# LIBSIB

A C++ library for computing _Smallest Intersecting Balls_ in arbitrary dimensions.

## Repository Structure

```
Data/                      # Folder contains some small data files
EXE/                       # Includes a pre-build executable file
LIBSIB/
├ DS/                      # Data structures
├ data_io.h                # Data IO
├ debug.h                  # Debugging
├ libsib.h                 # The main algorithm
├ product_cone.h           # The product cone
├ scmwu.h                  # The SCMWU algorithm
└ soc.h                    # The second-order cone
data_generator.cpp         # Data generator (independent with main.cpp)
main.cpp                   # The main procedure
```

## Usage

```
How to use:
  libsib [type] -f [file] [*options]

Arguments:
  -h: print help page
  [type]: type of objects to be intersected, currently support
          ├ point: a set of points
          ├ poly: a set of convex polytopes
          ├ rpoly: a set of reduced polytopes
          ├ aabb: a set of axis-aligned bounding boxes
          ├ ball: a set of Euclidean balls
          └ ellip: a set of ellipsoids
  -f [file]: (compulsory) specify the name of the data file, usually a binary file *.bin
  -p [path]: (optional) specify the path to the input data, the current directory by default
  -l [logfile]: (optional) output the statistics to a log file
```

Sample run with the pre-build EXE file:

```
$ cd EXE
$ libsib.exe point -p ../Data -f pset_2d_8_#0.bin
---- statistics ----
type of object: point
dimension: 2
number of objects: 8
final radius: 3.758233e+00
iteration count: 994
running time: 1 ms
```

## Dependency

To build LIBSIB from the source, you need to include [Boost](https://www.boost.org/) and [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page). Versions that we are using are Boost 1.82.0 and Eigen 3.4.0.
