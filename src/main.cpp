#include <cstdio>
#include <iostream>
#include <string.h>

#include "disassembler/disassembler.h"
#include "assembler/assembler.h"
#include "core/File.h"

void testing() {

    char input;
    while (true) {

        std::cout << "Select a test to run:" << std::endl;
        std::cout << "1 - Dissassembler" << std::endl;
        std::cout << "2 - Assembler" << std::endl;
        std::cout << "3 - File utility" << std::endl;
        std::cout << "4 - Exit" << std::endl;
        std::cin >> input;

        switch (input) {

            case '1': {

                const char* in = "intermediate/test.irbin";
                const char* out = "out/test.asm";

                Disassembler::ErrorCode errorCode = Disassembler::generateDisassemble(in, out);
                if (static_cast<uint32_t>(errorCode)) {
                    std::cout << "Dissassemble failed!" << std::endl;
                    std::cout << "Error Code: " << static_cast<uint32_t>(errorCode) << std::endl;
                }

                else {
                    std::cout << "Disassembled " << in << " -> " << out << std::endl;
                }

                break;

            }

            case '2': {

                const char* in = "intermediate/test.irbin";
                const char* out = "out/test.asm";

                Assembler::ErrorCode errorCode = Assembler::generateAssemble(in, out);
                if (static_cast<uint32_t>(errorCode)) {
                    std::cout << "Assemble failed!" << std::endl;
                    std::cout << "Error Code: " << static_cast<uint32_t>(errorCode) << std::endl;
                }

                else {
                    std::cout << "Assembled " << in << " -> " << out << std::endl;
                }

                break;

            }

            case '3': {

                File f("logs/test.txt");
                uint8_t* fileContents;

                fileContents = f.read();
                std::cout << "File Contents: " << std::endl;
                std::cout << fileContents << std::endl;
                delete[] fileContents;

                f.clear();
                std::cout << "Truncated file contents" << std::endl;

                const char* s = "test";
                f.write(reinterpret_cast<const uint8_t*>(s), strlen(s));
                std::cout << "Wrote text to the file" << std::endl;

                fileContents = f.read();
                std::cout << "File Contents: " << std::endl;
                std::cout << fileContents << std::endl;
                delete[] fileContents;

                break;

            }

            case '4': {
                return;
            }

            default: {
                std::cout << "That is not a valid choice." << std::endl;
                break;
            }

        }

    }

}

int main(int argc, char** argv) {

    // Remove once proper main functionality is built
    testing();

    return 0;

}
