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
    const uint8_t* start; // Pointer back to the original source buffer
    size_t length;
    //add more stuff
};


struct TokenBuffer {
    token* data;
    size_t size;
    size_t capacity;
};

// --- Functions to manage the TokenBuffer ---

// Initializes the buffer with an initial capacity
void init_buffer(TokenBuffer* buffer, size_t initial_capacity) {
    buffer->data = (token*)malloc(initial_capacity * sizeof(token));
    buffer->size = 0;
    buffer->capacity = initial_capacity;
}

// Frees the memory owned by the buffer
void free_buffer(TokenBuffer* buffer) {
    if (buffer->data) {
        free(buffer->data);
    }
    buffer->data = nullptr;
    buffer->size = 0;
    buffer->capacity = 0;
}

// Adds a token to the buffer, resizing if necessary
void push_token(TokenBuffer* buffer, token t) {
    // Check if we need to grow the buffer
    if (buffer->size >= buffer->capacity) {
        // Double the capacity. The "+ 8" is to handle an initial capacity of 0.
        size_t new_capacity = buffer->capacity * 2 + 8;
        
        // realloc is a C function that tries to resize a memory block.
        // It might move the block, so we must update our pointer.
        token* new_data = (token*)realloc(buffer->data, new_capacity * sizeof(token));

        if (!new_data) {
            // Handle allocation failure
            return;
        }
        
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }

    // Now we have space. Add the token.
    buffer->data[buffer->size] = t;
    buffer->size++;
}

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

void handle_multichar_token(std::vector<token*> tokenBuffer, uint8_t* start_of_the_token_ptr, uint8_t* current_char_ptr) {
    if (!start_of_the_token_ptr)
        return;

    const char * lexeme = range_to_cstr(start_of_the_token_ptr, current_char_ptr);

    // 1. Check if the lexeme is a keyword
    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        tokenBuffer.push_back(new token{it->second, lexeme});
        start_of_the_token_ptr = nullptr;
        return;
    }

    // 2. Check if the lexeme is an operator
    auto op_it = operators.find(lexeme);
    if (op_it != operators.end()) {
        tokenBuffer.push_back(new token{op_it->second, lexeme});
        start_of_the_token_ptr = nullptr;
        return;
    }

    // 3. If it's neither a keyword nor an operator, it's a literal.
    // This is a simplified approach and TBD: will need more logic to distinguish numbers from identifiers and strings.
    // For now, it assumes any remaining multi-character token is an identifier.
    // The main lexer loop should handle numbers and strings explicitly by checking the first character.
    tokenBuffer.push_back(new token{TokenType::IDENTIFIER, lexeme});
    start_of_the_token_ptr = nullptr;
}


TokenBuffer lexer(uint8_t* buf, size_t filesize) {
    TokenBuffer tokenBuffer;
    init_buffer(&tokenBuffer, 64); // Start with a capacity of 64 tokens

    if (!buf || filesize == 0)
        return tokenBuffer;
    


    

    uint8_t* current = buf;
    uint8_t* end = buf + filesize;

    size_t i = 0; // Use size_t for indexing
    uint8_t * start_of_the_token_ptr = nullptr;
    uint8_t * current_char_ptr = buf + i;
    size_t length_of_token;
    //tbd rework! No more vectors in this household! allocate a big chunk of memory char/token type of assumption based on file size
    //assumption is user will give not just some ridiculous amount of nonsense
    //make your own wrapper to store pure DATA of these structs, dont pass around pointers
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

        if(!start_of_the_token_ptr){
            length_of_token = 0;
        }else{
           length_of_token =  (size_t)(current - start_of_the_token_ptr);
        }

        // 2. Handle single-character tokens which sepparate the tokens
        switch (*current_char_ptr) { //change to index plz... TBD
            case ' ':
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case '(':
                tokenBuffer.push_back(new token{TokenType::LEFT_PAREN, "("});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case ')':
                tokenBuffer.push_back(new token{TokenType::RIGHT_PAREN, ")"});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case '}':
                tokenBuffer.push_back(new token{TokenType::RIGHT_BRACE, "}"});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case '{':
                tokenBuffer.push_back(new token{TokenType::LEFT_BRACE, "{"});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            // case ';':
            //     tokenBuffer.push_back(new token{TokenType::SEMICOLON, ";"});
            //     handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
            //     i++;
            //     continue;
            case ';':
                push_token(&tokenBuffer, token{TokenType::SEMICOLON, start_of_the_token_ptr, length_of_token});
                i++;
                continue;
            case ',':
                tokenBuffer.push_back(new token{TokenType::COMMA, ","});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case '*':
                tokenBuffer.push_back(new token{TokenType::STAR, "*"});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case '/':
                tokenBuffer.push_back(new token{TokenType::SLASH, "/"});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case '-':
                tokenBuffer.push_back(new token{TokenType::MINUS, "-"});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
            case '\n':
                tokenBuffer.push_back(new token{TokenType::NEW_LINE, "\n"});
                handle_multichar_token(tokenBuffer, start_of_the_token_ptr, current_char_ptr);
                i++;
                continue;
        }

        //all switch cases failed
        start_of_the_token_ptr = current_char_ptr;
        i++;
    }

    // After the loop, add the EOF token
    tokenBuffer.push_back(new token{TokenType::END_OF_FILE, nullptr});
    return tokenBuffer;
}