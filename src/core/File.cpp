#include "core/File.h"

#include "core/casting.h"

#include <iostream>


File::File(const char* path) : hFile(INVALID_HANDLE_VALUE), size(INVALID_FILE_SIZE) {
    
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
        std::cout << " >> Could not open the file." << std::endl;
    }

    this->size = GetFileSize(this->hFile, NULL);
    if (this->size == INVALID_FILE_SIZE) {
        std::cout << "Warning: File \"" << path << "\" failed to instantiate!" << std::endl;
        std::cout << " >> Could not get the length of the file." << std::endl;
        CloseHandle(this->hFile);
        return;
    }

    this->stringBuffer = new char[File::STRING_BUFFER_SIZE];

    return;
    
}

File::~File() {

    if (this->hFile != INVALID_HANDLE_VALUE) CloseHandle(this->hFile);

    return;

}

uint8_t* File::read(NullTerminate nullTerminate) {

    if (this->hFile == INVALID_HANDLE_VALUE) return nullptr;

    DWORD bufferSize = this->size;
    if (nullTerminate == NullTerminate::YES) bufferSize++;
    uint8_t* buffer = new uint8_t[bufferSize];
   
    SetFilePointer(this->hFile, 0, NULL, FILE_BEGIN);

    DWORD bytesRead;
    BOOL success = ReadFile(this->hFile, buffer, this->size, &bytesRead, NULL);

    if (!success) {
        std::cout << "Warning: Failed to read file!" << std::endl;
        std::cout << " >> Error Code: " << GetLastError() << std::endl;
        delete[] buffer;
        return nullptr;
    }

    if (bytesRead != this->size) {
        std::cout << "Warning: Failed to read file!" << std::endl;
        std::cout << " >> Tried to read " << this->size << " bytes but only read " << bytesRead << "!" << std::endl;
        delete[] buffer;
        return nullptr;
    }

    if (nullTerminate == NullTerminate::YES) buffer[this->size] = '\0';

    return buffer;

}

void File::clear() {

    if (this->hFile == INVALID_HANDLE_VALUE) return;

    // Move the file pointer to the start of the file
    SetFilePointer(this->hFile, 0, NULL, FILE_BEGIN);

    // Truncates anything past the file pointer, so everything
    SetEndOfFile(this->hFile);

    return;

}

DWORD File::getSize() {
    return this->size;
}

void File::write(const uint8_t* bufferIn, size_t length) {

    if (this->hFile == INVALID_HANDLE_VALUE) return;
    if (bufferIn == nullptr) return;

    DWORD bytesWritten;
    WriteFile(this->hFile, bufferIn, length, &bytesWritten, NULL);

    if (bytesWritten != length) {
        std::cout << "Warning: File::write failed to write all the requested bytes!" << std::endl;
    }

    this->size += length;

    return;

}

void File::write(const char* message, bool newLine) {

    if (this->hFile == INVALID_HANDLE_VALUE) return;
    if (message == nullptr) return;

    DWORD bytesWritten;
    size_t length = strlen(message);
    WriteFile(this->hFile, message, length, &bytesWritten, NULL);
    
    if (newLine) {
        WriteFile(this->hFile, "\r\n", 2, &bytesWritten, NULL);
        this->size += 2;
    }

    this->size += length;

    return;

}

void File::write(char message, bool newLine, WriteFormat writeFormat) {

    switch (writeFormat) {

        case WriteFormat::TEXT: {
            const char messageString[] = {message, '\0'};
            this->write(messageString, newLine);
            break;
        }

        case WriteFormat::DECIMAL: {
            this->write(static_cast<long long>(message), newLine);
            break;
        }

    }

    return;
    
}

void File::write(short message, bool newLine) {
    this->write(static_cast<long long>(message), newLine);
}

void File::write(int message, bool newLine) {
    this->write(static_cast<long long>(message), newLine);
}

void File::write(long long message, bool newLine) {
    intToString(message, this->stringBuffer, File::STRING_BUFFER_SIZE);
    this->write(stringBuffer, newLine);
}

void File::write(float message, bool newLine) {
    floatToString(message, this->stringBuffer, File::STRING_BUFFER_SIZE, 4);
    this->write(this->stringBuffer, newLine);
}

void File::write(double message, bool newLine) {
    doubleToString(message, this->stringBuffer, File::STRING_BUFFER_SIZE, 4);
    this->write(this->stringBuffer, newLine);
}

// New line
void File::writeNewLine() {
    this->write("\n");
}
