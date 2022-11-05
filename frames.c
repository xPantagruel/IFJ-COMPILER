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
    Frame *frame = malloc(sizeof(frame));
    frame->name = calloc(strlen(name), sizeof(char));
    strcpy(frame->name, name);
    frame->parent = stack->current;
    stack->current = frame;
}

Frame *popFrame(FrameStack *stack)
{
    if (strcmp(stack->current->name, GLOBAL_FRAME_NAME))
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
    free(frame->name);
    free(frame);
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