#include <cstdio>
#include <cstdlib>
#include <string_view>
#include <vector>
// --- From Lexer (Should be in a shared header) ---
//TBD! Re-do the code, this is PLACEHOLDER code.
enum class TokenType {
    // Punctuation
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, SEMICOLON, NEW_LINE,
    // Operators
    EQUAL, PLUS, MINUS, STAR, SLASH, EQUAL_EQUAL, NOT_EQUAL, PLUS_EQUAL,
    // Literals
    IDENTIFIER, STRING, NUMBER,
    // Keywords
    INT, RETURN, IF, ELSE, WHILE, FOR, PRINT,
    // Special
    END_OF_FILE,
};

struct Token {
    TokenType type;
    const uint8_t* start;
    size_t length;
};

struct TokenBuffer {
    Token* data;
    size_t size;
    size_t capacity;
};

// --- Abstract Syntax Tree (AST) Node Definitions ---
// (These would typically go in parser.h)

enum class AstNodeType {
    PROGRAM,
    DECLARATION,
    ASSIGNMENT,
    RETURN_STATEMENT,
    BINARY_OPERATION,
    NUMBER_LITERAL,
    VARIABLE_REFERENCE
};

// Base struct for all AST nodes
struct AstNode {
    AstNodeType type;
};

// Represents a list of statements
struct ProgramNode {
    AstNode base;
    AstNode** statements; // Dynamic array of statement nodes
    size_t statement_count;
};

// Represents a binary operation like `a + b`
struct BinaryOpNode {
    AstNode base;
    AstNode* left;
    AstNode* right;
    TokenType op; // PLUS, MINUS, STAR, SLASH
};

// Represents a literal number
struct NumberLiteralNode {
    AstNode base;
    // In a real compiler, you'd store the actual value, not the text.
    // For simplicity, we'll keep the token.
    Token number_token;
};

// Represents a variable being used in an expression
struct VariableReferenceNode {
    AstNode base;
    Token identifier_token;
};

// Represents a variable declaration: `int x = 5;`
struct DeclarationNode {
    AstNode base;
    Token identifier_token;
    AstNode* initializer; // Can be NULL if no initializer
};

// Represents an assignment: `x = 10;`
struct AssignmentNode {
    AstNode base;
    Token identifier_token;
    AstNode* expression;
};

// Represents a return statement: `return 0;`
struct ReturnNode {
    AstNode base;
    AstNode* expression;
};


// --- The Parser ---

struct Parser {
    TokenBuffer* tokens;
    size_t current;
};

// --- Parser Helper Functions ---

Token current_token(Parser* p) {
    return p->tokens->data[p->current];
}

bool is_at_end(Parser* p) {
    return current_token(p).type == TokenType::END_OF_FILE;
}

Token advance(Parser* p) {
    if (!is_at_end(p)) {
        p->current++;
    }
    return p->tokens->data[p->current - 1];
}

// Consumes the current token if it's of the expected type.
// Returns true on success, false on failure (syntax error).
bool match(Parser* p, TokenType type) {
    if (is_at_end(p)) return false;
    if (current_token(p).type == type) {
        advance(p);
        return true;
    }
    return false;
}

// --- Parsing Functions (Mapping Grammar to Code) ---

// Forward declarations are needed for recursive functions
AstNode* parse_expression(Parser* p);
AstNode* parse_statement(Parser* p);

// factor -> NUMBER | IDENTIFIER | LEFT_PAREN expression RIGHT_PAREN
AstNode* parse_factor(Parser* p) {
    if (match(p, TokenType::NUMBER)) {
        NumberLiteralNode* node = (NumberLiteralNode*)malloc(sizeof(NumberLiteralNode));
        node->base.type = AstNodeType::NUMBER_LITERAL;
        node->number_token = p->tokens->data[p->current - 1];
        return (AstNode*)node;
    }

    if (match(p, TokenType::IDENTIFIER)) {
        VariableReferenceNode* node = (VariableReferenceNode*)malloc(sizeof(VariableReferenceNode));
        node->base.type = AstNodeType::VARIABLE_REFERENCE;
        node->identifier_token = p->tokens->data[p->current - 1];
        return (AstNode*)node;
    }

    if (match(p, TokenType::LEFT_PAREN)) {
        AstNode* expr = parse_expression(p);
        if (!match(p, TokenType::RIGHT_PAREN)) {
            // Syntax Error: Expected ')' after expression.
            printf("Syntax Error: Expected ')' after expression.\n");
            return NULL;
        }
        return expr;
    }
    
    // Syntax Error: Expected an expression.
    printf("Syntax Error: Expected an expression factor (number, identifier, or parenthesis).\n");
    return NULL;
}

// term -> factor { (STAR | SLASH) factor }*
AstNode* parse_term(Parser* p) {
    AstNode* left = parse_factor(p);

    while (current_token(p).type == TokenType::STAR || current_token(p).type == TokenType::SLASH) {
        Token op_token = advance(p);
        AstNode* right = parse_factor(p);

        BinaryOpNode* node = (BinaryOpNode*)malloc(sizeof(BinaryOpNode));
        node->base.type = AstNodeType::BINARY_OPERATION;
        node->left = left;
        node->right = right;
        node->op = op_token.type;
        left = (AstNode*)node;
    }

    return left;
}

// expression -> term { (PLUS | MINUS) term }*
AstNode* parse_expression(Parser* p) {
    AstNode* left = parse_term(p);

    while (current_token(p).type == TokenType::PLUS || current_token(p).type == TokenType::MINUS) {
        Token op_token = advance(p);
        AstNode* right = parse_term(p);

        BinaryOpNode* node = (BinaryOpNode*)malloc(sizeof(BinaryOpNode));
        node->base.type = AstNodeType::BINARY_OPERATION;
        node->left = left;
        node->right = right;
        node->op = op_token.type;
        left = (AstNode*)node;
    }

    return left;
}

// declaration -> INT IDENTIFIER { EQUAL expression }? SEMICOLON
AstNode* parse_declaration(Parser* p) {
    if (!match(p, TokenType::IDENTIFIER)) {
        printf("Syntax Error: Expected identifier after 'int'.\n");
        return NULL;
    }
    Token identifier = p->tokens->data[p->current - 1];

    AstNode* initializer = NULL;
    if (match(p, TokenType::EQUAL)) {
        initializer = parse_expression(p);
    }

    if (!match(p, TokenType::SEMICOLON)) {
        printf("Syntax Error: Expected ';' after variable declaration.\n");
        return NULL;
    }

    DeclarationNode* node = (DeclarationNode*)malloc(sizeof(DeclarationNode));
    node->base.type = AstNodeType::DECLARATION;
    node->identifier_token = identifier;
    node->initializer = initializer;
    return (AstNode*)node;
}

// assignment -> IDENTIFIER EQUAL expression SEMICOLON
AstNode* parse_assignment(Parser* p) {
    Token identifier = p->tokens->data[p->current - 1]; // Already consumed by parse_statement

    if (!match(p, TokenType::EQUAL)) {
        printf("Syntax Error: Expected '=' in assignment statement.\n");
        return NULL;
    }

    AstNode* expr = parse_expression(p);

    if (!match(p, TokenType::SEMICOLON)) {
        printf("Syntax Error: Expected ';' after assignment.\n");
        return NULL;
    }

    AssignmentNode* node = (AssignmentNode*)malloc(sizeof(AssignmentNode));
    node->base.type = AstNodeType::ASSIGNMENT;
    node->identifier_token = identifier;
    node->expression = expr;
    return (AstNode*)node;
}

// statement -> ...
AstNode* parse_statement(Parser* p) {
    if (match(p, TokenType::INT)) {
        return parse_declaration(p);
    }
    if (match(p, TokenType::RETURN)) {
        ReturnNode* node = (ReturnNode*)malloc(sizeof(ReturnNode));
        node->base.type = AstNodeType::RETURN_STATEMENT;
        node->expression = parse_expression(p);
        if (!match(p, TokenType::SEMICOLON)) {
            printf("Syntax Error: Expected ';' after return value.\n");
            return NULL;
        }
        return (AstNode*)node;
    }
    if (match(p, TokenType::IDENTIFIER)) {
        // This could be an assignment.
        return parse_assignment(p);
    }

    printf("Syntax Error: Expected a statement.\n");
    return NULL;
}


// --- Main Entry Point ---

// program -> { statement }*
AstNode* parse(TokenBuffer* tokens) {
    Parser parser = { tokens, 0 };

    // For simplicity, we'll use a std::vector to temporarily hold statements
    // before creating the final ProgramNode. A C-style dynamic array would also work.
    std::vector<AstNode*> statements;

    while (!is_at_end(&parser)) {
        statements.push_back(parse_statement(&parser));
    }

    ProgramNode* program = (ProgramNode*)malloc(sizeof(ProgramNode));
    program->base.type = AstNodeType::PROGRAM;
    program->statement_count = statements.size();
    program->statements = (AstNode**)malloc(statements.size() * sizeof(AstNode*));
    for (size_t i = 0; i < statements.size(); ++i) {
        program->statements[i] = statements[i];
    }

    return (AstNode*)program;
}

// TBD: Create a `free_ast(AstNode* root)` function
// to recursively walk the tree and free all the allocated nodes.
