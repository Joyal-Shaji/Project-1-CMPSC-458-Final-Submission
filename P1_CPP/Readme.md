# CMPSC 458 Project 1

Please fill out the README's content in your intermediate submission 2 and final submission.

Student Name: Joyal Shaji
Student ID: 975702213
Email: jxs7202@psu.edu

## How to run the Project

Open the .exe file and you will be presented with some boxes with awesome faces on them. You can move around using W(forward), A(left), S(back), D(right), and the mouse to look around; You can also scroll to zoom in and out. 

To control the boxes:
- U: Increase rotation rate in X axis
- J: Decrease rotation rate in X axis
- I: Increase rotation rate in Y axis
- K: Decrease rotation rate in Y axis
- O: Increase rotation rate in Z axis
- L: Decrease rotation rate in Z axis
- R: Reset all transformations
- P: Uniform scaling in all axis
- Shift + U: Increase scale in X axis
- Shift + J: Decrease scale in X axis
- Shift + I: Increase scale in Y axis
- Shift + K: Decrease scale in Y axis
- Shift + O: Increase scale in Z axis
- Shift + L: Decrease scale in Z axis
- Ctrl + U: Positive translation in X axis
- Ctrl + J: Negative translation in X axis
- Ctrl + I: Positive translation in Y axis
- Ctrl + K: Negative translation in Y axis
- Ctrl + O: Positive translation in Z axis
- Ctrl + L: Negative translation in Z axis

## Project description

In this project I added the box transformations from the second openGL tutorial and created a heightmap based on a grayscale image. The more brighter a pixel is on the image, the higher it appears on the heightmap. The texture on the heightmap is a grid so it is easy to see the contours of the heightmap. I added a skybox that is locked to the cameras translation so the user is never able to reach the skybox but they can look around. The main code is in `Project1.cpp` and the heightmap code is in `heightmap.hpp`.

## Extra credit attempt

N/A
