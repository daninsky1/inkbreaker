CXX=g++

# Debug or Release
BUILD_TYPE=Debug
# IO_SQLITE=

SRC=src/main.cpp src/main_window.cpp src/shapes.cpp src/view2d.cpp
OBJ=src/main.o src/main_window.o src/shapes.o src/view2d.o

PROGRAM_NAME=inkbreaker

BUILD_DIR=build

DEFINES=/D
FLTK_DIR=vendor/fltk
FLTK_BUILD_DIR=$(FLTK_DIR)/build
FLTK_CONFIG=./$(FLTK_BUILD_DIR)/fltk-config
# https://www.gnu.org/software/make/manual/html_node/Variables-in-Recipes.html
# Make will expands this var after build fltk
CXXFLAGS+=$(shell $(FLTK_CONFIG) --cxxflags)
LDFLAGS+=$(shell $(FLTK_CONFIG) --ldstaticflags)
# WARNING: fltk lib path relie on the order of --ldstaticflags this may break,
# if the script change the order of things
FLTK=$(firstword $(LDFLAGS))

INCLUDE_DIRS=-Ivendor/fltk -Ivendor/fltk/build -Ivendor
# LIBS_SRC_DIRS=vendor\fltk\src vendor\sqlite

DEBUGGER=gdb
DEBUGGER_FLAGS=-silent

ifdef IO_SQLITE
DEFINES+= /DIO_SQLITE
SQLITE=vendor/sqlite/sqlite3.c
else
DEFINES=
SQLITE=
endif

ifeq ($(BUILD_TYPE), Debug)
CXXFLAGS=-fexceptions -ggdb -O0 -Wall
PROGRAM=$(BUILD_DIR)/$(PROGRAM_NAME)debug
else ifeq ($(BUILD_TYPE), Release)
CXXFLAGS=-O2
PROGRAM=$(BUILD_DIR)/$(PROGRAM_NAME)
else
$(error Select a valid build type for BUILD_TYPE variable: Debug or Release)
endif

.PHONY: all
all: $(PROGRAM)

.PHONY: $(PROGRAM_NAME)
$(PROGRAM_NAME): $(PROGRAM)

$(PROGRAM): $(SRC) $(FLTK)
	@echo $(PROGRAM) Unix $(BUILD_TYPE) build
	mkdir -p $(BUILD_DIR)
	$(CXX) -o $(PROGRAM) $(SRC) $(SQLITE) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_DIRS) $(DEFINES)

.PHONY: fltk
fltk: $(FLTK)
$(FLTK):
	@echo FLTK Unix $(BUILD_TYPE) build
	mkdir -p $(FLTK_BUILD_DIR)
	if [ ! -e "$(FLTK_BUILD_DIR)/Makefile" ]; then cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -S$(FLTK_DIR) -B$(FLTK_BUILD_DIR); fi
	make -C $(FLTK_BUILD_DIR) -f Makefile	

.PHONY: run
run: $(PROGRAM)
	./$(PROGRAM)

.PHONY: debugger
debugger: $(PROGRAM_)
	$(DEBUGGER) ./$(PROGRAM) $(DEBUGGER_FLAGS)

.PHONY: tags
tags: $(SRC)
	del /q tags
	ctags -R --kinds-C=+l --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ --sort=true $(SRC)

.PHONY: cleanall
cleanall: clean
	$(RM) -r $(FLTK_BUILD_DIR)/*
	
.PHONY: clean
clean:
	$(RM) -r build/*

.PHONY: test
test:
	@echo $(PROGRAM)
