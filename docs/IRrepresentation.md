# Compiler Tokens

This document outlines the token types and the token structure used in the lexical analysis phase of the compiler.

---

## Token Types

The following list details the different types of tokens recognized by the compiler's lexer. Each token type represents a distinct category of language element, such as keywords, operators, and literals.

- **Punctuation:** Characters used to structure code.
    - `(` LEFT_PAREN 
    - `)` RIGHT_PAREN
    - `{` LEFT_BRACE
    - `}` RIGHT_BRACE
    - `,` COMMA
    - `;` SEMICOLON

- **Operators:** Symbols that perform actions or comparisons.
    - `=` EQUAL
    - `+` PLUS
    - `-` MINUS
    - `*` STAR
    - `/` SLASH
    - `==` EQUAL_EQUAL
    - `!=` NOT_EQUAL
    - `+=` PLUS_EQUAL

- **Literals:** Fixed values that appear directly in the code.
    - **Identifiers:** Names for variables, functions, etc. (e.g., `main`, `message`). represented by IDENTIFIER
    - **String:** Text enclosed in double quotes (e.g., `"Hello, world!"`). represented by STRING
    - **Number:** Numeric values (e.g., `123`, `3.14`). represented by NUMBER

- **Keywords:** Reserved words with a predefined meaning.
    - `int` INT
    - `return` RETURN
    - `if` IF
    - `else` ELSE
    - `while` WHILE
    - `for` FOR
    - `print` PRINT

- **Special tokens:** special tokens like end_line
 
---

## `Token` Struct

The `Token` struct is the fundamental data structure used to represent a single token after it has been identified by the lexer.

### Members:

- **`type`**: The category of the token (e.g., `KEYWORD`, `IDENTIFIER`).
- **`lexeme`**: The original text from the source code that forms the token (e.g., `"int"`, `"main"`).

### Example of Tokenization: `int main() {`

The lexer would process this line and produce the following sequence of tokens:

1.  `{ type: KEYWORD, lexeme: "int" }`
2.  `{ type: IDENTIFIER, lexeme: "main" }`
3.  `{ type: PUNCTUATION, lexeme: "(" }`
4.  `{ type: PUNCTUATION, lexeme: ")" }`
5.  `{ type: PUNCTUATION, lexeme: "{" }`