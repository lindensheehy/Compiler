#pragma once

#include <windows.h>

#include <cstdint>
#include <cstddef>


class File {

    /*
        This class wraps a std::FILE type
    */

    public:

        /*   Instance Variables   */

        // Constructor
        File(const char* path);

        // Destructor
        ~File();

        /*   Instance Functions   */

        // Returns a new array containing the contents of the file. Ownership is passed to the caller. Returns nullptr on fail.
        uint8_t* read();

        // Appends 'writeBufferLength' bytes from 'writeBuffer' into the file.
        void write(const uint8_t* bufferIn, size_t length);

        // Appends 'message' to the file with an optional newline terminator
        void write(const char* message, bool newLine = false);

        // Truncates the file to size 0. Ie, clears the contents of the file.
        void clear();

    private:

        // The file descriptor (owned by this class)
        HANDLE hFile = nullptr;

};
