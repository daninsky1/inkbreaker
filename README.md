# How to get the project
`git clone --recurse-submodules https://github.com/daninsky1/inkbreaker.git`  

# How to build on Windows
## Using zig compiler
Download zig [https://ziglang.org/](https://ziglang.org/).  
Add the zig.exe to the PATH environment variable.

### Build FLTK
```sh
cd vendor/fltk
make -j<num_threads> CXX="zig c++ -lgdi32 -lcomdlg32"
```

### Build
Open `build.sh` set the CC variable to `zig c++ -lgdi32 -lcomdlg32`, and build on the project root directory
```sh
./build.sh
```

## Using MinGW GCC/Clang
Install MSYS2 [https://www.msys2.org/](https://www.msys2.org/).  
Open the UCRT64 shell environment  
```sh
# GCC
pacman -Syu
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```
```sh
# Clang
pacman -Syu
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-clang
```
### Build FLTK
```sh
# GCC
cd vendor/fltk
make -j<num_threads>
```
```sh
# Clang
cd vendor/fltk
make -j<num_threads> CXX="clang"
```

### Build
Open `build.sh` set the CC variable to `gcc` or `clang`, and build on the project root directory
```sh
./build.sh
```

## Using native tools, MSVC
Build on Windows:
Install the Windows Build Tools for C/C++.  
Open the developer comand prompt to compile the project:  
Compile: `nmake /F make/Makefile.vc`  
Compile and Run: `nmake /F make/Makefile.vc && nmake /F make/Makefile.vc run`  
Open VS Debugger: `nmake debugger`  

Build on Linux:  
Compile: `make -f make/Makefile`  
Compile and Run: `make -f make/Makefile && make -f make/Makefile run`  

Build the FLTK:


# How to build on Linux
## Using zig compiler
Download zig [https://ziglang.org/](https://ziglang.org/).  
Add the zig.exe to the PATH environment variable.

### Build FLTK
```sh
cd vendor/fltk
make -j<num_threads> CXX="zig c++"
```

### Build
Open `build.sh` set the CC variable to `zig c++`, and build on the project root directory
```sh
./build.sh
```

## Using native tools, GCC
For Debian package manager systems
```sh
apt update -y && apt upgrade -y
apt install build-essential -y
```
```sh
### Build FLTK
```sh
cd vendor/fltk
make -j<num_threads>
```
### Build
```sh
./build.sh
```

Todo:  
* Make polygon and polygon edit tool.  
* Make Bezier and Bezier edit tool.  
* Invert Y on world coordinate to match a cartesian plane. This may be done on a translation layer or conversion functions between screen to world and vice versa.  
* Rewire the menu.  
* Make the persistence system with ORM or some other database system.  
* Make layer system and widget to interact with layers, shapes and objects.    
* Make canvas system to act like artwork in Affinity Designer or cameras in Blender.  

Bugs:
* Polygon fill draw acting weird depending on the mouse position on x11.  
* Color picker draws the line according to the last state of the line in the application, it is not protected from external state changes.  
* Giant shapes break clipping when you zoom in, maybe because the conversion of world space to screen space make the shape to big to x11 handle. Or the conversion exceed the application screen space integer. It will probably make sense to deal with clipping within the application in the future, but for now a workaround is to limit the size of the world to not exceed the application's internal and external, x11 or gdi limits.
