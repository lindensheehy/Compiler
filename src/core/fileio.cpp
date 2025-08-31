#include "core/fileio.h"
#include <cstdio>
#include <filesystem>
#include <system_error>
char* readFile(const char* fileName)
{
    size_t file_length = getFileLength(fileName);
        
}

size_t getFileLength(const char* fileName) 
{
    std::error_code ec;
    std::uintmax_t sz = std::filesystem::file_size(fileName, ec);
    if(ec){
        return 0;
    }else{
        return sz;
    }
}

void writeFile(const char* fileName, const char* writeBuffer, size_t writeBufferLength) {
    
}
