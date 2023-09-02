#include "ast.h"
#include "util.h"

/* Peeks at the next char in string */
static inline char next_char(char *string)
{
    return *(string + 1);
}

/* Finds the index of the inner most paren */
static int first_occurance_paren(char *input)
{
    char *pointer = input;

    while (*pointer++)
    {
        if (*pointer == '(')
        {
            char *inner_most = pointer;
            while (*pointer != ')')
            {
                if (*pointer++ == '(')
                {
                    inner_most = pointer;
                }
            }

            return (int)(inner_most - input);
        }
    }

    return -1;
}


/* Creates a buffer with the tokenized expr */
static Token *tokenize_expr(char *expr)
{
    Token *token_buffer;

    while (*expr)
    {
        Token current_token = {0};

        switch (*expr)
        {
            case ' ': expr++; break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                int val = 0;
                while (isdigit(*expr)) 
                {
                    val *= 10;
                    val += (int)(*expr++ - '0');
                }

                current_token.type = TOKEN_TYPE_number;
                current_token.val = val;
            } break;

            case '+':
            {
                current_token.type = TOKEN_TYPE_add;
                expr++;
            } break;

            case '-':
            {
                if (next_char(expr) == ' ')
                {
                    current_token.type = TOKEN_TYPE_subtract;
                    expr++;
                }
                else
                {
                    int val = 0;
                    while (isdigit(*expr)) 
                    {
                        val *= 10;
                        val += (int)(*expr++ - '0');
                    }

                    current_token.type = TOKEN_TYPE_number;
                    current_token.val = -val; // negative number
                }
            } break;

            case '*':
            {
                current_token.type = TOKEN_TYPE_multiply;
                expr++;
            } break;

            case '/':
            {
                current_token.type = TOKEN_TYPE_divide;
                expr++;
            } break;

            case '(':
            {
                current_token.type = TOKEN_TYPE_lparen;
                expr++;
            } break;

            case ')':
            {
                current_token.type = TOKEN_TYPE_rparen;
                expr++;
            } break;

            default:
            {
                printf("ERROR: character %c not supported", *expr);
                exit(1);
            }
        }

        sb_push(token_buffer, current_token);
    }

    return token_buffer;
}

/* Creates new AST node */
AST *ast_new(AST ast)
{
    AST *node = (AST *)malloc(sizeof(AST));
    if (node) *node = ast;
    return node;
}

/* Recursively frees AST giving the root node */
void ast_free(AST *root)
{
    switch (root->tag) 
    {
        case AST_TYPE_main: 
        {
            ast_free(root->data.body);
        } break;

        case AST_TYPE_add:
        case AST_TYPE_subtract:
        case AST_TYPE_multiply:
        case AST_TYPE_divide:
        {
            ast_free(root->data.op.left);
            ast_free(root->data.op.right);
        } break;

        default: break;
    }

    free(root);
}

/* Recursively prints AST giving the root node */
void ast_print(AST *root) 
{
    switch (root->tag) 
    {
        case AST_TYPE_main: 
        {
            printf("main() = ");
            ast_print(root->data.body);
            return;
        }

        case AST_TYPE_number: 
        {
            printf("%d", root->data.number);
            return;
        }

        case AST_TYPE_add: 
        {
            printf("(");
            ast_print(root->data.op.left);
            printf(" + ");
            ast_print(root->data.op.right);
            printf(")");
            return;
        }

        case AST_TYPE_subtract:
        {
            printf("(");
            ast_print(root->data.op.left);
            printf(" - ");
            ast_print(root->data.op.right);
            printf(")");
            return;
        }

        case AST_TYPE_multiply:
        {
            printf("(");
            ast_print(root->data.op.left);
            printf(" * ");
            ast_print(root->data.op.right);
            printf(")");
            return;
        }

        case AST_TYPE_divide:
        {
            printf("(");
            ast_print(root->data.op.left);
            printf(" / ");
            ast_print(root->data.op.right);
            printf(")");
            return;
        }
    }
}

AST *ast_generate_from_expr(char *expr)
{
    Token *token_buffer = tokenize_expr(expr);

    // TODO: generate the ast

    AST *part_ast[1024];
    int part_ast_count = 0;

    int occurance_index;
    while (occurance_index = first_occurance_paren(expr), occurance_index != -1)
    {
        // part_ast[part_ast_count++] = collapse_paren(input, occurance_index);
    }

    AST *root = NULL;
    return root;
}

