#ifndef FRAMES_H
#define FRAMES_H
#include <string.h>  // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define GLOBAL_FRAME_NAME "global"

typedef struct frame Frame;
typedef struct frameStack FrameStack;

extern FrameStack *frameStack;

typedef struct frame
{
    char *name;
    Frame *parent;
} Frame;

typedef struct frameStack
{
    Frame *current;
} FrameStack;

void pushFrame(FrameStack *stack, const char *name);

Frame *popFrame(FrameStack *stack);

Frame *findFrame(FrameStack *stack, const char *name);

Frame *peekFrame(FrameStack *stack);

Frame *eraseFrame(Frame *frame);

void eraseFrameStack(FrameStack *stack);

void printFrameStack(FrameStack *stack);

FrameStack *initFrameStack();
#endif
