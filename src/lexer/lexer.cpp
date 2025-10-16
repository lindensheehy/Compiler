#include "core/fileio.h"
#include "lexer/lexer.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>


// --- Functions to manage the TokenBuffer ---

// Initializes the buffer with an initial capacity
void init_buffer(TokenBuffer* buffer, size_t initial_capacity) {
    buffer->data = (Token*)malloc(initial_capacity * sizeof(Token));
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


//TBD: think of better way to handle these maps
std::unordered_map<std::string_view, TokenType> keywords = {
    {"int", TokenType::INT},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"print", TokenType::PRINT}
};

std::unordered_map<std::string_view, TokenType> operators = {
    {"=", TokenType::EQUAL},
    {"+", TokenType::PLUS},
    {"==", TokenType::EQUAL_EQUAL},
    {"!=", TokenType::NOT_EQUAL},
    {"+=", TokenType::PLUS_EQUAL}
};

void push_token(TokenBuffer* buffer, Token t,  size_t filesize) {
    // Check if we need to grow the buffer
    if (buffer->size >= buffer->capacity) {
        size_t new_capacity;
        float ratio = static_cast<float>(filesize)/static_cast<float>(buffer->size);
        if(ratio < 2 && ratio > 1.1){//TBD see if really it makes since moving by small increments does not make sense
            new_capacity = static_cast<size_t>((buffer->size) + (buffer->size) * ratio); 
        }else{
            new_capacity = (buffer->size) * 2; 
        }

        //size is too much smaller - then just double
        // realloc is a C function that tries to resize a memory block.
        // It might move the block, so we must update our pointer.
        Token* new_data = (Token*)realloc(buffer->data, new_capacity * sizeof(Token));

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
    return;

}

void handle_multichar_token(TokenBuffer* tokenBuffer, uint8_t* start_of_the_token, size_t length, size_t filesize) {

    if (!start_of_the_token) return;

    //all switch cases failed or they stopped the token
    //TBD add logic to push multichar token
    std::string_view lexeme(reinterpret_cast<const char*>(start_of_the_token), length);
    
    // 1. Check if the lexeme is a keyword
    auto it = keywords.find(lexeme);
    if (it != keywords.end()) { 
        push_token(tokenBuffer, Token{it->second, start_of_the_token, length}, filesize);
        return;
    }

    // 2. Check if the lexeme is an operator
    auto op_it = operators.find(lexeme);
    if (op_it != operators.end()) {
        push_token(tokenBuffer, Token{op_it->second, start_of_the_token, length}, filesize);
        return;
    }

    // 3. If it's neither a keyword nor an operator, it's a literal.
    // This is a simplified approach and TBD: will need more logic to distinguish numbers from identifiers and strings.
    // For now, it assumes any remaining multi-character token is an identifier.
    // The main lexer loop should handle numbers and strings explicitly by checking the first character.
    push_token(tokenBuffer, Token{TokenType::IDENTIFIER, start_of_the_token, length}, filesize); //start of the token ptr because thats where token starts

    return;

}

TokenBuffer lexer(uint8_t* buf, size_t filesize) {

    TokenBuffer tokenBuffer;
    init_buffer(&tokenBuffer, filesize/4); // TBD: make a nice nice estimation like 3 chars per token or something

    if (!buf || filesize == 0) {
        return tokenBuffer;
    }
    
    size_t start_of_the_token = -1;
    uint8_t current_char;
    size_t length_of_token = 0;
    TokenType newTokenType;
    bool tokenBreak;

    for (size_t i = 0; i < filesize; i++) {
        current_char = buf[i];

        if (start_of_the_token == -1) {
            length_of_token = 1;
        }
        
        tokenBreak = true;
        switch (current_char) {

            case ' ':
                break;

            case '(':
                newTokenType = TokenType::LEFT_PAREN;
                break;

            case ')':
                newTokenType = TokenType::RIGHT_PAREN;
                break;

            case '}':
                newTokenType = TokenType::RIGHT_BRACE;
                break;

            case '{':
                newTokenType = TokenType::LEFT_BRACE;
                break;

            case ';':
                newTokenType = TokenType::SEMICOLON;
                break;

            case ',':
                newTokenType = TokenType::COMMA;
                break;

            case '*':
                newTokenType = TokenType::STAR;
                break;

            case '/':
                newTokenType = TokenType::SLASH;
                break;

            case '-':
                newTokenType = TokenType::MINUS;
                break;

            case '\n': //TBD change to \r\n since we read with std::fread
                newTokenType = TokenType::NEW_LINE;
                break;

            default: {
                
                tokenBreak = false;
                
                if (start_of_the_token == -1) {
                    start_of_the_token = i;
                    length_of_token = 1;
                }

                else {
                    length_of_token++;
                }
                
                break;

            }

        }

        if (tokenBreak) {

            push_token(&tokenBuffer, Token{newTokenType, &(buf[i]), length_of_token}, filesize);
            handle_multichar_token(&tokenBuffer, &(buf[start_of_the_token]), length_of_token, filesize);

            start_of_the_token = -1;
            length_of_token = 1;

        }

    }
    return tokenBuffer;
    
}