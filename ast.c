#include "ast.h"
#include "util.h"

// Lexer //

/* Peeks at the next char in string */
static inline char next_char(char *string)
{
    return *(string + 1);
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
                current_token.type = TOKEN_TYPE_plus;
                expr++;
            } break;

            case '-':
            {
                if (next_char(expr) == ' ')
                {
                    current_token.type = TOKEN_TYPE_minus;
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
                current_token.type = TOKEN_TYPE_asterisk;
                expr++;
            } break;

            case '/':
            {
                current_token.type = TOKEN_TYPE_slash;
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


// Parser //

/* Returns the index of the closing paren or -1 if not found */
static int find_closing_paren(Token *token_buffer, int open_paren_index)
{
    int open_paren_count = 1;

    for (int i = ++open_paren_index; i < sb_len(token_buffer); ++i)
    {
        Token current_token = token_buffer[i];
        switch (current_token.type) 
        {
            case TOKEN_TYPE_lparen: ++open_paren_count; break;
            case TOKEN_TYPE_rparen: --open_paren_count; break;
            default: break;
        }

        if (open_paren_count == 0)
        {
            return i;
        }
    }

    return -1;
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
            ast_free(root->body);
        } break;

        case AST_TYPE_add:
        case AST_TYPE_subtract:
        case AST_TYPE_multiply:
        case AST_TYPE_divide:
        {
            ast_free(root->left);
            ast_free(root->right);
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
            ast_print(root->body);
            return;
        }

        case AST_TYPE_number: 
        {
            printf("%d", root->val);
            return;
        }

        case AST_TYPE_add: 
        {
            printf("(");
            ast_print(root->left);
            printf(" + ");
            ast_print(root->right);
            printf(")");
            return;
        }

        case AST_TYPE_subtract:
        {
            printf("(");
            ast_print(root->left);
            printf(" - ");
            ast_print(root->right);
            printf(")");
            return;
        }

        case AST_TYPE_multiply:
        {
            printf("(");
            ast_print(root->left);
            printf(" * ");
            ast_print(root->right);
            printf(")");
            return;
        }

        case AST_TYPE_divide:
        {
            printf("(");
            ast_print(root->left);
            printf(" / ");
            ast_print(root->right);
            printf(")");
            return;
        }
    }
}

static AST *parse2(Token *token_buffer)
{
    AST *root = NULL;

    for (int i = 1; i < sb_len(token_buffer) - 1; ++i)
    {
        AST *current;
        Token current_token = token_buffer[i];
        switch (current_token.type) 
        {
            case TOKEN_TYPE_minus:
            case TOKEN_TYPE_plus:
            {
                Token left_token = (root == NULL) ? 
                    token_buffer[i - 1] : 
                    (Token) { .type = TOKEN_TYPE_ast, .node = root };
                Token right_token = token_buffer[i + 1];

                // TODO: assert left and right token are either number or ast

                AST *left = (left_token.type == TOKEN_TYPE_ast) ? 
                    left_token.node : ast_new((AST) { 
                        .tag = AST_TYPE_number, .val = left_token.val 
                    });

                AST *right = (right_token.type == TOKEN_TYPE_ast) ? 
                    right_token.node : ast_new((AST) { 
                        .tag = AST_TYPE_number, .val = right_token.val 
                    });

                current = ast_new((AST) {
                    .tag = (current_token.type == TOKEN_TYPE_plus) ? 
                        AST_TYPE_add : AST_TYPE_subtract,
                    .left = left,
                    .right = right,
                });
            } break;

            default: break;
        }

        root = current;
    }

    return root;
}

static AST *parse1(Token *token_buffer)
{
    return parse2(token_buffer);
}

AST *parse0(Token *token_buffer, int offset, int buffer_len)
{
    for (int i = offset; i < buffer_len; ++i)
    {
        Token current_token = token_buffer[i];
        switch (current_token.type) 
        {
            case TOKEN_TYPE_minus:
            {
                Token next_token = token_buffer[++i];
                AST *node = NULL;

                if (next_token.type == TOKEN_TYPE_number)
                {
                    node = ast_new((AST) {
                        .tag = AST_TYPE_negative,
                        .body = ast_new((AST) {
                            .tag = AST_TYPE_number,
                            .val = next_token.val
                        })
                    });
                }
                else if (next_token.type == TOKEN_TYPE_lparen)
                {
                    int closing_paren_index = find_closing_paren(token_buffer, i);
                    if (closing_paren_index < i || closing_paren_index > buffer_len)
                    {
                        // Error
                    }

                    AST *body_node = parse0(token_buffer, i, closing_paren_index);
                    node = ast_new((AST) {
                        .tag = AST_TYPE_negative,
                        .body = body_node 
                    });
                }
                else
                {
                    // Error
                }
            } break;

            case TOKEN_TYPE_number:
            {
            } break;

            case TOKEN_TYPE_lparen:
            {
            } break;

            default:
            {
                // TODO: Better err message
                printf("Unexpected token while parsing\n");
                exit(1);
            }
        }
    }

    return parse1(token_buffer);
}

AST *ast_generate_from_expr(char *expr)
{
    Token *token_buffer = tokenize_expr(expr);
    return parse0(token_buffer, 0, sb_len(token_buffer));
}

