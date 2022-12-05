#include <string.h>  // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

char *tempString(char *string)
{
    char *tempString = calloc(strlen(string) + 1, 1);
    strcpy(tempString, string);
    return tempString;
}