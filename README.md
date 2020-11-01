# Rubik Cube

## Input

* W/S - spin line
* A/D - select previous/next line
* Q - change axis
* Spacebar - switch between score display and cube display
* MouseWheelUp - zoom in
* MouseWheelDown - zoom out
* Click and drag - spin cube
* P - Pause/start animation
* Esc - Stop game

## Dependencies

* FreeGLUT/GLUT

## Build

```
meson ./build
ninja -C ./build
```

## Run

```
./build/rubik
```
