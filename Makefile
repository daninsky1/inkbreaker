# Make the build silent
.SILENT:

# Define DEBUG_BUILD to make a debug build otherwise will make a release build
DEBUG_BUILD=

SRC=src\main.cpp src\main_window.cpp src\shapes.cpp vendor\sqlite\sqlite3.c src\view2d.cpp 

OBJ=src\main.o src\main_window.o src\shapes.o vendor\sqlite\sqlite3.o src\view2d.o

DBGCFLAGS=/EHsc /Zi /Od /FC /nologo /MDd /MP /W3 /diagnostics:column
RELCFLAGS=/O2 /FC /nologo

!IFDEF DEBUG_BUILD
CPPFLAGS=$(DBGCFLAGS)
PROGRAM=inkbreakerd.exe
!ELSE
CPPFLAGS=$(RELCFLAGS)
PROGRAM=inkbreaker.exe
!ENDIF

BUILD_DIR=build/

WIN32=User32.lib kernel32.lib gdi32.lib gdiplus.lib comdlg32.lib advapi32.lib shell32.lib \
	 ole32.lib uuid.lib Ws2_32.lib comctl32.lib

INCLUDE_DIR=/Ivendor\fltk /Ivendor\fltk\build\lib\Debug /Ivendor\fltk\build /Ivendor
FLTK_SRC_DIR=vendor\fltk\src vendor\sqlite vendor\stb
FLTK=vendor\fltk\build\lib\Debug\fltkd.lib

DEBUGGER=devenv

LIBS=$(WIN32) $(FLTK)

all: $(PROGRAM)

$(PROGRAM): $(SRC)
	echo Windows $(BUILD_TYPE) target
	if not exist $(BUILD_DIR) mkdir build
	$(CC) $(CPPFLAGS) $(SRC) $(WIN32) $(FLTK) $(INCLUDE_DIR) /Fo$(BUILD_DIR) /Fe$(BUILD_DIR)$* \
		/link /PDB:$(BUILD_DIR)$*.pdb

run: $(PROGRAM)
	.\build\$(_PROGRAM)

debugger: $(PROGRAM)
	$(DEBUGGER) /nosplash .\build\$(PROGRAM)

clean:
	del build /s /q

test:
	echo $(LIBS)

tags: $(SRC)
	del /q tags
	ctags -R --kinds-C=+l --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ --sort=true $(SRC)

