
typedef struct stackNode
{
    StackNode *next;
    StackNode *previous;
    Term *term;
} StackNode;

typedef struct stack
{
    StackNode *top;
} Stack;

void initNode(StackNode *n, Term *term);

void initStack(Stack *s, StackNode *startNode);

StackNode *pop(Stack *s);

bool *push(Stack *s, StackNode *t);
