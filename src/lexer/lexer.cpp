#include "core/fileio.h"
#include <cstdio>
#include <vector>
#include <string>
#include <unordered_map>


enum class TokenType {
    // Single-character punctuation tokens - all considered to break appart the tokens (also space but it is not a token)
    LEFT_PAREN,   // (
    RIGHT_PAREN,  // )
    LEFT_BRACE,   // {
    RIGHT_BRACE,  // }
    COMMA,        // ,
    SEMICOLON,    // ;
    NEW_LINE,     // new line
    // Operators
    EQUAL,        // = 
    PLUS,         // +
    MINUS,        // -  (may also break apart tokens but later should not since you can do -=) TBD: expand token set
    STAR,         // *  (may also break apart tokens)
    SLASH,        // /  (may also break apart tokens)
    EQUAL_EQUAL,  // ==
    NOT_EQUAL,   // !=
    PLUS_EQUAL,   // +=
    //identifiers should be handled sepparately
    IDENTIFIER,
    // Literals
    STRING,
    NUMBER,
    // Keywords
    INT,
    RETURN,
    IF,
    ELSE,
    WHILE,
    FOR,
    PRINT,

    // Special tokens
    END_OF_FILE,
};

struct token{//TBD move to header file
    TokenType token;
    const char * tokenValue;
    //add more stuff
};

//TBD: think of better way to handle these maps
std::unordered_map<std::string, TokenType> keywords = {
    {"int", TokenType::INT},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"print", TokenType::PRINT}
};

std::unordered_map<std::string, TokenType> operators = {
    {"=", TokenType::EQUAL},
    {"+", TokenType::PLUS},
    {"==", TokenType::EQUAL_EQUAL},
    {"!=", TokenType::NOT_EQUAL},
    {"+=", TokenType::PLUS_EQUAL}
};

// Helper function to convert a character range to a string
const char* range_to_cstr(const uint8_t* start, const uint8_t* end) {
    //1. just in case
    if (start == nullptr || end == nullptr || start >= end)
        return nullptr;


    size_t length = end - start;
    char* cstr = (char*)malloc(length + 1);
    if (cstr == nullptr)
        return nullptr;

    // 2. Copy the data from the uint8_t* buffer
    memcpy(cstr, start, length);

    // 3. Add the null terminator
    cstr[length] = '\0';

    return cstr;
}

void handle_multichar_token(std::vector<token*>* tokenBuffer, uint8_t* start_of_the_token_ptr, uint8_t* current_char_ptr) {
    if (!start_of_the_token_ptr)
        return;

    const char * lexeme = range_to_cstr(start_of_the_token_ptr, current_char_ptr);

    // 1. Check if the lexeme is a keyword
    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        tokenBuffer->push_back(new token{it->second, lexeme});
        return;
    }

    // 2. Check if the lexeme is an operator
    auto op_it = operators.find(lexeme);
    if (op_it != operators.end()) {
        tokenBuffer->push_back(new token{op_it->second, lexeme});
        return;
    }

    // 3. If it's neither a keyword nor an operator, it's a literal.
    // This is a simplified approach and TBD: will need more logic to distinguish numbers from identifiers and strings.
    // For now, it assumes any remaining multi-character token is an identifier.
    // The main lexer loop should handle numbers and strings explicitly by checking the first character.
    tokenBuffer->push_back(new token{TokenType::IDENTIFIER, lexeme});
}


std::vector<token*>* lexer(uint8_t* buf, size_t filesize) {
    if (!buf || filesize == 0) {
        return nullptr;
    }

    std::vector<token*>* tokenBuffer = new std::vector<token*>();
    size_t i = 0; // Use size_t for indexing
    bool tokenNew = true;
    uint8_t * start_of_the_token_ptr = nullptr;
    uint8_t * current_char_ptr = buf + i;
    while (i < filesize) {
        current_char_ptr = buf + i;

        // 1. Skip whitespace
        if (*current_char_ptr == ' ') {
            i++;
            continue;
        }

        // 3. Handle multi-character tokens
        //to handle multi character tokens we shall set a pointer to the first letter, start_of_token = buf + i;
        //then once we find a sepparator which is in switch case - we will set the new token to TRUE, and in case the pointer was nullptr = we shall 
        // collect all the letters from that pointer.


        // 2. Handle single-character tokens which sepparate the tokens
        switch (*current_char_ptr) {
            case ' ':
                i++;
                continue;
            case '(':
                tokenBuffer->push_back(new token{TokenType::LEFT_PAREN, "("});
                i++;
                continue;
            case ')':
                tokenBuffer->push_back(new token{TokenType::RIGHT_PAREN, ")"});
                i++;
                continue;
            case '}':
                tokenBuffer->push_back(new token{TokenType::RIGHT_BRACE, "}"});
                i++;
                continue;
            case '{':
                tokenBuffer->push_back(new token{TokenType::LEFT_BRACE, "{"});
                i++;
                continue;
            case ';':
                tokenBuffer->push_back(new token{TokenType::SEMICOLON, ";"});
                i++;
                continue;
            case ',':
                tokenBuffer->push_back(new token{TokenType::COMMA, ","});
                i++;
                continue;
            case '*':
                tokenBuffer->push_back(new token{TokenType::STAR, "*"});
                i++;
                continue;
            case '/':
                tokenBuffer->push_back(new token{TokenType::SLASH, "/"});
                i++;
                continue;
            case '-':
                tokenBuffer->push_back(new token{TokenType::MINUS, "-"});
                i++;
                continue;
            case '\n':
                tokenBuffer->push_back(new token{TokenType::NEW_LINE, "\n"});
                i++;
                continue;
        }

        //all switch cases failed
        tokenNew = false;
        start_of_the_token_ptr = buf + i;
        //make a helper function to go and deposit all the things into a string and make a token
        //once a sepparator was found - call the function to go and check for the 


        // 4. Handle literals and identifiers
        // Numbers

        // Identifiers and Keywords

        // 5. Handle strings

        // 6. Handle unrecognized characters (errors)
        // If the code reaches here, it's an unrecognized character.
        // You should handle this as a lexical error.
        
        // Advance to the next character to avoid infinite loop
        i++;
    }

    // After the loop, add the EOF token
    tokenBuffer->push_back(new token{TokenType::END_OF_FILE, nullptr});
    return tokenBuffer;
}