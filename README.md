# Binarization
Application in C++ with OpenMP and OpenCV to get a binary image from a color image

## :construction_worker: How to run

### :gear: Prerequisites

```bash
# Install OpenCV
$ sudo apt-get install libopencv-dev
```
### 💻 Compile and Running application
```bash
$ g++ main.cpp -o main -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -fopenmp
$ ./main image_to_binarizing.extension new_image.extension
```
Where 'image_to_binarizing' is located in 'images' folder.
The new image will be generated in 'images' folder.
This second argument is optional and if it is not present,
the origin image will be replaced by new image
