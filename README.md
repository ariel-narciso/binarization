# Binarization
Application in C++ with OpenMP and OpenCV to get a binary image from a color image

## :construction_worker: How to run

### :gear: Prerequisites
Install OpenCV
```bash
sudo apt-get install libopencv-dev
```

### 💻 Compile and Running application
```bash
g++ main.cpp -o main -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -fopenmp
```
```bash
./main origin_image.extension new_image.extension
```
The 'origin_image' must be located in 'images' folder.
The new image will be generated in 'images' folder.
This second argument is optional and if it is not present,
the origin image will be replaced by new image
