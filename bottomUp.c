#include "bottomUp.h"

int precedenceTable[11][11] = {
    //*/   +-.  CMP  EQ   (    )    I    F    S    $
    {'>', '>', '>', '>', '>', '<', '>', '<', '<', '<', '>'}, // * /
    {'<', '>', '>', '>', '>', '<', '>', '<', '<', '<', '>'},
    {'<', '>', '>', '>', '>', '<', '>', '<', '<', '<', '>'}, // + - .
    {'<', '<', '<', '>', '>', '<', '>', '<', '<', '<', '>'}, // CMP
    {'<', '<', '<', '<', '>', '<', '>', '<', '<', '<', '>'}, // EQ
    {'<', '<', '<', '<', '<', '<', '=', '<', '<', '<', -1},  // (
    {'>', '>', '>', '>', '>', -1, '<', -1, -1, -1, '>'},     // )
    {'>', '>', '>', '>', '>', -1, '>', -1, -1, -1, '>'},
    {'>', '>', '>', '>', '>', -1, '>', -1, -1, -1, '>'},  // IDENTIFIER
    {'>', '>', '>', '>', '>', -1, '>', -1, -1, -1, '>'},  // IDENTIFIER
    {'<', '<', '<', '<', '<', '<', -1, '<', '<', '<', 0}, //$

};

void printBottomUp(Stack *s, Expression *e, int current)
{
    printf("\n");
    StackNode *tmp = s->top;
    while (tmp && tmp->previous)
    {
        tmp = tmp->previous;
    }

    while (tmp)
    {
        if (tmp->term->isStop)
        {
            printf("<");
        }

        if (tmp->term->childTerms[0])
        {
            printf("'");
        }
        if (tmp->term->type != -1)
        {
            printf("[%d", tmp->term->type);
        }
        else
        {
            printf("[NULL");
        }

        if (tmp->term->value)
        {
            printf(",%s] ", tmp->term->value);
        }
        else
        {
            printf(",NULL] ");
        }

        tmp = tmp->next;
    }
    printf("\t\t");
    for (int i = current; i < e->arrayLen; i++)
    {
        printf("[%d", e->tokenArray[i]->t);
        printf(",%s] ", e->tokenArray[i]->val);
    }
    printf("\n");
}

int convertTokenToTermType(int type)
{
    switch (type)
    {
    case PLUS:
    case MINUS:
        return PLUS_MINUS;
        break;
    case DOT:
        return CONCAT;
        break;
    case MUL:
    case SLASH:
        return MULT_DIV;
        break;
    case INT:
        return I_INT;
        break;
    case FLOAT:
        return I_FLOAT;
        break;
    case STRING:
        return I_STRING;
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

Term *initTerm(char *value, int type, Token *originalToken)
{
    Term *t = calloc(1, sizeof(Term));
    if (value)
    {
        t->value = malloc((strlen(value) + 1) * sizeof(char));
        strcpy(t->value, value);
        t->termLen = strlen(value);
    }
    t->originalToken = originalToken;
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
        if (terms[0])
        {
            if (terms[1])
            {
                terms[2] = terms[1];
            }
            terms[1] = terms[0];
        }
        terms[0] = popped->term;

        free(popped);
        if (terms[0]->isStop)
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

        term = initTerm(termCount == 1 ? terms[0]->value : NULL, EXPRESSION, NULL);
        stackNode = initNode(term);
        for (int i = 0; i < termCount; i++)
            term->childTerms[i] = terms[i];
        if (termCount == 3)
        {
            if (term->childTerms[0]->type == L_BRACKET)
            {
                term->type = term->childTerms[1]->type;
            }
            else if (term->childTerms[0]->type == I_FLOAT || term->childTerms[2]->type == I_FLOAT)
            {
                term->childTerms[0]->type = I_FLOAT;
                term->childTerms[2]->type = I_FLOAT;
                term->type = I_FLOAT;
            }
            else if (term->childTerms[0]->type == I_INT || term->childTerms[2]->type == I_INT)
            {
                if (term->childTerms[1]->value && term->childTerms[1]->value[0] == '/')
                {
                    term->type = I_FLOAT;
                }
                else
                {
                    term->type = I_INT;
                }
            }
            else if (term->childTerms[0]->type == I_STRING && term->childTerms[2]->type == I_STRING)
            {
                term->type = I_STRING;
            }
            for (int i = 0; i < termCount; i++)
            {
                if (term->childTerms[i]->originalToken && term->childTerms[i]->value && term->childTerms[i]->type != L_BRACKET && term->childTerms[i]->type != R_BRACKET)
                {
                    if (term->childTerms[i]->type == I_FLOAT)
                    {
                        term->childTerms[i]->originalToken->t = FLOAT;
                    }
                    codeGeneration(term->childTerms[i]->originalToken);
                }
            }
        }
        else
        {
            term->originalToken = term->childTerms[0]->originalToken;
            term->type = term->childTerms[0]->type;
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
                ruleConcat(terms[0], terms[1], terms[2]) ||
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
    if (t->type != I_FLOAT && t->type != I_INT && t->type != I_STRING)
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
        return false;
        break;
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:

        break;

    default:
        return false;
        break;
    }

    return true;
}

bool ruleConcat(Term *t1, Term *operator, Term * t2)
{
    if (operator->type != CONCAT)
    {
        return false;
    }

    switch (t1->type)
    {

    case I_STRING:
        break;

    default:
        return false;
        break;
    }

    switch (t2->type)
    {
    case I_STRING:
        break;

    default:
        return false;
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
        return false;
        break;
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case EXPRESSION:
        break;

    default:
        return false;
        break;
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
    case I_STRING:
    case EXPRESSION:
        break;

    default:
        return false;
        break;
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
    case I_STRING:
    case EXPRESSION:
        break;

    default:
        return false;
        break;
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case I_STRING:
    case EXPRESSION:
        break;

    default:
        return false;
        break;
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
    case I_STRING:
    case EXPRESSION:
        break;

    default:
        return false;
        break;
    }

    switch (t2->type)
    {
    case I_INT:
    case I_FLOAT:
    case I_STRING:
    case EXPRESSION:
        break;

    default:
        return false;
        break;
    }

    return true;
}

bool freeAndReturn(bool success, Stack *stack)
{
    (void)stack;
    return success;
}

bool bottomUp(Expression *exp, int *resultType)
{

    *resultType = 0;
    int currentExpPos = 0;

    Term *startTerm = initTerm("$", TOP_BOTTOM, NULL);
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
        }
        else
        {
            expType = convertTokenToTermType(exp->tokenArray[currentExpPos]->t);
        }

        int termType;
        if (stack->top->term->childTerms[0])
        {
            termType = stack->top->previous->term->type;
        }
        else
        {
            termType = stack->top->term->type;
        }

        switch (precedenceTable[termType][expType])
        {
        case '<':
            newTerm = initTerm(exp->tokenArray[currentExpPos]->val, expType, exp->tokenArray[currentExpPos]);
            newStackNode = initNode(newTerm);

            push(stack, newStackNode);
            if (stack->top->previous && stack->top->previous->term->childTerms[0])
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
            reduce(stack);

            break; // TODO throw error when false

        case '=':
            newTerm = initTerm(exp->tokenArray[currentExpPos]->val, expType, exp->tokenArray[currentExpPos]);
            newStackNode = initNode(newTerm);
            push(stack, newStackNode);
            currentExpPos++;
            break;
        case 0:
            switch (stack->top->term->type)
            {
            case I_INT:

                *resultType = INT;
                break;
            case I_FLOAT:

                *resultType = FLOAT;
                break;
            case I_STRING:
                *resultType = STRING;
                break;

            default:
                break;
            }
            return freeAndReturn(true, stack);

        default:

            return freeAndReturn(false, stack);
        }
    }
    return freeAndReturn(false, stack);
}
