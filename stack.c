#include "bottomUp.h"
// TODO: add memory cleanup, comments

Stack *initStack(StackNode *startNode)
{
    Stack *s = calloc(1, sizeof(Stack));
    if (!s)
    {
        printf("FUCK");
        return NULL;
    }
    s->top = startNode;
    return s;
}

StackNode *initNode(Term *term)
{
    StackNode *n = calloc(1, sizeof(StackNode));
    n->term = term;
    return n;
}

StackNode *pop(Stack *s)
{
    StackNode *toPop = s->top;
    if (s->top)
    {
        s->top = s->top->previous;
    }

    return toPop;
}

bool push(Stack *s, StackNode *t)
{
    StackNode *n = s->top;
    s->top = t;
    t->previous = n;
    return true;
}

void freeChildTerms(Term *t)
{
    for (int i = 0; i < 3; i++)
    {
        if (t->childTerms[i])
        {
            if (t->childTerms[i]->childTerms[0])
            {
                freeChildTerms(t->childTerms[i]);
            }
            printf("FREEING: %d\n", t->childTerms[i]->type);
            if (t->childTerms[i]->value)
            {
                free(t->childTerms[i]->value);
            }

            free(t->childTerms[i]);
        }
    }
}

bool freeStack(Stack *s)
{
    if (!s)
    {
        printf("Stack doesnt exist");
        return false;
    }

    StackNode *tmp = pop(s);
    while (tmp)
    {
        if (tmp->term->childTerms[0])
        {
            freeChildTerms(tmp->term);
        }

        if (tmp->term->value)
        {
            printf("FREEING: %s\n", tmp->term->value);
            free(tmp->term->value);
        }
        printf("FREEING: %d\n", tmp->term->type);
        free(tmp->term);
        free(tmp);
        tmp = pop(s);
    }
    if (s)
    {
        printf("FREEING STACK\n");
        free(s);
        printf("Stack is free");
    }

    return true;
}
