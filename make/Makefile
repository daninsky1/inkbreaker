CXX=g++

# Debug or Release
BUILD_TYPE=Debug
# IO_SQLITE=

PROGRAM_NAME=inkbreaker
BUILD_DIR=build

SRC=$(addprefix src/, \
	main.cpp \
	main_window.cpp \
	view2d.cpp tree.cpp \
	objects/shape.cpp \
	objects/polygon.cpp \
	objects/bezier.cpp \
	edit_tools/edit_tool.cpp \
	edit_tools/polygon_tool.cpp \
	edit_tools/bezier_tool.cpp \
	io/io.cpp \
	objects/shapes.cpp)
OBJ=$(addprefix $(BUILD_DIR)/, \
	main.o \
	main_window.o \
	shapes.o view2d.o)

DISABLED_WARNINGS=-Wno-unused-variable -Wno-unused-function -Wno-unused-value \
-Wno-unused-but-set-variable
DEFINES=/D
FLTK_DIR=vendor/fltk
FLTK_BUILD_DIR=$(FLTK_DIR)/build
FLTK_CONFIG=./$(FLTK_BUILD_DIR)/fltk-config
# https://www.gnu.org/software/make/manual/html_node/Flavors.html#Flavors
# Make will expands 'var=' recursively
ifneq ("$(wildcard $(FLTK_CONFIG))","")
	CXXFLAGS+=$(shell $(FLTK_CONFIG) --cxxflags)
	LDFLAGS+=$(shell $(FLTK_CONFIG) --ldstaticflags)
endif
# WARNING: fltk lib path relie on the order of --ldstaticflags this may break
# if the script change the order of things
FLTK=libfltk.a

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
	$(CXX) -o $(PROGRAM) $(SRC) $(SQLITE) $(CXXFLAGS) $(DISABLED_WARNINGS) $(LDFLAGS) $(INCLUDE_DIRS) $(DEFINES)

.PHONY: fltk
fltk: $(FLTK) $(FLTK_CONFIG) $(FLTK_BUILD_DIR)/Makefile
$(FLTK):
	@echo FLTK Unix $(BUILD_TYPE) build
	if [ ! -e "$(FLTK_BUILD_DIR)/Makefile" ]; then cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -S$(FLTK_DIR) -B$(FLTK_BUILD_DIR); fi
	$(MAKE_COMMAND) -C $(FLTK_BUILD_DIR) -f Makefile -j 4
	
.PHONY: configurefltk
configurefltk: $(FLTK_CONFIG)
$(FLTK_CONFIG):
	mkdir -p $(FLTK_BUILD_DIR)
	echo $(FLTK_BUILD_DIR)
	cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -S$(FLTK_DIR) -B$(FLTK_BUILD_DIR)

.PHONY: run
run: $(PROGRAM)
	./$(PROGRAM)

.PHONY: debugger
debugger: $(PROGRAM)
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
	$(RM) -r $(BUILD_DIR)/*

.PHONY: test
test: workbench
workbench: src/workbench.cpp src/tree.cpp src/coordinates.cpp src/objects/polygon.cpp src/objects/shape.h
	$(CXX) -o $(BUILD_DIR)/$@ $^ $(INCLUDE_DIRS) $(DEFINES)

.PHONY: maketest
maketest:
	echo $(FLTK)
