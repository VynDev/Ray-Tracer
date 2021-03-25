![preview](https://github.com/VynDev/Ray-Tracer/blob/main/images/preview.png)

Implementation from the book: https://raytracing.github.io/books/RayTracingInOneWeekend.html

## Build the source (Linux)

## Requirements

- SDL2 (search how to install it for your OS)

## Compile

### Using g++

```
g++ source/main.cpp -o RayTracer gcc $(sdl2-config --cflags --libs)
```

### Using cmake & g++
```
mkdir build && cd build && cmake .. && make && cd -
```

## Run

```
./RayTracer
```