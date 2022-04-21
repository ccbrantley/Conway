## Conway_C
My attempt at Conway's Game of Life in C/C++. The application initializes a n * m array with randomly generated 0s and 1s, where 0 is an inactive cell and 1 is an active cell. It then iteratively applies the following rules to each cell based on the count of its active neighbours:
<ol>
  <li> If the cell has three active neighbours, the cell's state is active.</li>
  <li> If the cell is active and has two active neighbours, the cell's state remains active.</li>
  <li> Otherwise the cell's state is inactive.</li>
</ol>

## Motivation
The motivation behind this project was to create a fun project that utilizes X11.

## Features
<ol>
  <li>Cell count easily modified by two variables (xUnits, yUnits).</li>
  <li>Cell's can be rendered by either drawing pixels or filling rectangles.</li>
  <li>Frames per second can be modified by changing input to ConwayDisplay.</li>
</ol>

## Possible Improvements
<ol>
  <li>Utilizing double buffer with OpenGL to prevent flickering at high cell count.</li>
  <li>Only calculating and applying rules to active cells and their neighbours.</li>
  <li>Catching resize events to apply dynamic projection.</li>
</ol>

## Requirements
The requirements are xorg, the X11 library, and a c++ compiler.

## Screenshots

![Peek 2022-04-19 03-40](https://user-images.githubusercontent.com/19720138/163951404-b964a76f-fbc2-4bcc-a01a-93332dc48e2b.gif)
