#pragma once

#include <windows.h>

#include <cstdint>
#include <cstddef>


enum class NullTerminate : uint8_t {
    NO = 0,
    YES = 1,
};

enum class WriteFormat : uint8_t {
    TEXT,
    DECIMAL,
};

class File {

    /*
        A class-ified version of fileio.h
        It does all the same things, but it does repeat OS calls unecessarily
        Use this when you want to perform many reads/writes with less overhead
    */

    public:

        /*   Instance Variables   */

        // Constructor
        File(const char* path);

        // Destructor
        ~File();

        /*   Instance Functions   */

        // Returns a new array containing the contents of the file. Ownership is passed to the caller. Returns nullptr on fail.
        uint8_t* read(NullTerminate nullTerminate = NullTerminate::YES);

        // Truncates the file to size 0. Ie, clears the contents of the file.
        void clear();
        
        // Returns the size of the file in bytes
        DWORD getSize();

        // Appends 'writeBufferLength' bytes from 'writeBuffer' into the file.
        void write(const uint8_t* bufferIn, size_t length);

        // Appends 'message' to the file with an optional newline terminator (overloads for various types)
        void write(const char* message, bool newLine = false);
        void write(char message, bool newLine = false, WriteFormat writeFormat = WriteFormat::TEXT);
        void write(short message, bool newLine = false);
        void write(int message, bool newLine = false);
        void write(long long message, bool newLine = false);
        void write(float message, bool newLine = false);
        void write(double message, bool newLine = false);

        // New line only
        void writeNewLine();

    private:

        // The file descriptor (owned by this class)
        HANDLE hFile = nullptr;

        // The size of the file in bytes
        DWORD size;

        // Intermediate buffer for storing the results from ToString casts
        char* stringBuffer;
        static constexpr size_t STRING_BUFFER_SIZE = 64;

};
