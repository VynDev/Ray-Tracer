![preview](https://github.com/VynDev/Ray-Tracer/blob/main/images/preview.png)

Implementation from the book: https://raytracing.github.io/books/RayTracingInOneWeekend.html

## Build

### Using g++

```
g++ source/main.cpp -o RayTracer
```

### Using cmake & g++
```
mkdir build && cd build && cmake .. && make && cd -
```

## Run

Redirect the output of the program to a file:
```
./RayTracer > image.ppm
```
