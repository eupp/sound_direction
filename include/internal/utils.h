#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>

template <typename T>
void debug_print(const char* filename, T* array, size_t size)
{
    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return;
    }
    for (size_t i = 0; i < size; i++) {
        file << array[i] << std::endl;
    }
}

#endif // UTILS_H
