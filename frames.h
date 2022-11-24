#ifndef FRAMES_H
#define FRAMES_H
#include <string.h>  // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "symtable.h"

#define GLOBAL_FRAME_NAME "global"

typedef struct frame Frame;
typedef struct frameStack FrameStack;
typedef struct htab_pair htab_pair_t;

extern FrameStack *frameStack;

typedef struct frame
{
    char *name;
    Frame *parent;
    htab_pair_t **vars;
    int varCount;
} Frame;

typedef struct frameStack
{
    Frame *current;
} FrameStack;

void pushFrame(FrameStack *stack, const char *name);

Frame *popFrame(FrameStack *stack);

Frame *findFrame(FrameStack *stack, const char *name);

Frame *peekFrame(FrameStack *stack);

bool addVariableToFrame(Frame *frame, htab_pair_t *var);

Frame *eraseFrame(Frame *frame);

void eraseFrameStack(FrameStack *stack);

void printFrameStack(FrameStack *stack);

FrameStack *initFrameStack();
#endif
