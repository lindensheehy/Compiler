Simple Language Grammar
This document provides a minimal grammar - more to be done
How to Read This Grammar
::=: Means "is defined as".

|: Means "or".

{ ... }?: Means the part in braces is optional (zero or one time).

Words in UPPERCASE: These are tokens from the lexer (e.g., IDENTIFIER, SEMICOLON).

Words in lowercase: These are grammar rules.

Grammar Rules:
Top-Level Rule:
A program is simply a sequence of one or more statements.

program ::= { statement }+

Statements
For this simple version, a statement can only be a variable declaration, an assignment, or a return statement.

statement ::= declaration_statement | assignment_statement | return_statement

declaration_statement ::= INT IDENTIFIER { EQUAL expression }? SEMICOLON

This rule defines how to declare an integer variable. It can be just a declaration or a declaration with an initial value.

Example: int x;

Example: int y = 10 + 5;

assignment_statement ::= IDENTIFIER EQUAL expression SEMICOLON

This rule defines how to assign a new value to an existing variable.

Example: x = 20;

return_statement ::= RETURN expression SEMICOLON

This rule defines how to return a value, which is essential for any program to have an exit status.

Example: return 0;

Example: return x * 2;

Expressions:
These rules define what constitutes a valid mathematical expression.

expression ::= term { { PLUS | MINUS } term }*
term       ::= factor { { STAR | SLASH } factor }*
factor     ::= NUMBER | IDENTIFIER | LEFT_PAREN expression RIGHT_PAREN

factor: This is the most basic part of an expression. It's either a raw number, a variable, or a whole new expression wrapped in parentheses.

ex: 

123

my_variable

(5 + 2)

term: This rule handles multiplication and division by grouping factors.

10 * x

expression: This is the top-level rule that handles addition and subtraction by grouping terms.

5 + (10 * x)

# LL(1) Grammar and Parsing Table

Here we transform our grammar into an **LL(1) grammar**, a format that is unambiguous and ready for a predictive, table-driven parser.

The process involves three main steps:
1.  **Rewriting the Grammar:** Eliminate left recursion and perform left factoring.
2.  **Calculating FIRST and FOLLOW Sets:** Determine which tokens can start or follow a grammar rule.
3.  **Constructing the Parsing Table:** Build the final table that will drive the parser.

---

## Step 1: The LL(1)-Ready Grammar

The grammar from `simple_grammar.md` is already very close to being LL(1) ready. The main transformation required is to rewrite the expression rules to be purely right-recursive, which is a standard technique to eliminate the "looping" nature of `{...}*` for this formal process.

#### Original Expression Rules:

expression ::= term { { PLUS | MINUS } term }*
term       ::= factor { { STAR | SLASH } factor }*

#### Transformed LL(1) Grammar:
| #  | Production Rule                                                         |
| -- | ----------------------------------------------------------------------- |
| 1  | **`program → statement program`**                                       |
| 2  | **`program → ε`**                                                       |
| 3  | **`statement → declaration_statement`**                                 |
| 4  | **`statement → assignment_statement`**                                  |
| 5  | **`statement → return_statement`**                                      |
| 6  | **`declaration_statement → INT IDENTIFIER decl_initializer SEMICOLON`** |
| 7  | **`decl_initializer → EQUAL expression`**                               |
| 8  | **`decl_initializer → ε`**                                              |
| 9  | **`assignment_statement → IDENTIFIER EQUAL expression SEMICOLON`**      |
| 10 | **`return_statement → RETURN expression SEMICOLON`**                    |
| 11 | **`expression → term expr_prime`**                                      |
| 12 | **`expr_prime → PLUS term expr_prime`**                                 |
| 13 | **`expr_prime → MINUS term expr_prime`**                                |
| 14 | **`expr_prime → ε`**                                                    |
| 15 | **`term → factor term_prime`**                                          |
| 16 | **`term_prime → STAR factor term_prime`**                               |
| 17 | **`term_prime → SLASH factor term_prime`**                              |
| 18 | **`term_prime → ε`**                                                    |
| 19 | **`factor → NUMBER`**                                                   |
| 20 | **`factor → IDENTIFIER`**                                               |
| 21 | **`factor → LEFT_PAREN expression RIGHT_PAREN`**                        |
#### FIRST Sets:
| Non-Terminal            | FIRST Set                                |
| ----------------------- | ---------------------------------------- |
| `program`               | { `INT`, `IDENTIFIER`, `RETURN`, `ε` }   |
| `statement`             | { `INT`, `IDENTIFIER`, `RETURN` }        |
| `declaration_statement` | { `INT` }                                |
| `decl_initializer`      | { `EQUAL`, `ε` }                         |
| `assignment_statement`  | { `IDENTIFIER` }                         |
| `return_statement`      | { `RETURN` }                             |
| `expression`            | { `NUMBER`, `IDENTIFIER`, `LEFT_PAREN` } |
| `expr_prime`            | { `PLUS`, `MINUS`, `ε` }                 |
| `term`                  | { `NUMBER`, `IDENTIFIER`, `LEFT_PAREN` } |
| `term_prime`            | { `STAR`, `SLASH`, `ε` }                 |
| `factor`                | { `NUMBER`, `IDENTIFIER`, `LEFT_PAREN` } |
#### LL(1) Parsing Table:
| Non-Terminal | INT | IDENTIFIER | RETURN | EQUAL | SEMICOLON | PLUS | MINUS | STAR | SLASH | NUMBER | ( | ) | $ |
| :--- | :-: | :---: | :---: | :-: | :---: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| **program** | 1 | 1 | 1 | | | | | | | | | | 2 |
| **statement** | 3 | 4 | 5 | | | | | | | | | | |
| **declaration** | 6 | | | | | | | | | | | | |
| **decl_init** | | | | 7 | 8 | | | | | | | | |
| **assignment** | | 9 | | | | | | | | | | | |
| **return** | | | 10 | | | | | | | | | | |
| **expression** | | 11 | | | | | | | | 11 | 11 | | |
| **expr_prime** | | | | | 14 | 12 | 13 | | | | | 14 | |
| **term** | | 15 | | | | | | | | 15 | 15 | | |
| **term_prime** | | | | | 18 | 18 | 18 | 16 | 17 | | | 18 | |
| **factor** | | 20 | | | | | | | | 19 | 21 | | |