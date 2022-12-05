#include "bottomUp.h"

int precedenceTable[12][12] = {
    //*/   +-.  CMP  EQ   (    )    I    F    S    $
    {'>', '>', '>', '>', '>', '<', '>', '<', '<', '<', '<', '>'}, // * /
    {'<', '>', '>', '>', '>', '<', '>', '<', '<', '<', '<', '>'},
    {'<', '>', '>', '>', '>', '<', '>', '<', '<', '<', '<', '>'}, // + - .
    {'<', '<', '<', '>', '>', '<', '>', '<', '<', '<', '<', '>'}, // CMP
    {'<', '<', '<', '<', '>', '<', '>', '<', '<', '<', '<', '>'}, // EQ
    {'<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', -1},  // (
    {'>', '>', '>', '>', '>', -1, '<', -1, -1, -1, -1, '>'},      // )
    {'>', '>', '>', '>', '>', -1, '>', -1, -1, -1, -1, '>'},
    {'>', '>', '>', '>', '>', -1, '>', -1, -1, -1, -1, '>'}, // IDENTIFIER
    {'>', '>', '>', '>', '>', -1, '>', -1, -1, -1, -1, '>'},
    {'>', '>', '>', '>', '>', -1, '>', -1, -1, -1, -1, '>'},   // IDENTIFIER
    {'<', '<', '<', '<', '<', '<', -1, '<', '<', '<', '<', 0}, //$

};

void changeTermValue(Term *t, char *value)
{
    t->value = realloc(t->value, strlen(value) + 1);
    strcpy(t->value, value);
}

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
    case NULL_KEYWORD:
        return I_NULL;
    default:
        return TOP_BOTTOM;
    }
}

int convertTermToTokenType(enum termType type, enum type originalType)
{
    switch (type)
    {
    case I_INT:
        return INT;
        break;
    case I_FLOAT:
        return FLOAT;
        break;
    case I_STRING:
        return STRING;
    case I_NULL:
        return NULL_KEYWORD;
    default:
        return originalType;
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

int loadTermsToExpression(Term *expression, Stack *stack)
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
    for (int i = 0; i < termCount; i++)
        expression->childTerms[i] = terms[i];

    return termCount;
}

void changeTokenValue(Token *token, char *newValue)
{
    if (newValue && (strcmp(token->val, newValue) != 0))
    {
        token->val = realloc(token->val, strlen(newValue) + 1);
        strcpy(token->val, newValue);
    }
}

bool reduce(Stack *stack)
{

    Term *term;
    term = initTerm(NULL, EXPRESSION, NULL);

    int termCount = loadTermsToExpression(term, stack);
    if (ruleDecider(term->childTerms, termCount))
    {
        StackNode *stackNode;
        stackNode = initNode(term);

        if (termCount == 3)
        {

            term->type = convertTypesInReduce(term->childTerms[0], term->childTerms[1], term->childTerms[2]);
            for (int i = 0; i < 3; i++)
            {

                if (term->childTerms[i]->value)
                {
                    if (term->childTerms[i]->originalToken->t != VAR_ID)
                    {
                        term->childTerms[i]->originalToken->t = convertTermToTokenType(term->childTerms[i]->type, term->childTerms[i]->originalToken->t);
                        changeTokenValue(term->childTerms[i]->originalToken, term->childTerms[i]->value);
                    }

                    codeGeneration(term->childTerms[i]->originalToken);
                }
            }
        }
        else
        {
            changeTermValue(term, term->childTerms[0]->value);
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

int convertTypesInReduce(Term *t1, Term *operator, Term * t2)
{

    if (t1->type == L_BRACKET && t2->type == R_BRACKET)
    {
        return convertBrackets(operator);
    }

    switch (operator->type)
    {
    case PLUS_MINUS:
        return convertPlusMinusMul(t1, t2);
    case MULT_DIV:
        if (operator->value[0] == '/')
        {
            return convertDiv(t1, t2);
        }
        else
        {
            return convertPlusMinusMul(t1, t2);
        }
    case CONCAT:
        return convertDot(t1, t2);
    case COMPARE:
    case EQUALS:
        return convertCompareEquals(t1, t2);
    default:
        FREE_EXIT(99, ERROR_99_INTERNAL_ERROR, "convertTypesInReduce");
        break;
    }
}

int convertPlusMinusMul(Term *t1, Term *t2)
{
    if (t1->type == I_STRING || t2->type == I_STRING)
    {
        FREE_EXIT(7, ERROR_7_INCOMPATIBLE_TYPE, "");
    }
    else if (t1->type == I_FLOAT || t2->type == I_FLOAT)
    {
        if (t1->type == I_NULL)
        {
            changeTermValue(t1, "0.0");
        }
        if (t2->type == I_NULL)
        {
            changeTermValue(t2, "0.0");
        }
        t1->type = I_FLOAT;
        t2->type = I_FLOAT;
        return FLOAT;
    }
    else
    {
        if (t1->type == I_NULL)
        {
            changeTermValue(t1, "0");
        }
        if (t2->type == I_NULL)
        {
            changeTermValue(t2, "0");
        }
        t1->type = I_INT;
        t2->type = I_INT;
        return I_INT;
    }
}

int convertDiv(Term *t1, Term *t2)
{
    if (t1->type == I_STRING || t2->type == I_STRING)
    {
        FREE_EXIT(7, ERROR_7_INCOMPATIBLE_TYPE, "");
    }

    if (t1->type == I_NULL)
    {
        changeTermValue(t1, "0.0");
    }
    if (t2->type == I_NULL)
    {
        changeTermValue(t2, "0.0");
    }
    t1->type = I_FLOAT;
    t2->type = I_FLOAT;
    return I_FLOAT;
}

int convertDot(Term *t1, Term *t2)
{
    if (!(t1->type == I_STRING || t1->type == I_NULL) || !(t1->type == I_STRING || t1->type == I_NULL))
    {
        FREE_EXIT(7, ERROR_7_INCOMPATIBLE_TYPE, "");
    }
    if (t1->type == I_NULL)
    {
        changeTermValue(t1, "");
    }
    if (t1->type == I_NULL)
    {
        changeTermValue(t1, "");
    }
    t1->type = I_STRING;
    t2->type = I_STRING;
    return I_STRING;
}

int convertCompareEquals(Term *t1, Term *t2)
{
    if (t1->type == I_STRING || t2->type == I_STRING)
    {
        if (!(t1->type == I_STRING || t1->type == I_NULL) || !(t1->type == I_STRING || t1->type == I_NULL))
        {
            FREE_EXIT(7, ERROR_7_INCOMPATIBLE_TYPE, "");
        }
    }
    else if (t1->type == I_FLOAT || t2->type == I_FLOAT)
    {
        if (t1->type == I_NULL)
        {
            changeTermValue(t1, "0.0");
        }
        if (t2->type == I_NULL)
        {
            changeTermValue(t2, "0.0");
        }
        t1->type = I_FLOAT;
        t2->type = I_FLOAT;
    }
    else if (t1->type == I_INT || t2->type == I_INT)
    {
        if (t1->type == I_NULL)
        {
            changeTermValue(t1, "0");
        }
        if (t2->type == I_NULL)
        {
            changeTermValue(t2, "0");
        }
        t1->type = I_INT;
        t2->type = I_INT;
    }
    return BOOL;
}

int convertBrackets(Term *t)
{
    return t->type;
}

bool ruleDecider(Term *terms[3], int termCount)
{
    if (termCount == 1)
    {
        return ruleIdentifier(terms[0]);
    }
    else if (termCount == 3)
    {
        if (terms[0]->type == BOOL || terms[2]->type == BOOL)
        {
            FREE_EXIT(7, ERROR_7_INCOMPATIBLE_TYPE, "");
        }

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
    if (t->type != I_FLOAT && t->type != I_INT && t->type != I_STRING && t->type != I_NULL)
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
    case I_NULL:
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
    case I_NULL:

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
    case I_NULL:
        break;

    default:
        return false;
        break;
    }

    switch (t2->type)
    {
    case I_STRING:
    case I_NULL:
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
    case I_NULL:
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
    case I_NULL:
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
    case I_NULL:
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
    case I_NULL:
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
    case I_NULL:
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
    case I_NULL:
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
    case I_NULL:
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
            SymVariable *variable = getVariable(exp->tokenArray[currentExpPos]->val);
            if (!variable)
            {
                FREE_EXIT(5, ERROR_5_VARIABLE_NOT_DEFINED, exp->tokenArray[currentExpPos]->val)
            }

            expType = convertTokenToTermType(variable->type);
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
            if (!reduce(stack))
            {
                FREE_EXIT(2, ERROR_2_SYNTACTIC, "in expression");
            }

            break; // TODO throw error when false

        case '=':
            newTerm = initTerm(exp->tokenArray[currentExpPos]->val, expType, exp->tokenArray[currentExpPos]);
            newStackNode = initNode(newTerm);
            push(stack, newStackNode);
            currentExpPos++;
            break;

        case 0:
            if (resultType)
            {
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
            }

            return freeAndReturn(true, stack);

        default:

            return freeAndReturn(false, stack);
        }
    }
    return freeAndReturn(false, stack);
}
