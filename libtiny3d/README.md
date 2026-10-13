libtiny3D is a minimal 3D software renderer written in C that supports wireframe rendering, basic lighting, and animation using custom vector/matrix math.

The project folder structure includes: include/ for headers, src/ for source files, demo/ for example programs, and build/ for compiled outputs.

To use this project, make sure you have GCC (or Clang), Make, and ImageMagick installed.

To build everything, open a terminal or PowerShell in the project root and run: make.

This compiles the library (libtiny3d.a) and demo binaries into build/bin.

To build only the library, use: make lib.

To build all demos, use: make demos.

To build a specific demo, for example the light animation, run: make demo_light.

The executable will be created at build/bin/demo_light.

Run it using: ./build/bin/demo_light (Linux/macOS) or .\build\bin\demo_light.exe (Windows/PowerShell).

The demo will output a sequence of .pgm image frames like frame0000.pgm, frame0001.pgm, etc.

To convert these frames into a GIF, use the following ImageMagick command:

magick -delay 5 -loop 0 frame*.pgm output.gif

If using an older version, the convert command may work but is deprecated: convert -delay 5 -loop 0 frame*.pgm output.gif

To clean up all compiled files, run: make clean.

You can modify or create new demos inside the demo/ folder and add them to the Makefile.

Lighting uses a fixed directional light and simple Lambert reflection based on edge vectors.

Animations can be driven by rotation matrices and Bezier paths (as used in animate_cube.c and mainlight.c).