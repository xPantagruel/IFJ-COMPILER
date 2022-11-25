#include "frames.h"

FrameStack *frameStack;

FrameStack *initFrameStack()
{
    FrameStack *stack = malloc(sizeof(FrameStack));
    stack->current = NULL;
    pushFrame(stack, GLOBAL_FRAME_NAME);
    return stack;
}

void pushFrame(FrameStack *stack, const char *name)
{
    Frame *frame = malloc(sizeof(Frame));
    frame->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(frame->name, name);
    frame->varCount = 0;
    frame->vars = NULL;
    frame->parent = stack->current;
    stack->current = frame;
}

Frame *popFrame(FrameStack *stack)
{
    if (strcmp(stack->current->name, GLOBAL_FRAME_NAME) != 0)
    {
        Frame *pop = stack->current;
        stack->current = stack->current->parent;
        return pop;
    }
    return NULL;
}

Frame *peekFrame(FrameStack *stack)
{
    return stack->current;
}

Frame *findFrame(FrameStack *stack, const char *name)
{
    Frame *current = stack->current;
    while (stack->current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->parent;
    }
    return NULL;
}

bool addVariableToFrame(Frame *frame, htab_pair_t *var)
{
    if (frame->varCount == 0)
    {
        frame->vars = calloc(1, sizeof(htab_pair_t *));
        frame->vars[frame->varCount] = var;
    }
    else
    {
        frame->vars = realloc(frame->vars, (frame->varCount + 1) * sizeof(htab_pair_t *));
        frame->vars[frame->varCount] = var;
    }
    frame->varCount++;
    return true;
}

Frame *eraseFrame(Frame *frame)
{
    for (int i = 0; i < frame->varCount; i++)
    {
        htab_erase_variable(frame->vars[i]->variable);
        frame->vars[i]->variable = NULL;
    }
    free(frame->vars);

    frame = NULL;
    return NULL;
}

void eraseFrameStack(FrameStack *stack)
{
    Frame *current = stack->current;
    while (current != NULL)
    {
        current = eraseFrame(current);
    }
    free(stack);
}

void printFrameStack(FrameStack *stack)
{
    printf("==========\n");
    printf("FRAMESTACK:\n\n");
    Frame *tmp = stack->current;
    while (tmp)
    {
        printf("name: %s\n", tmp->name);
        if (tmp->varCount)
            printf("vars:\n");

        for (int i = 0; i < tmp->varCount; i++)
        {
            printf("\t%s\n", tmp->vars[i]->variable->name);
        }

        if (tmp->parent)
        {
            printf("parent: %s\n", tmp->parent->name);
            printf(" ↓\n");
        }
        else
            printf("\n");

        tmp = tmp->parent;
    }
    printf("==========\n");
}
