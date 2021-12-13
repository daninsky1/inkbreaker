dependecies:
fltk
sqlite

Windows:</br>
To compile you need install the Windows Build Tools for C/C++.
Create a vendor directory: `mkdir vendor`.</br>
Inside vendor directory clone the FLTK repo: `git clone <FLTK_repo>`.</br>
Download and extract sqlite C source code amalgamation directory and rename it to sqlite3.

Open the developer comand prompt to compile FLTK and the project:
Compile FLTK:
```
cd vendor\fltk
mkdir build && cd build
cmake ..
msbuild -nologo FLTK.sln -p:Configuration=Debug -p:Pratform=x64
```

Compile project on the project directory:
```
nmake /a
```

TODOS:
* Ctrl + c, ctrl + v
* Save and Open feature
* 

* add node editor
* make node editor line gradient:
https://www.youtube.com/watch?list=PLyGoWAGEPJ1Bayny7kYzsOpBC3NrQhFEY&v=d18o5HrBRVI
