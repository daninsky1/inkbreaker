
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
* Remake Shapes the inheritance structure to be able to represent containers of shapes like a directory and the shapes itself been special containers of other shapes.  
* The future shape container will keep track if its group contains only one type of shapes being pure or not being dirty, to diferentiate what king of operations we can do on those containers.
