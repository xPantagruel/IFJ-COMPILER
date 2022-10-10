#include "scanner.h"

int main() {
    Token *token = getToken();
    while (strcmp(token->val, "EOF")) {
        printf("%s %d\n", token->val, token->t);
        dtorToken(token);
        token = getToken();
    }
    printf("%s\n", token->val); //print EOF
}
