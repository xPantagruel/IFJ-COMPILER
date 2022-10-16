#include "bottomUp.h"
#include "stack.h"

// TODO: add memory cleanup, comments

int precedenceTable[8][8] = {
    {'>', '>', '>', '>', '<', '>', '<', '>'}, // * /
    {'<', '>', '>', '>', '<', '>', '<', '>'}, // + -
    {'<', '<', '>', '>', '<', '>', '<', '>'}, // CMP
    {'<', '<', '<', '>', '<', '>', '<', '>'}, // EQ
    {'<', '<', '<', '<', '<', '=', '<', 0},   // (
    {'>', '>', '>', '>', 0, '<', 0, '>'},     // )
    {'>', '>', '>', '>', 0, '>', 0, '>'},     // IDENTIFIER
    {'<', '<', '<', '<', 0, 0, '<', 0},       //$

};

int convertTokenToTermType(int type)
{
    switch (type)
    {
    case PLUS:
    case MINUS:
        return PLUS_MINUS;
        break;
    case MUL:
    case SLASH:
        return MULT_DIV;
        break;
    case VAR_ID:
    case INT:
    case FLOAT:
        return IDENTIFIER;
        break;
    case L_PAR:
        return L_BRACKET;
    case R_PAR:
        return R_BRACKET;

    case LESS_EQ:
    case LESS:
    case MORE_EQ:
    case MORE:
        return COMPARE;
        break;
    case THREE_EQ:
    case NOT_EQ:
        return EQUALS;
        break;
    default:
        return -1;
    }
}

void initTerm(Term *t, char *value, int type)
{
    t = calloc(1, sizeof(Term));
    strcpy(t->value, value);
    if (t->childTerms[0] != NULL)
    {
        t->type = EXPRESSION;
        return;
    }

    if (value)
    {
        t->type = convertTokenToTermType(type);
    }
}

bool reduce(Stack *stack)
{
    Term *terms[3] = {NULL};
    int termCount = 0;
    while (stack->top->previous && termCount != 3)
    {
        terms[termCount] = pop(stack)->term;
        if (terms[termCount]->isStop)
            break;
    }
    if (ruleDecider(terms, termCount))
    {
        Term *term;
        initTerm(term, NULL, EXPRESSION);
        for (size_t i = 0; i < 3; i++)
            term->childTerms[i] = terms[i];
        push(stack, term);
    }
    else
    {
        return false;
    }
}

bool ruleDecider(Term *terms[3], int termCount)
{
    if (termCount == 1)
    {
        return ruleIdentifier(terms[termCount]);
    }
    else if (termCount == 3)
    {
        return (ruleMultDiv(terms[0], terms[1], terms[2]) ||
                rulePlusMinus(terms[0], terms[1], terms[2]) ||
                ruleBracket(terms[0], terms[1], terms[2]) ||
                ruleCompare(terms[0], terms[1], terms[2]) ||
                ruleEquals(terms[0], terms[1], terms[2]));
    }
    else
    {
        return false;
    }
}

bool ruleIdentifier(Term *t)
{
    if (t->type != IDENTIFIER)
    {
        return false;
    }
    return true;
}

bool rulePlusMinus(Term *t1, Term *operator, Term * t2)
{
    if (operator->type != PLUS_MINUS)
    {
        return false;
    }

    switch (t1->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    switch (t2->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    return true;
}

bool ruleMultDiv(Term *t1, Term *operator, Term * t2)
{
    if (operator->type != MULT_DIV)
    {
        return false;
    }

    switch (t1->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    switch (t2->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    return true;
}

bool ruleBracket(Term *leftBracket, Term *t1, Term *rightBracket)
{
    if (leftBracket->type != L_BRACKET || rightBracket->type != R_BRACKET)
    {
        return false;
    }

    switch (t1->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    return true;
}

bool ruleCompare(Term *t1, Term *operator, Term * t2)
{
    if (operator->type != COMPARE)
    {
        return false;
    }

    switch (t1->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    switch (t2->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    return true;
}

bool ruleEquals(Term *t1, Term *operator, Term * t2)
{
    if (operator->type != EQUALS)
    {
        return false;
    }

    switch (t1->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    switch (t2->type)
    {
    case IDENTIFIER:
    case EXPRESSION:
        break;

    default:
        // throw error
    }

    return true;
}

bool bottomUp(Expression *exp)
{
    int currentExpPos = 0;

    Term *startTerm;
    initTerm(startTerm, "$", TOP_BOTTOM);
    StackNode *startNode;
    initNode(startNode, startTerm);
    Stack *stack;
    initStack(stack, startNode);

    while (currentExpPos != exp->arrayLen)
    {
        Term *newTerm;
        StackNode *newStackNode;
        int expType = convertTokenToTermType(exp->tokenArray[currentExpPos]->t);
        int termType = stack->top->term->type;
        switch (precedenceTable[termType][expType])
        {
        case '>':
            initTerm(newTerm, exp->tokenArray[currentExpPos], expType);
            initNode(newStackNode, newTerm);
            push(stack, newStackNode);
            if (stack->top->previous && stack->top->previous->term->type == EXPRESSION)
            {
                stack->top->previous->term->isStop = true;
            }
            else
            {
                stack->top->term->isStop = true;
            }
            currentExpPos++;
            break;

        case '<':
            reduce(stack); // TODO throw error when false

        // TODO case '='
        default:
            break;
        }
    }
}