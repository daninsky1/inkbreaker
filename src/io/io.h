#include <iostream>
#include <fstream>

#include <FL/filename.H>

struct FileState {
    char filepath[FL_PATH_MAX];
    bool changed;
};

/* Warning, this can erase previous saved file */
void write_file(const char *filepath);

std::ifstream read_file(const char *filepath);

void load_file(const char *filepath);

/* This prepare data to call write file */
void save_file();
