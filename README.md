# Binarization
Application in C++ with OpenMP and OpenCV to get a binary image from a color image.

## :construction_worker: How to run
Although instructions below work on ubuntu,
the application can be run in other
Operating Systems such as Windows, Mac OS and
other linux distributions.

### :gear: Prerequisites

#### Install OpenMP
The GNU Compiler Collection (GCC) supports OpenMP, thus
there is no need of installation instructions if use this compiler.

#### Install OpenCV
```bash
sudo apt-get install libopencv-dev
```

### 💻 Compile and Run application
```bash
g++ main.cpp -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -fopenmp
```
```bash
./main a.out origin_image.extension new_image.extension
```
The 'origin_image' is the color image to binarize for.
It must be located inside the 'images' folder.
The new image will be generated inside 'images' folder.
The name of it is 'new_image.extension'.
This second argument is optional and if it is not present,
the origin image will be replaced by new image.
