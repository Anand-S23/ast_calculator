#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

// Note: Inspired by Vladimir Keleshev's blog post: 
// https://keleshev.com/abstract-syntax-tree-an-example-in-c/

void ast_test(void)
{
    char *expr = "4 + 3\0";
    AST *root = ast_generate_from_expr(expr);
    ast_print(root);
    printf("\n\n");
}

int main(int argc, char **argv)
{
    ast_test();

    for (int i = 0; i < argc; ++i)
    {
        printf("%s\n", argv[i]);
    }

    return 0;
}
