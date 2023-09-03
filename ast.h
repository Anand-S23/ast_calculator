#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"

typedef enum Token_Type
{
    TOKEN_TYPE_number,
    TOKEN_TYPE_lparen,
    TOKEN_TYPE_rparen,
    TOKEN_TYPE_plus,
    TOKEN_TYPE_minus,
    TOKEN_TYPE_asterisk,
    TOKEN_TYPE_slash,
    TOKEN_TYPE_ast
} Token_Type;

typedef struct Token
{
    Token_Type type;
    union
    {
        struct AST *node;
        int val; // Only for number
    };
} Token;


typedef enum AST_TYPE
{
    AST_TYPE_main, 
    AST_TYPE_negative,
    AST_TYPE_number,
    AST_TYPE_add,
    AST_TYPE_subtract,
    AST_TYPE_multiply,
    AST_TYPE_divide
} AST_Type;

typedef struct AST
{
    AST_Type tag;
    union
    {
        struct AST *body;

        struct
        {
            struct AST *left;
            struct AST *right;
        };

        int val;
    };
} AST;

// Function Decl // 

AST *ast_new(AST ast);
void ast_free(AST *root);
void ast_print(AST *root);
AST *ast_generate_from_expr(char *expr);

#endif // AST_H
