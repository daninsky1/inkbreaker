CXX=g++

# Define BUILD_TYPE, Debug or Release
BUILD_TYPE=Debug
# IO_SQLITE=

SRC=src/main.cpp src/main_window.cpp src/shapes.cpp src/view2d.cpp
OBJ=src/main.o src/main_window.o src/shapes.o src/view2d.o

DEFINES=/D

BUILD_DIR=build

# This are the libs necessery to compile with fltk, check out fltk-config or
# fltk-config.in LDLIBS
FLTK_LDLIBS=-lm -lX11 -lXext -lpthread -lXinerama -lXfixes -lXcursor -lXft -lXrender -lm -lfontconfig -ldl

FLTK_DIR=vendor/fltk
FLTK_BUILD_DIR=$(FLTK_DIR)/build
FLTK_CONFIG=$(FLTK_BUILD_DIR)/fltk-config
FLTK=$(FLTK_BUILD_DIR)/lib/libfltk.a

INCLUDE_DIRS=-Ivendor/fltk -Ivendor/fltk/build -Ivendor
LIBS=$(WIN32) $(FLTK)
# LIBS_SRC_DIRS=vendor\fltk\src vendor\sqlite

DEBUGGER=gdb

ifdef IO_SQLITE
DEFINES+= /DIO_SQLITE
SQLITE=vendor/sqlite/sqlite3.c
else
DEFINES=
SQLITE=
endif

ifeq ($(BUILD_TYPE), Debug)
CXXFLAGS=-fexceptions -ggdb -O0 -Wall
PROGRAM=inkbreakerd
else ifeq ($(BUILD_TYPE), Release)
CXXFLAGS=-O2
PROGRAM=inkbreaker
else
$(error Select a valid build type for BUILD_TYPE variable: Debug or Release)
endif

CXXFLAGS=$(shell $(FLTKCONFIG) --cxxflags)
FLTK_LDFLAGS=$(shell $(FLTKCONFIG) --ldstaticflags)

all: $(PROGRAM)

$(PROGRAM): $(SRC) $(FLTK)
	echo Unix $(BUILD_TYPE) build
	if [ ! -d "$(BUILD_DIR)" ]; then mkdir build; fi
# $(CXX) $(SRC) $(SQLITE) $(CPPFLAGS) $(UNIX_LIBS) $(FLTK) $(INCLUDE_DIRS) $(DEFINES) -o $(BUILD_DIR)/$(PROGRAM) 		
	$(CXX) $(SRC) $(SQLITE) $(CXXFLAGS) $(FLTK) $(FLTK_LDLIBS) $(INCLUDE_DIRS) $(DEFINES) -o $(BUILD_DIR)/$(PROGRAM) 

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

cleanall: clean
	rm -r -f $(FLTK_BUILD_DIR)/*
	
clean:
	rm -r -f build/*

tags: $(SRC)
	del /q tags
	ctags -R --kinds-C=+l --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ --sort=true $(SRC)


test:
	echo $(SRC)
