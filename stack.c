#include "stack.h"
#include "bottomUp.h"
// TODO: add memory cleanup, comments

void initStack(Stack *s, StackNode *startNode)
{
    s = calloc(1, sizeof(Stack));
    push(s, startNode);
}

void initNode(StackNode *n, Term *term)
{
    n = calloc(1, sizeof(StackNode));
    n->term = term;
    n->next = NULL;
    n->previous = NULL;
}

StackNode *pop(Stack *s)
{
    StackNode *toPop = s->top;
    s->top = s->top->previous;
    return toPop;
}

bool *push(Stack *s, StackNode *t)
{
    StackNode *n = s->top;
    s->top = t;
    t->previous = n;
}
