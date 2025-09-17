#include "core/fileio.h"
#include <cstdio>
#include <filesystem>
#include <system_error>

uint8_t* readFile(const char* fileName) {

    size_t file_length;
    bool file_length_found = getFileLength(fileName, &file_length);
    if (file_length_found) return nullptr;
    if (file_length == 0) return nullptr;

    std::FILE* file = std::fopen(fileName, "rb");
    if (!file) return nullptr;

    uint8_t* buf = new uint8_t[file_length];
    size_t n = std::fread(buf, file_length, 1, file);
    std::fclose(file);

    if (n == 0) {
        delete[] buf;
        return nullptr;
    }

    return buf;   

}

bool getFileLength(const char* fileName, std::uintmax_t * size) {

    std::error_code ec;
    std::uintmax_t temp = std::filesystem::file_size(fileName, ec);
    if (ec) return true;
    
    *size = temp;
    return false;

}

void writeFile(const char* fileName, uint8_t* writeBuffer, size_t writeBufferLength) {
    // std::ofstream out(fileName, std::ios::binary | std::ios::app);
    // if (!out) return;
    // out.write(writeBuffer, static_cast<std::streamsize>(writeBufferLength));
    return;
}

