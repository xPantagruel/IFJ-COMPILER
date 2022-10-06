#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void getProlog() {
    char prologValidString[] = "<?phpdeclare(strict_types=1);";
    char prologString[30];
    
    int pos = 0; //actual position in new string
    int c;
    while (strlen(prologString) != strlen(prologValidString)) {
        c = getchar();
        if (!isspace(c)) { //ignoring spaces
            prologString[pos] = c;
            ++pos;
            prologString[pos] = '\0';
        }
    }

    if (strcmp(prologString, prologValidString)) {
        exit(2);
    }
}

Token *getToken() {
    if (!prologFound) {
        getProlog();
        prologFound = 1;
    }

    Token *token = malloc(sizeof(Token));
    return token;
}

//remove me
//todo case atd + konecny prolog
int main() {
    getProlog();
}
