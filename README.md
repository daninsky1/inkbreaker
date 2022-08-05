
Clone this repository with submodules:  
`git clone --recurse-submodules https://github.com/daninsky1/inkbreaker.git`  
Install Cmake.  


Windows:

Install the Windows Build Tools for C/C++.  
Open the developer comand prompt to compile the project:  
Compile: `nmake /F make/Makefile.vc`  
Compile and run: `nmake run`  
Open VS debugger: `nmake debugger`  

Linux:  
Compile: `make -f make/Makefile`  

Todo:  
* Make a tree data structore to store the shapes.
* Make polygon and polygon edit tool.  
* Make Bezier and Bezier edit tool.  
* Invert Y on world coordinate to match a cartesian plane. This may be done on a translation layer or conversion functions between screen to world and vice versa.  
* Rewire the menu.  
* Make the persistence system with ORM.  
* Make layer system and widget to interact with layers, shapes and objects.    
* Make canvas system to act like artwork in Affinity Designer or cameras in Blender.  

Bugs:
* Polygon fill draw acting weird depending on the mouse position on x11.  
* Color picker draws the line according to the last state of the line in the application, it is not protected from external state changes.  
* Giant shapes break clipping when you zoom in, maybe because the conversion of world space to screen space make the shape to big to x11 handle. Or the conversion exceed the application screen space integer. It will probably make sense to deal with clipping within the application in the future, but for now a workaround is to limit the size of the world to not exceed the application's internal and external, x11 or gdi, limits.