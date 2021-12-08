.SILENT:
.IGNORE:
# Source
SRC_DIR=src
SRC=src\main.cpp src\main_window.cpp src\view2d.cpp src\shapes.cpp vendor\sqlite\sqlite3.c

# Objects
OBJ=win32.obj

# Headers
HDR=

DCFLAGS=/EHsc /Zi /Od /FC /nologo /MDd /MP /W3 /diagnostics:column
RCFLAGS=/O2 /FC /nologo

BUILD_DIR=build/

SYS_LIBS=User32.lib kernel32.lib gdi32.lib gdiplus.lib comdlg32.lib advapi32.lib shell32.lib \
		 ole32.lib uuid.lib Ws2_32.lib comctl32.lib
INCLUDE_DIR=/Ivendor\fltk /Ivendor\fltk\msvc\lib\Debug /Ivendor\fltk\msvc /Ivendor
LIB_SRC_DIR=vendor\fltk\src vendor\sqlite vendor\stb
LIB_SRC=vendor\fltk\src\* vendor\sqlite\* vendor\stb\*

FLTK_LIB=vendor\fltk\msvc\lib\Debug\fltkd.lib
PROGRAM=inkbreaker.exe
DEBUG_PROGRAM_NAME=dinkbreaker
DEBUG_PROGRAM=dinkbreaker.exe

all: $(DEBUG_PROGRAM)

$(DEBUG_PROGRAM): $(SRC)
	if not exist $(BUILD_DIR) mkdir build
	if not exist $(BUILD_DIR)debug mkdir $(BUILD_DIR)debug
	$(CC) $(DCFLAGS) $(SRC) $(SYS_LIBS) $(INCLUDE_DIR) $(FLTK_LIB) /Fo$(BUILD_DIR)debug/ /Fe$(BUILD_DIR)debug/$(DEBUG_PROGRAM) /link /PDB:$(BUILD_DIR)debug/$(DEBUG_PROGRAM_NAME).pdb

$(PROGRAM): $(SRC)
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	if not exist $(BUILD_DIR)release mkdir $(BUILD_DIR)release 
	$(CC) $(RCFLAGS) $(SRC) $(WIN32_LIBS) /Fe$(BUILD_DIR)release\$(PROGRAM)

clean:
	del build /s /q

tags: $(SRC) $(LIB_SRC)
	ctags -R --kinds-C=+l --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ --sort=true $(SRC_DIR) $(LIB_SRC_DIR)

