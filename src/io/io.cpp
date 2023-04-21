#include "io.h"

/* Warning, this can erase previous saved file */
void write_file(const char *filepath)
{
}

std::ifstream read_file(const char *filepath)
{
    std::ifstream ifs{filepath};
    if (!ifs) std::cerr << "Cannot open file.\n";
    return ifs;
}

void load_file(const char *filepath)
{
    std::ifstream ifs = read_file(filepath);
}

/* This prepare data to call write file */
void save_file()
{
    
}
