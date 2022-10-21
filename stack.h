#ifndef STACK_H
#define STACK_H

#include "bottomUp.h"
#include <stdbool.h>

typedef struct term Term;

typedef struct stackNode
{
    struct stackNode *next;
    struct stackNode *previous;
    Term *term;
} StackNode;

typedef struct stack
{
    StackNode *top;
} Stack;

StackNode *initNode(Term *term);

Stack *initStack(StackNode *startNode);

StackNode *pop(Stack *s);

bool freeStack(Stack *s);

bool push(Stack *s, StackNode *t);

#endif