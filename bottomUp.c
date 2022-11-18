#include "bottomUp.h"

int precedenceTable[8][8] = {
    {'>', '>', '>', '>', '<', '>', '<', '>'}, // * /
    {'<', '>', '>', '>', '<', '>', '<', '>'}, // + -
    {'<', '<', '>', '>', '<', '>', '<', '>'}, // CMP
    {'<', '<', '<', '>', '<', '>', '<', '>'}, // EQ
    {'<', '<', '<', '<', '<', '=', '<', -1},  // (
    {'>', '>', '>', '>', -1, '<', -1, '>'},   // )
    {'>', '>', '>', '>', -1, '>', -1, '>'},   // IDENTIFIER
    {'<', '<', '<', '<', -1, -1, '<', 0},     //$

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
    case FLOAT:
        return I_FLOAT;
        break;
    case INT:
        return I_INT;
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
        return TOP_BOTTOM;
    }
}

Term *initTerm(char *value, int type)
{
    Term *t = calloc(1, sizeof(Term));
    if (value)
    {
        t->value = malloc((strlen(value) + 1) * sizeof(char));
        strcpy(t->value, value);
        t->termLen = strlen(value);
    }

    t->isStop = false;

    t->type = type;
    return t;
}

bool reduce(Stack *stack)
{
    Term *terms[3] = {NULL};
    int termCount = 0;
    while (stack->top && termCount != 3)
    {
        StackNode *popped = pop(stack);
        terms[termCount] = popped->term;
        free(popped);
        if (terms[termCount]->isStop)
        {
            termCount++;
            break;
        }
        termCount++;
    }
    if (ruleDecider(terms, termCount))
    {
        Term *term;
        StackNode *stackNode;

        term = initTerm(termCount == 1 ? terms[0]->value : NULL, EXPRESSION);
        stackNode = initNode(term);
        for (int i = 0; i < termCount; i++)
            term->childTerms[i] = terms[i];
        if (term->childTerms[0] == L_BRACKET)
        {
            term->type = term->childTerms[1]->type;
        }
        else if (term->childTerms[0]->type == I_FLOAT || term->childTerms[2]->type == I_FLOAT)
        {
            term->type = I_FLOAT;
        }
        else if (term->childTerms[0]->type == I_INT || term->childTerms[2]->type == I_INT)
        {
            term->type = I_INT;
        }

        push(stack, stackNode);
        return true;
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
        return ruleIdentifier(terms[0]);
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
    if (t->type != I_FLOAT || t->type != I_INT)
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
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
        break;
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;
        break;

    default:
        printf("ERROR");
        break;
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
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
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
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
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
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
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
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        printf("ERROR");
    }

    return true;
}

bool freeAndReturn(bool success, Stack *stack)
{
    return success;
}

bool bottomUp(Expression *exp)
{
    int currentExpPos = 0;

    Term *startTerm = initTerm("$", TOP_BOTTOM);
    Token *endToken = tokenInit();
    addCharToToken('$', endToken);
    endToken->t = -1;

    addTokenToExpression(exp, endToken);

    StackNode *startNode = initNode(startTerm);

    Stack *stack = initStack(startNode);

    while (currentExpPos < exp->arrayLen)
    {
        Term *newTerm;
        StackNode *newStackNode;
        int expType;
        if (exp->tokenArray[currentExpPos]->t == VAR_ID)
        {
            // SEMANTIC CHECK
            htab_pair_t *pair = htab_search(symTable, exp->tokenArray[currentExpPos]->val);
            if (pair && pair->variable)
            {
                expType = convertTokenToTermType(pair->variable->t);
            }
            else
            {
                return freeAndReturn(false, stack);
            }
        }
        else
        {
            expType = convertTokenToTermType(exp->tokenArray[currentExpPos]->t);
        }

        int termType;
        if (stack->top->term->type == EXPRESSION)
        {
            termType = stack->top->previous->term->type;
        }
        else
        {
            termType = stack->top->term->type;
        }

        StackNode *tmp = stack->top;

        switch (precedenceTable[termType][expType])
        {
        case '<':
            /* printf("SHIFTING\n"); */
            newTerm = initTerm(exp->tokenArray[currentExpPos]->val, expType);
            newStackNode = initNode(newTerm);

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

        case '>':
            /* printf("REDUCING\n"); */
            reduce(stack);

            break; // TODO throw error when false

        case '=':
            newTerm = initTerm(exp->tokenArray[currentExpPos]->val, expType);
            newStackNode = initNode(newTerm);
            push(stack, newStackNode);
            currentExpPos++;
            break;
        case 0:

            /* while (tmp != NULL)
            {
                printf("Type: %d, Value: %s\n", tmp->term->type, tmp->term->value);

                tmp = tmp->previous;
            }

            for (int i = 0; i < 3; i++)
            {
                if (stack->top->term->childTerms[i]->value)
                {
                    printf("%s\n", stack->top->term->childTerms[i]->value);
                }
            } */

            return freeAndReturn(stack->top->term->type == EXPRESSION, stack);

        default:
            printf("ERROR");
            return freeAndReturn(false, stack);
        }
    }
    return freeAndReturn(false, stack);
}
