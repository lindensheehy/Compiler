#include "core/File.h"

#include <iostream>

    
File::File(const char* path) : hFile(INVALID_HANDLE_VALUE) {
    
    // Get handle to file
    this->hFile = CreateFile(
        path,                           // File name
        GENERIC_READ | GENERIC_WRITE,   // Desired access: read and write
        FILE_SHARE_READ,                // Share mode: allow other processes to read
        NULL,                           // Security attributes
        CREATE_ALWAYS,                  // Creation disposition: Create new file if none exists, otherwise overwrite existing file with empty file
        FILE_ATTRIBUTE_NORMAL,          // Flags and attributes
        NULL                            // Template file handle
    );

    if (this->hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Warning: File \"" << path << "\" failed to instantiate!" << std::endl;
    }

    return;
    
}

File::~File() {

    if (this->hFile != INVALID_HANDLE_VALUE) CloseHandle(this->hFile);

    return;

}

uint8_t* File::read() {

    if (this->hFile == INVALID_HANDLE_VALUE) return nullptr;

    // stub

    return nullptr;

}

void File::write(const uint8_t* bufferIn, size_t length) {

    if (this->hFile == INVALID_HANDLE_VALUE) return;
    if (bufferIn == nullptr) return;

    DWORD bytesWritten;
    WriteFile(this->hFile, bufferIn, length, &bytesWritten, NULL);

    if (bytesWritten != length) {
        std::cout << "Warning: File::write failed to write all the requested bytes!" << std::endl;
    }

    return;

}

void File::write(const char* message, bool newLine) {

    if (this->hFile == INVALID_HANDLE_VALUE) return;
    if (message == nullptr) return;

    DWORD bytesWritten;
    WriteFile(this->hFile, message, strlen(message), &bytesWritten, NULL);
    
    if (newLine) {
        WriteFile(this->hFile, "\r\n", 2, &bytesWritten, NULL);
    }

    return;

}

void File::clear() {

    if (this->hFile == INVALID_HANDLE_VALUE) return;

    // Move the file pointer to the start of the file
    SetFilePointer(this->hFile, 0, NULL, FILE_BEGIN);

    // Truncates anything past the file pointer, so everything
    SetEndOfFile(this->hFile);

    return;

}
