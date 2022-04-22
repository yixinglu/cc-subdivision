The project implements the Catmull-Clark subdivision surface algorithm with C++.

The source includes following knowledge:

* half edge data structure representing mesh.
* C++11 syntax and `shared_ptr`, `weak_ptr`.
* freeglut opengl library.
* obj file reading implementation.

## Build

In ubuntu:

```shell
$ sudo apt install -y freeglut3-dev
$ mkdir build && cd build
$ cmake .. && make -j
```

## Usage

* `ccsubd filename.obj`
* up/down/left/right arrow key -> up/down/left/right rotation
* `c` key -> subdivison one time
* `u` key -> previous mesh
