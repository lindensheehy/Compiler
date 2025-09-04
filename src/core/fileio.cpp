#include "core/fileio.h"
#include <cstdio>
#include <filesystem>
#include <system_error>


#define ELEMENT_SIZE 1
char * readFile(const char* fileName)
{
    size_t file_length;
    bool file_length_found = getFileLength(fileName, &file_length);
    if(file_length_found)
    {
        return nullptr;
    }
    if (file_length == 0) return nullptr;

    std::FILE* file = std::fopen(fileName, "rb");
    if (!file) return nullptr;
    char* buf = new char[file_length];
    size_t off = 0;
    while (off < file_length) 
    {
        size_t n = std::fread(buf + off, ELEMENT_SIZE, file_length - off, file);
        if (n == 0) 
        {
            delete[] buf;
            std::fclose(file);
            return nullptr;
        }
        off += n;
    }
    std::fclose(file);
    return buf;   
}

bool getFileLength(const char* fileName, std::uintmax_t * size) 
{
    std::error_code ec;
    *size = std::filesystem::file_size(fileName, ec);
    if(ec){
        return false;
    }else{
        return true;
    }
}

void writeFile(const char* fileName, const char* writeBuffer, size_t writeBufferLength) {
    // std::ofstream out(fileName, std::ios::binary | std::ios::app);
    // if (!out) return;
    // out.write(writeBuffer, static_cast<std::streamsize>(writeBufferLength));
}
