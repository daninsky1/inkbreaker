# Make the build silent
.SILENT:

# Define DEBUG_BUILD to make a debug build otherwise will make a release build
DEBUG_BUILD=
# IO_SQLITE=

SRC=src\main.cpp src\main_window.cpp src\shapes.cpp src\view2d.cpp 

OBJ=src\main.o src\main_window.o src\shapes.o src\view2d.o

BUILD_DIR=build/

WIN32=User32.lib kernel32.lib gdi32.lib gdiplus.lib comdlg32.lib advapi32.lib shell32.lib \
	 ole32.lib uuid.lib Ws2_32.lib comctl32.lib

FLTK_DIR=vendor\fltk
FLTK_BUILD_DIR=$(FLTK_DIR)\build

INCLUDE_DIRS=/Ivendor\fltk /Ivendor\fltk\build /Ivendor
LIBS=$(WIN32) $(FLTK)
# LIBS_SRC_DIRS=vendor\fltk\src vendor\sqlite

DEBUGGER=devenv

!IFDEF IO_SQLITE
DEFINES=/DIO_SQLITE
SQLITE=vendor\sqlite\sqlite3.c
!ELSE
DEFINES=
SQLITE=
!ENDIF

!IFDEF DEBUG_BUILD
BUILD_TYPE=Debug
CPPFLAGS=/EHsc /Zi /Od /FC /nologo /MDd /MP /W3 /diagnostics:column
PROGRAM=inkbreakerd.exe
FLTK=$(FLTK_BUILD_DIR)\lib\$(BUILD_TYPE)\fltkd.lib
!ELSE
BUILD_TYPE=Release
CPPFLAGS=/O2 /FC /nologo /MP /MD
PROGRAM=inkbreaker.exe
FLTK=$(FLTK_BUILD_DIR)\lib\$(BUILD_TYPE)\fltk.lib
!ENDIF

all: $(PROGRAM)

$(PROGRAM): $(SRC) $(FLTK)
	echo Windows $(BUILD_TYPE) build
	if not exist $(BUILD_DIR) mkdir build
	$(CC) $(CPPFLAGS) $(SRC) $(SQLITE) $(WIN32) $(FLTK) $(INCLUDE_DIRS) $(DEFINES) /Fo$(BUILD_DIR) /Fe$(BUILD_DIR)$* \
		/link /PDB:$(BUILD_DIR)$*.pdb

fltk: $(FLTK)
$(FLTK):
	echo FLTK $(BUILD_TYPE) build
	if not exist $(FLTK_BUILD_DIR) mkdir $(FLTK_BUILD_DIR)
	if not exist $(FLTK_BUILD_DIR)\FLTL.sln cmake -S$(FLTK_DIR) -B$(FLTK_BUILD_DIR)
	MSBuild -nologo $(FLTK_BUILD_DIR)\FLTK.sln -p:Configuration=$(BUILD_TYPE) -p:Platform=x64

run: $(PROGRAM)
	.\build\$(PROGRAM)

debugger: $(PROGRAM)
	$(DEBUGGER) /nosplash .\build\$(PROGRAM)

clean:
	del build /s /q

tags: $(SRC)
	del /q tags
	ctags -R --kinds-C=+l --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ --sort=true $(SRC)


test:
	echo $(SRC)
