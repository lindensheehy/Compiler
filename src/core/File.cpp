#include "core/File.h"


#if defined(_WIN32) || defined(_WIN64)
    #define ON_WINDOWS 1
#else
    #define ON_WINDOWS 0
#endif

#if defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
    #define ON_UNIX 1
#else
    #define ON_UNIX 0
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

    
File::File(const char* path) : f(-1), size(0) {

    #if ON_WINDOWS

        this->f = _open(path, _O_RDWR | _O_BINARY | _O_CREAT, 0666);
        
        if (this->f == -1) {
            std::cout << "Warning: File \"" << path << "\" failed to instantiate!" << std::endl;
            return;
        }

        struct _stat st;
        if (_fstat(this->f, &st)) {
            this->size = 0;
            return;
        }
        
        size = st.st_size;

    #endif

    #if ON_UNIX

        this->f = open(path, O_RDWR | O_CREAT, 0666);

        if (this->f == -1) {
            std::cout << "Warning: File \"" << path << "\" failed to instantiate!" << std::endl;
            return;
        }

        struct stat st;
        if (fstat(this->f, &st)) {
            this->size = 0;
            return;
        }
        
        size = st.st_size;

    #endif

    return;
    
}

File::~File() {

    if (this->f != -1) {

        #if ON_WINDOWS
            _close(this->f);
        #endif
        
        #if ON_UNIX
            close(this->f);
        #endif

    }

    return;
}

uint8_t* File::read() {

    if (this->f == -1) return nullptr;

    uint8_t* buf = new uint8_t[size];
    if (!buf) return nullptr;

    #if ON_WINDOWS
        _lseek(this->f, 0, SEEK_SET);
    #endif

    #if ON_UNIX
        lseek(this->f, 0, SEEK_SET);
    #endif

    size_t totalRead = 0;
    while (totalRead < size) {

        #if ON_WINDOWS
            int n = _read(this->f, buf + totalRead, (unsigned int)(size - totalRead));
        #endif

        #if ON_UNIX
            ssize_t n = ::read(this->f, buf + totalRead, size - totalRead);
        #endif

        if (n <= 0) {
            delete[] buf;
            return nullptr;
        }
        totalRead += n;

    }

    return buf;

}

void File::write(const uint8_t* bufferIn, size_t length) {

    if (this->f == -1 || !bufferIn) return;

    size_t totalWritten = 0;
    while (totalWritten < length) {

        #if ON_WINDOWS
            int n = _write(this->f, bufferIn + totalWritten, (unsigned int)(length - totalWritten));
        #endif

        #if ON_UNIX
            ssize_t n = ::write(this->f, bufferIn + totalWritten, length - totalWritten);
        #endif

        if (n <= 0) {
            std::cout << "Error: File:write failed to write all bytes!" << std::endl;
            return;
        }
        totalWritten += n;

    }

    this->size += length;

    return;

}

void File::truncate() {

    if (this->f == -1) {
        std::cout << "Warning: File::truncate called on invalid File instance!" << std::endl;
        return;
    }

    #if ON_WINDOWS
        _chsize_s(this->f, 0);
    #endif

    #if ON_UNIX
        ftruncate(this->f, 0);
    #endif

    this->size = 0;

    return;

}

uintmax_t File::getSize() {
    return this->size;
}
