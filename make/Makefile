CC=g++

# Define BUILD_TYPE, Debug or Release
BUILD_TYPE=Debug
# IO_SQLITE=

SRC=src/main.cpp src/main_window.cpp src/shapes.cpp src/view2d.cpp
OBJ=src/main.o src/main_window.o src/shapes.o src/view2d.o

BUILD_DIR=build/

WIN32=User32.lib kernel32.lib gdi32.lib gdiplus.lib comdlg32.lib advapi32.lib shell32.lib \
	 ole32.lib uuid.lib Ws2_32.lib comctl32.lib

FLTK_DIR=vendor/fltk
FLTK_BUILD_DIR=$(FLTK_DIR)/build

INCLUDE_DIRS=/vendor/fltk -Ivendor/fltk/build -Ivendor
LIBS=$(WIN32) $(FLTK)
# LIBS_SRC_DIRS=vendor\fltk\src vendor\sqlite

DEBUGGER=gdb

ifdef IO_SQLITE
DEFINES=/DIO_SQLITE
SQLITE=vendor/sqlite/sqlite3.c
else
DEFINES=
SQLITE=
endif

ifeq ($(BUILD_TYPE), Debug)
CPPFLAGS=-fexceptions -ggdb -O0 -Wall
PROGRAM=inkbreakerd
FLTK=$(FLTK_BUILD_DIR)/lib/libfltkd.a
else ifeq ($(BUILD_TYPE), Release)
CPPFLAGS=-O2
PROGRAM=inkbreaker
FLTK=$(FLTK_BUILD_DIR)/lib/libfltk.a
else
$(error Select a valid build type for BUILD_TYPE variable: Debug or Release)
endif

all: $(PROGRAM)

$(PROGRAM): $(SRC) $(FLTK)
	echo Unix $(BUILD_TYPE) build
	if [ ! -d "$(BUILD_DIR)" ]; then mkdir build; fi
	$(CC) $(CPPFLAGS) $(SRC) $(SQLITE) $(WIN32) $(FLTK) $(INCLUDE_DIRS) $(DEFINES) -o$(BUILD_DIR)/$(PROGRAM)

fltk: $(FLTK)
$(FLTK):
	echo FLTK $(BUILD_TYPE) build
	if [ ! -d "$(FLTK_BUILD_DIR)" ]; then mkdir $(FLTK_BUILD_DIR); fi
	if [ ! -e "$(FLTK_BUILD_DIR)/Makefile" ]; then cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -S$(FLTK_DIR) -B$(FLTK_BUILD_DIR); fi
# WARNING: -C change the make directory so make will call Makefire relative to
# the FLTK_BUILD_PATH
	make -C $(FLTK_BUILD_DIR) -f Makefile

run: $(PROGRAM)
	./build/$(PROGRAM)

debugger: $(PROGRAM)
	$(DEBUGGER) ./build/$(PROGRAM)

clean:
	rm -r -f build/*
	rm -r -f $(FLTK_BUILD_DIR)/*

tags: $(SRC)
	del /q tags
	ctags -R --kinds-C=+l --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ --sort=true $(SRC)


test:
	echo $(SRC)
