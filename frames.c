#include "frames.h"

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

Frame *eraseFrame(Frame *frame)
{
    Frame *parent = frame->parent;
    frame = NULL;
    return parent;
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
