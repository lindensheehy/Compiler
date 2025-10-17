#pragma once
#include <cstdio>
#include <cstdint>

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
    NOT_EQUAL,    // !=
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
struct Token {//TBD move to header file
    TokenType token;
    const uint8_t* start; // Pointer back to the original source buffer
    size_t length;
};


struct TokenBuffer {
    Token* data;
    size_t size;
    size_t capacity;
};

TokenBuffer lexer(uint8_t* buf, size_t filesize);