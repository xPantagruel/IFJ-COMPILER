/**
 * @file code_generation.c
 *
 * @author Martin Kubička (xkubic45)
 * @author Matěj Macek (xmacek27)
 * @author Dominik Petřík (xpetri25)
 * @author Lukáš Zítko (xzitko00)
 *
 * @brief  Code generation implementation
 *
 */
#include "code_generation.h"

//TODO
// -add pops in functions 
// -while nejdriv prijde token R_CPAR a pak teprve WHILE coz je problem pri generovani kodu WHILE zveda InWHile a podle ni se vykonava instrukce v R_CPAR
// -//  COMMA,        // ,     --DONE CHECK IF ITS RIGH by tests

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init(int num)
{
    if (num == 0)
    {
        listCodeGen = malloc(sizeof(DLList));
        if (listCodeGen == NULL)
        {
            exit(99);
        }
        listCodeGen->firstElement = NULL;
        listCodeGen->lastElement = NULL;
        listCodeGen->activeElement = NULL;
    }
    else if (num == 1)
    {
        listIfLabels = malloc(sizeof(DLList));
        if (listIfLabels == NULL)
        {
            exit(99);
        }
        listIfLabels->firstElement = NULL;
        listIfLabels->lastElement = NULL;
        listIfLabels->activeElement = NULL;
    }
    else if (num == 2)
    {
        listWhileLabels = malloc(sizeof(DLList));
        if (listWhileLabels == NULL)
        {
            exit(99);
        }
        listWhileLabels->firstElement = NULL;
        listWhileLabels->lastElement = NULL;
        listWhileLabels->activeElement = NULL;
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( int num) {
    if (num == 0)
    {	
        if (listCodeGen->firstElement != NULL) {
		listTmp =  listCodeGen->firstElement;

		if (listCodeGen->firstElement == listCodeGen->activeElement) { //checking if first element is active
			listCodeGen->activeElement = NULL;
		}

		if (listCodeGen->firstElement == listCodeGen->lastElement) { //checking if the first element is also the last element
			listCodeGen->lastElement = NULL;
		}

		listCodeGen->firstElement = listCodeGen->firstElement->nextElement;
		if (listCodeGen->firstElement != NULL) {
			listCodeGen->firstElement->previousElement = NULL;
		}
		free(listTmp);
	    }
    }else if(num == 1) {
        if (listIfLabels->firstElement != NULL) 
        {
            listTmp = listIfLabels->firstElement;

            if (listIfLabels->firstElement == listIfLabels->activeElement) { //checking if first element is active
                listIfLabels->activeElement = NULL;
            }

            if (listIfLabels->firstElement == listIfLabels->lastElement) { //checking if the first element is also the last element
                listIfLabels->lastElement = NULL;
            }

            listIfLabels->firstElement = listIfLabels->firstElement->nextElement;
            if (listIfLabels->firstElement != NULL) {
                listIfLabels->firstElement->previousElement = NULL;
            }
            free(listTmp);
	    }
        else if(num == 2)
        {
            if (listWhileLabels->firstElement != NULL) 
            {
                listTmp= listWhileLabels->firstElement;

                if (listWhileLabels->firstElement == listWhileLabels->activeElement) { //checking if first element is active
                    listWhileLabels->activeElement = NULL;
                }

                if (listWhileLabels->firstElement == listWhileLabels->lastElement) { //checking if the first element is also the last element
                    listWhileLabels->lastElement = NULL;
                }

                listWhileLabels->firstElement = listWhileLabels->firstElement->nextElement;
                if (listWhileLabels->firstElement != NULL) {
                    listWhileLabels->firstElement->previousElement = NULL;
                }
                free(listTmp);
            }
        }
    }
}

void DLL_Dispose(int num)
{
    if (num == 0)
    {
        while (listCodeGen->firstElement != NULL)
        { // removing first element until first element will be NULL
            DLL_DeleteFirst(0);
        }
    }
    else if (num == 1)
    {
        while (listIfLabels->firstElement != NULL)
        { // removing first element until first element will be NULL
           DLL_DeleteFirst(1);
        }
    } else if (num == 2) {
        while (listWhileLabels->firstElement != NULL)
        { // removing first element until first element will be NULL
           DLL_DeleteFirst(2);
        }   
    }
}

void DLL_InsertFirst(int num, char *data)
{
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    
    if (new_element == NULL)
    {
        exit(99);
        return;
    }
    if (num == 0)
    {
        if (data != NULL) {
            new_element->data = malloc(sizeof(char)*(strlen(data)+1));
            if (new_element->data == NULL) {
                exit(99);
            } 
            strcpy(new_element->data, data);
        }
        new_element->nextElement = listCodeGen->firstElement;
        new_element->previousElement = NULL;
        if (listCodeGen->firstElement != NULL)
        { // if list isn't empty
            listCodeGen->firstElement->previousElement = new_element;
        }

        listCodeGen->firstElement = new_element;
        if (listCodeGen->lastElement == NULL)
        { // if list was empty -> we need to set last element too
            listCodeGen->lastElement = listCodeGen->firstElement;
        }
    }
    else if (num == 1)
    {
        if (data != NULL) {
            new_element->data = malloc(sizeof(char)*(strlen(data)+1));
            if (new_element->data == NULL) {
                exit(99);
            }
            strcpy(new_element->data, data);
        } 
        new_element->nextElement = listIfLabels->firstElement;
        new_element->previousElement = NULL;
        if (listIfLabels->firstElement != NULL)
        { // if list isn't empty
            listIfLabels->firstElement->previousElement = new_element;
        }

        listIfLabels->firstElement = new_element;
        if (listIfLabels->lastElement == NULL)
        { // if list was empty -> we need to set last element too
            listIfLabels->lastElement = listIfLabels->firstElement;
        }
    }
    else if(num ==2)
    {
        if (data != NULL) {
            new_element->data = malloc(sizeof(char)*(strlen(data)+1));
            if (new_element->data == NULL) {
                exit(99);
            } 
            strcpy(new_element->data, data);
        }
        new_element->nextElement = listWhileLabels->firstElement;
        new_element->previousElement = NULL;
        if (listWhileLabels->firstElement != NULL)
        { // if list isn't empty
            listWhileLabels->firstElement->previousElement = new_element;
        }

        listWhileLabels->firstElement = new_element;
        if (listWhileLabels->lastElement == NULL)
        { // if list was empty -> we need to set last element too
            listWhileLabels->lastElement = listWhileLabels->firstElement;
        }
    }
}
// todo zkontrolovat v zadani špatného formátu a navratove hodnoty
// function reads() : ?string
//todo je tu spravny argument 0? (ak nie tak zmenit aj inde)
void READS()
{
    addToString(2, "LABEL $readS \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarReadS\n");
    addToString(2, "POPS LF@VarReadS\n");
    addToString(2, "READ LF@VarReadS string\n");
    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// todo zkontrolovat v zadani špatného formátu a navratove hodnoty
// function readi() : ?int
void READI()
{
    addToString(2, "LABEL $readS \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarReadI\n");
    addToString(2, "POPS LF@VarReadI\n");
    addToString(2, "READ LF@VarReadI int\n");
    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// todo zkontrolovat v zadani špatného formátu a navratove hodnoty
// function readf() : ?float
void READF()
{
    addToString(2, "LABEL $readS\n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarReadF\n");
    addToString(2, "POPS LF@VarReadF\n");
    addToString(2, "READ LF@VarReadF float\n");
    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// todo WRITE - zkontrolovat v zadani zda WRITE vypisuje spravne "%a" nebo to ma byt '%a'
// todo function write ( term1 , term2 , …, term𝑛 ) : void
// todo osetrit zda kdyz dostanu bool tak co dal
// todo vypise write variable ???
void WRITE()
{
    addToString(2, "LABEL $write \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarWrite\n");
    addToString(2, "POPS LF@VarWrite\n");
    addToString(2, "DEFVAR LF@VarType\n");

    // zjistit typ a zapis do VarType
    addToString(2, "TYPE LF@VarType LF@VarWrite \n");

    // skoc podle hodnoty VarType
    addToString(2, "JUMPIFEQ $INT LF@VarType int\n");       // type == int
    addToString(2, "JUMPIFEQ $FLOAT LF@VarType float\n");   // type == float
    addToString(2, "JUMPIFEQ $STRING LF@VarType string\n"); // type == string
    addToString(2, "JUMPIFEQ $NULL LF@VarType null\n");     // type == NULL

    // int
    addToString(2, "LABEL $INT\n");
    addToString(2, "WRITE LF@VarWrite ”%d”\n");
    addToString(2, "JUMP $END\n");

    // float
    addToString(2, "LABEL $FLOAT \n");
    addToString(2, "WRITE LF@VarWrite ”%a”\n");
    addToString(2, "JUMP $END\n");

    // string
    addToString(2, "LABEL $STRING\n");
    addToString(2, "WRITE LF@VarWrite\n");
    addToString(2, "JUMP $END\n");

    // hodnota null dle tabulky 1.
    addToString(2, "LABEL $NULL\n");
    addToString(2, "WRITE "
                   "\n");
    addToString(2, "JUMP $END\n");

    // END
    addToString(2, "LABEL $END\n");

    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// function floatval(term) : float
// todo zkontrolovat zda je dobre convert a vypi
void FLOATVAL()
{
    addToString(2, "LABEL $FLOATVAL \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarFloatval\n");
    addToString(2, "POPS LF@VarFloatval\n");
    addToString(2, "INT2FLOAT LF@VarFloatval LF@VarFloatval\n"); // konvert na float
    addToString(2, "WRITE LF@VarWrite\n");                       // vypise na vystup

    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// function intval(term) : int
// todo zkontrolovat zda je dobre convert a vypi
void INTVAL()
{
    addToString(2, "LABEL $INTVAL \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarIntval\n");
    addToString(2, "POPS LF@VarIntval\n");
    addToString(2, "FLOAT2INT LF@VarIntval LF@VarIntval\n"); // konvert na int
    addToString(2, "WRITE LF@VarWrite\n");                   // vypise na vystup

    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// function strval(term) : string –
void STRVAL()
{
    addToString(2, "LABEL $STRVAL \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarStrval\n");
    addToString(2, "POPS LF@VarStrval\n");
    addToString(2, "DEFVAR LF@VarType\n");

    // zjistit typ a zapis do VarType
    addToString(2, "TYPE LF@VarType LF@VarWrite \n");
    addToString(2, "JUMPIFEQ $NULL LF@VarType null\n"); // type == null

    addToString(2, "WRITE LF@VarWrite\n"); // vypise na vystup
    addToString(2, "JUMP $END\n");

    // hodnota null dle tabulky 1.
    addToString(2, "LABEL $NULL\n");
    addToString(2, "WRITE "
                   "\n");
    addToString(2, "JUMP $END\n");

    // END
    addToString(2, "LABEL $END\n");

    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// function strlen(string $𝑠) : int
void STRLEN()
{
    addToString(2, "LABEL $STRLEN \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarStrlen\n");
    addToString(2, "POPS LF@VarStrlen\n");
    addToString(2, "DEFVAR LF@Length\n");
    addToString(2, "STRLEN LF@Length LF@VarStrlen\n");
    addToString(2, "WRITE LF@Length\n"); // vypise na vystup
    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

//• function substring(string $𝑠, int $𝑖, int $𝑗) : ?string –
void SUBSTRING()
{
}

// function ord(string $c) : int –
void ORD()
{
    addToString(2, "LABEL $ORD \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarOrd\n");
    addToString(2, "POPS LF@VarOrd\n");
    addToString(2, "DEFVAR LF@Length\n");
    addToString(2, "STRLEN LF@Length LF@VarOrd\n");

    addToString(2, "JUMPIFEQ int@0 LF@Length END\n"); // string je prazdny
    addToString(2, "DEFVAR LF@Ord\n");

    addToString(2, "GETCHAR LF@Ord LF@VarOrd int@0\n"); // ziskani prvniho znaku
    addToString(2, "WRITE LF@Ord\n");                   // vypise na vystup

    // END
    addToString(2, "LABEL $END\n");
    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// function chr(int $i) : string –
// todo doresit
void CHR()
{
    addToString(2, "LABEL $CHR \n");
    addToString(2, "CREATEFRAME\n");
    addToString(2, "PUSHFRAME\n");
    addToString(2, "DEFVAR LF@VarChr\n");
    addToString(2, "POPS LF@VarChr\n");
    addToString(2, "POPFRAME\n");
    addToString(2, "RETURN\n");
}

// MARTIN
//  NOT_DEFINED,  // initial type    --
//  VAR_ID,       // $variable       --      TODO check return == 1 and we have to push var to stack
//  SLASH,        // / (divide)      --
//  EOF_T,        // EOF             --
//  L_PAR,        // (               --
//  R_PAR,        // )               --      
//  L_CPAR,       // {               --
//  R_CPAR,       // }               --
//  SEMICOL,      // ;               --
//  PLUS,         // +               --
//  MINUS,        // -               --
//  DOT,          // .               --
//  MUL,          // *               --
//  EQ,           // =               --
//  INT,          // 123e-1          --
//  FLOAT,        // 1.32e+32        --
//  THREE_EQ,     // ===             --
//  LESS,         // <               --
//  MORE,         // >               --
//  LESS_EQ,      // <=              --
//  MORE_EQ,      // >=              --
//  NOT_EQ,       // !==             --
//  STRING,       // "string \x1F"   --

// MATEJ
//  FUNCTION,     // function        --DONE NOT TESTED
//  ID,           // write, reads..  --DONE NOT TESTED
//  COMMA,        // ,               --DONE CHECK IF ITS RIGHT  
//  COLON         // :               --DONE NOT TESTED ->only empty case nothing to be done here
//  IF,           // if              --DONE NOT TESTED
//  ELSE,         // else            --DONE NOT TESTED
//  INT_TYPE,     // int             --DONE NOT TESTED ->only empty case nothing to be done here
//  NULL_KEYWORD, // null            --DONE NOT TESTED added CASE next to VAR_ID
//  RETURN,       // return
//  STRING_TYPE,  // string          --DONE NOT TESTED ->only empty case nothing to be done here
//  FLOAT_TYPE,   // float           --DONE NOT TESTED ->only empty case nothing to be done here
//  VOID,         // void            --DONE NOT TESTED ->only empty case nothing to be done here
//  WHILE,        // while           --DONE NOT TESTED

// function add in UniqueName +1 its for purpose of not having same name of function and variable
void GetUniqueName()
{
    UniqueName+=1;
}

void GetUniqueVarName()
{
    UniqueVarName+=1;
}

int GetNumberOfDigets(){
    int n = UniqueName;
    int count = 0;
    do {
    n /= 10;
    ++count;
  } while (n != 0);

  return count;
}

void AddLForFG(int frameStr,int IAmInFunction){
    if (IAmInFunction)
    {
        addToString(frameStr, "LF@");
    }
    else{
        addToString(frameStr, "GF@");
    }
}


void addToString(int str, char *newStr)
{
    if (newStr != NULL)
    {
        if (str == 0)
        { // generatedString
            // creating space for new string
            if (generatedString == NULL)
            {
                generatedString = calloc(strlen(newStr) + 1, sizeof(char));
            }
            else
            {
                generatedString = realloc(generatedString, (strlen(generatedString) + strlen(newStr) + 1) * sizeof(char));
            }

            if (generatedString == NULL)
            {
                exit(99);
            }
            strcat(generatedString, newStr);
        }
        else if (str == 1)
        { // inFunctionString
            // creating space for new string
            if (inFunctionString == NULL)
            {
                inFunctionString = calloc(strlen(newStr) + 1, sizeof(char));
            }
            else
            {
                inFunctionString = realloc(inFunctionString, (strlen(inFunctionString) + strlen(newStr) + 2) * sizeof(char));
            }

            if (inFunctionString == NULL)
            {
                exit(99);
            }
            strcat(inFunctionString, newStr);
        }
        else if (str == 2)
        { // allFunctionsString
            // creating space for new string
            if (allFunctionsString == NULL)
            {
                allFunctionsString = calloc(strlen(newStr) + 1, sizeof(char));
            }
            else
            {
                allFunctionsString = realloc(allFunctionsString, (strlen(allFunctionsString) + strlen(newStr) + 1) * sizeof(char));
            }

            if (allFunctionsString == NULL)
            {
                exit(99);
            }
            strcat(allFunctionsString, newStr);
        }
        else if (str == 3)
        { // listCodeGen->firstElement->data
            if (whileIfString == NULL)
            {
                whileIfString = calloc(strlen(newStr) + 1, sizeof(char));
            }
            else
            {
                whileIfString = realloc(whileIfString, (strlen(whileIfString) + strlen(newStr) + 1) * sizeof(char));
            }

            if (whileIfString == NULL)
            {
                exit(99);
            }
            strcat(whileIfString, newStr);
        }
    }
}

void store(char *val)
{
    // creating space for new item
    if (storage == NULL)
    {
        storage = malloc(sizeof(char *));
    }
    else
    {
        storage = realloc(storage, (storageLen + 1) * sizeof(char *));
    }

    if (storage == NULL)
    {
        exit(99);
    }

    storage[storageLen] = calloc(strlen(val) + 1, sizeof(char));
    if (storage[storageLen] == NULL)
    {
        exit(99);
    }

    strcpy(storage[storageLen], val);

    storageLen++;
}

void removeLastFromStorage()
{
    if (storageLen)
    {
        free(storage[storageLen - 1]);
        storage[storageLen - 1] = NULL;
        storageLen--;
    }
}

void addToOperator(enum type t)
{
    operator= t;
}

void removeOperator()
{
    operator= NOT_DEFINED;
}

void resetGlobalValues()
{
    removeOperator();
    while (storage != NULL)
    {
        removeLastFromStorage();
        if (storageLen == 0)
        {
            free(storage);
            storage = NULL;
        }
    }
    storageLen = 0;
    eqSymbolFound = 0;
}

void checkStorage()
{
    int frameStr = 0;
    char frame[] = " GF@";
    if (IAmInFunction)
    {
        frameStr = 1; // inFunctionString
        strcpy(frame, " LF@");
    }

    if (storageLen == 2)
    { // case $var = 1;
        addToString(frameStr, "MOVE");
        if (storage[0] != NULL && storage[0][0] == '-')
        { // if first letter is '-' -> it is variable
            addToString(frameStr, frame);
        }
        else
        {
            addToString(frameStr, " ");
        }
        addToString(frameStr, storage[0]);
        if (storage[1] != NULL && storage[1][0] == '-')
        { // if first letter is '-' -> it is variable
            addToString(frameStr, frame);
        }
        else
        {
            addToString(frameStr, " ");
        }
        addToString(frameStr, storage[1]);
        addToString(frameStr, "\n");

        removeLastFromStorage();
        removeLastFromStorage();
    } else if (storageLen == 1 && IAmInFunctionDeclaration) {
        addToString(frameStr, "PUSHS");
        if (storage[0] != NULL && storage[0][0] == '-')
        { // if first letter is '-' -> it is variable
            addToString(frameStr, frame);
        }
        else
        {
            addToString(frameStr, " ");
        }
        addToString(frameStr, storage[0]);
        addToString(frameStr, "\n");

        removeLastFromStorage();
    }

}

void threeAddress(int frameStr, char *frame)
{
    if (storage[0] != NULL && storage[0][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[0]);
    if (storage[1] != NULL && storage[1][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[1]);
    if (storage[2] != NULL && storage[2][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[2]);
    addToString(frameStr, "\n");

    removeLastFromStorage();
    removeLastFromStorage();
    removeOperator();
}

void threeAddressWithoutRemove(int frameStr, char *frame)
{
    if (storage[0] != NULL && storage[0][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[0]);
    if (storage[1] != NULL && storage[1][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[1]);
    if (storage[2] != NULL && storage[2][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[2]);
    addToString(frameStr, "\n");
}

void pushStorage(int frameStr, char *frame)
{
    addToString(frameStr, "PUSHS");
    if (storage[0] != NULL && storage[0][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[0]);
    addToString(frameStr, "\n");
    addToString(frameStr, "PUSHS");
    if (storage[1] != NULL && storage[1][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[1]);
    addToString(frameStr, "\n");

    removeLastFromStorage();
    removeLastFromStorage();
    removeOperator();
}

void pushWithoutDeleting(int frameStr, char *frame)
{
    addToString(frameStr, "PUSHS");
    if (storage[0] != NULL && storage[0][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[0]);
    addToString(frameStr, "\n");
    addToString(frameStr, "PUSHS");
    if (storage[1] != NULL && storage[1][0] == '-')
    { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    }
    else
    {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[1]);
    addToString(frameStr, "\n");
}

void randStr(char *dest, size_t length)
{
    char charset[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0)
    {
        size_t index = (double)rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

void convertToSameType(int frameStr, char *frame)
{
    char tmp1[11];
    char tmp2[11];

    char neqLabel[11];
    char checkTypeLabel[11];
    char checkTypeEnd[11];

    char neqEqLabel[11];
    char neqNeqLabel[11];

    randStr(tmp1, 10);
    randStr(tmp2, 10);
    randStr(neqLabel, 10);
    randStr(neqEqLabel, 10);
    randStr(neqNeqLabel, 10);
    randStr(checkTypeLabel, 10);
    randStr(checkTypeEnd, 10);

    addToString(frameStr, "LABEL ");
    addToString(frameStr, checkTypeLabel);
    addToString(frameStr, "\n");
    addToString(frameStr, "DEFVAR ");
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    addToString(frameStr, "\n");

    addToString(frameStr, "DEFVAR ");
    addToString(frameStr, frame);
    addToString(frameStr, tmp2);
    addToString(frameStr, "\n");

    addToString(frameStr, "TYPE ");
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    if (!strstr(storage[storageLen - 1], "@")) {
        addToString(frameStr, frame);
    }
    addToString(frameStr, storage[storageLen - 1]);
    addToString(frameStr, "\n");

    addToString(frameStr, "TYPE ");
    addToString(frameStr, frame);
    addToString(frameStr, tmp2);
    if (!strstr(storage[storageLen - 2], "@")) {
        addToString(frameStr, frame);
    }
    addToString(frameStr, storage[storageLen - 2]);
    addToString(frameStr, "\n");

    addToString(frameStr, "JUMPIFNEQ ");
    addToString(frameStr, neqLabel);
    addToString(frameStr, frame);
    addToString(frameStr, tmp2);
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    addToString(frameStr, "\n");

    addToString(frameStr, "JUMP ");
    addToString(frameStr, checkTypeEnd);
    addToString(frameStr, "\n");

    addToString(frameStr, "LABEL "); // neq
    addToString(frameStr, neqLabel);
    addToString(frameStr, "\n");
    addToString(frameStr, "JUMPIFEQ ");
    addToString(frameStr, neqEqLabel); // create
    addToString(frameStr, " string@float");
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    addToString(frameStr, "\n");

    addToString(frameStr, "JUMPIFNEQ ");
    addToString(frameStr, neqNeqLabel); // create
    addToString(frameStr, " string@float");
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    addToString(frameStr, "\n");

    addToString(frameStr, "LABEL "); // neqEq
    addToString(frameStr, neqEqLabel);
    addToString(frameStr, "\n");
    if (!strstr(storage[storageLen - 1], "@")) {
        addToString(frameStr, "INT2FLOAT ");
        addToString(frameStr, frame);
        addToString(frameStr, storage[storageLen - 1]);
        if (!strstr(storage[storageLen - 1], "@")) {
            addToString(frameStr, frame);
        }
        addToString(frameStr, storage[storageLen - 1]);
        addToString(frameStr, "\n");
    } 


    addToString(frameStr, "JUMP ");
    addToString(frameStr, checkTypeEnd);
    addToString(frameStr, "\n");

    addToString(frameStr, "LABEL "); // neqNeq
    addToString(frameStr, neqNeqLabel);
    addToString(frameStr, "\n");
    if (!strstr(storage[storageLen - 2], "@")) {
        addToString(frameStr, "INT2FLOAT ");
        addToString(frameStr, frame);
        addToString(frameStr, storage[storageLen - 2]);
        if (!strstr(storage[storageLen - 2], "@")) {
            addToString(frameStr, frame);
        }
        addToString(frameStr, storage[storageLen - 2]);
        addToString(frameStr, "\n");
    } 

    addToString(frameStr, "JUMP ");
    addToString(frameStr, checkTypeEnd);
    addToString(frameStr, "\n");

    addToString(frameStr, "LABEL ");
    addToString(frameStr, checkTypeEnd);
    addToString(frameStr, "\n");
}

void divIdiv(int frameStr, char *frame)
{
    char tmp1[11];
    char endLabel[11];
    char startLabel[11];
    char eqLabel[11];
    char neqLabel[11];

    randStr(tmp1, 10);
    randStr(startLabel, 10);
    randStr(endLabel, 10);
    randStr(eqLabel, 10);

    addToString(frameStr, "LABEL "); // start
    addToString(frameStr, startLabel);
    addToString(frameStr, "\n");
    addToString(frameStr, "DEFVAR ");
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    addToString(frameStr, "\n");

    addToString(frameStr, "TYPE ");
    addToString(frameStr, frame);
    addToString(frameStr, tmp1); // result in tmp1
    if (!strstr(storage[storageLen - 1], "@")) {
        addToString(frameStr, frame);
    }
    addToString(frameStr, storage[storageLen - 1]);
    addToString(frameStr, "\n");

    addToString(frameStr, "JUMPIFEQ ");
    addToString(frameStr, eqLabel);
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    addToString(frameStr, " string@float");
    addToString(frameStr, "\n");

    addToString(frameStr, "JUMPIFNEQ ");
    addToString(frameStr, neqLabel);
    addToString(frameStr, frame);
    addToString(frameStr, tmp1);
    addToString(frameStr, " string@float");
    addToString(frameStr, "\n");

    addToString(frameStr, "LABEL "); // eq
    addToString(frameStr, eqLabel);
    addToString(frameStr, "\n");

    if (eqSymbolFound)
    {
        addToString(frameStr, "DIV");
        threeAddress(frameStr, frame);
    }
    else
    {
        pushStorage(frameStr, frame);
        addToString(frameStr, "DIVS\n");
        addToString(frameStr, "PUSHS int@0\n");
        addToString(frameStr, "LTS\n");
    }

    addToString(frameStr, "JUMP ");
    addToString(frameStr, endLabel);
    addToString(frameStr, "\n");

    addToString(frameStr, "LABEL "); // neq
    addToString(frameStr, eqLabel);
    addToString(frameStr, "\n");

    if (eqSymbolFound)
    {
        addToString(frameStr, "IDIV");
        threeAddressWithoutRemove(frameStr, frame);
    }
    else
    {
        pushStorage(frameStr, frame);
        addToString(frameStr, "IDIVS\n");
        addToString(frameStr, "PUSHS int@0\n");
        addToString(frameStr, "LTS\n");
    }

    addToString(frameStr, "JUMP ");
    addToString(frameStr, endLabel);
    addToString(frameStr, "\n");

    addToString(frameStr, "LABEL "); // end
    addToString(frameStr, endLabel);
    addToString(frameStr, "\n");
}

void codeGeneration(Token *token)
{
    // if prolog not added
    if (allFunctionsString == NULL) {
        addToString(2, ".IFJcode22\nJUMP $main\n");
    }

    char TmpWhileAndIf[256];
    int NumberOfDigets=0;
    char *WhileNames =NULL;//purpose->to store the name of the while labels to list
    int defined = 0;  // auxiliary variable to know if variable was defined or not
    int frameStr = 0; // generatedString
    char frame[5];    // string of actual frame LF/GF/TF

    double strFloat;
    char convertedFloatToHexa[100];
    int convertedLen;
    

    // variables used to create random names
    char randomVar1[11];
    char randomVar2[11];

    // checking if we are in function -> chaning frame
    if (IAmInFunction)
    {
        strcpy(frame, " LF@");
    }
    else
    {
        strcpy(frame, " GF@");
    }

    // auxiliary variable for declarating new variables
    char *tmp = calloc(strlen(token->val) + strlen("DEFVAR GF@") + 2, sizeof(char));
    strcpy(tmp, "DEFVAR");

    // auxiliary variable which will be send to threeAddress function as newStr
    char *var = calloc(strlen(token->val) + strlen(" string@") + 2, sizeof(char)); // max lenght which can occur

    switch (token->t)
    { // switch by token type
    case NOT_DEFINED:
        break;

    case EOF_T:
        // end of file -> we want to print generated code
        addToString(2, "LABEL $main\n");
        addToString(2, generatedString); // merge

        if (generatedString != NULL)
        {
            // free(generatedString);
        }
        printf("%s\n", allFunctionsString);
        free(var);
        free(tmp);
        exit(0);
        break;

    case VAR_ID:
    case INT:
    case FLOAT:
        if (IAmInFunction) {
            frameStr = 1;
        }

        if (token->t == FLOAT) {
            strFloat = atof(token->val);
            sprintf(convertedFloatToHexa, "%a", strFloat);
            convertedLen = strlen(convertedFloatToHexa);
            token->val = realloc(token->val, sizeof(char)*convertedLen);
            if (token->val == NULL) {
                exit(99);
            }
            strcpy(token->val, convertedFloatToHexa);
        }

        // setting prefix
        if (token->t == VAR_ID)
        { // variable -> GF@-var1
            strcpy(var, "-");
            strcat(var, token->val);
        }
        else if (token->t == INT)
        { // int -> int@-7
            strcpy(var, " int@");
            strcat(var, token->val);
        }
        else if (token->t == FLOAT)
        { // float -> float@3.42
            strcpy(var, " float@");
            strcat(var, token->val);
        }
        else
        {
            strcpy(var, token->val);
        }

        if (token->t == VAR_ID)
        { // token is variable -> setting up correct frame
            if (IAmInFunction)
            {
                strcat(tmp, " LF@");
                strcat(tmp, var);
                strcat(tmp, "\n");
                if (inFunctionString)
                {
                    if (strstr(inFunctionString, tmp))
                    { // checking if variable was defined
                        defined = 1;
                    }
                }
            }
            else
            {
                strcat(tmp, " GF@");
                strcat(tmp, var);
                strcat(tmp, "\n");
                if (generatedString)
                {
                    if (strstr(generatedString, tmp))
                    { // checking if variable was defined
                        defined = 1;
                    }
                }
            }
        }
        else
        {
            defined = 1; // not variable
        }

        // storing variable
        store(var);

        // variable wasn't defined -> define it
        if (!defined)
        {
            addToString(frameStr, "DEFVAR");
            addToString(frameStr, frame);
            addToString(frameStr, var);
            addToString(frameStr, "\n");
        }

            // if operator was set (it means that in storage we have at least 2 items)
        if (operator!= NOT_DEFINED)
        {
            convertToSameType(frameStr, frame);
            switch (operator)
            {
            case PLUS:
                if (eqSymbolFound)
                { // $var = $var1 + 2;
                    addToString(frameStr, "ADD");
                    threeAddress(frameStr, frame);
                }
                else
                { // $var1 + 2 (without '=' -> it means that we are for example in condition, so we store result to stack)
                    pushStorage(frameStr, frame);
                    addToString(frameStr, "ADDS\n");
                    addToString(frameStr, "PUSHS int@0\n");
                    addToString(frameStr, "LTS\n");

                    pushStorage(3, frame);
                    addToString(3, "ADDS\n");
                    addToString(3, "PUSHS int@0\n");
                    addToString(3, "LTS\n");
                }
                break;
            case MINUS:
                if (eqSymbolFound)
                {
                    addToString(frameStr, "SUB");
                    threeAddress(frameStr, frame);
                }
                else
                {
                    pushStorage(frameStr, frame);
                    addToString(frameStr, "SUBS\n");
                    addToString(frameStr, "PUSHS int@0\n");
                    addToString(frameStr, "LTS\n");

                    pushStorage(3, frame);
                    addToString(3, "SUBS\n");
                    addToString(3, "PUSHS int@0\n");
                    addToString(3, "LTS\n");
                }
                break;
            case SLASH:
                divIdiv(frameStr, frame);
                break;
            case MUL:
                if (eqSymbolFound)
                {
                    addToString(frameStr, "MUL");
                    threeAddress(frameStr, frame);
                }
                else
                {
                    pushStorage(frameStr, frame);
                    addToString(frameStr, "MULS\n");
                    addToString(frameStr, "PUSHS int@0\n");
                    addToString(frameStr, "LTS\n");

                    pushStorage(3, frame);
                    addToString(3, "MULS\n");
                    addToString(3, "PUSHS int@0\n");
                    addToString(3, "LTS\n");
                }
                break;
            case DOT:
                addToString(frameStr, "CONCAT");
                threeAddress(frameStr, frame);
                break;
            case THREE_EQ:
                if (eqSymbolFound)
                {
                    addToString(frameStr, "EQ");
                    threeAddress(frameStr, frame);
                }
                else
                {
                    pushStorage(frameStr, frame);
                    addToString(frameStr, "EQS\n");

                    pushStorage(3, frame);
                    addToString(3, "EQS\n");
                }
                break;
            case LESS:
                if (eqSymbolFound)
                {
                    addToString(frameStr, "LT");
                    threeAddress(frameStr, frame);
                }
                else
                {
                    pushStorage(frameStr, frame);
                    addToString(frameStr, "LTS\n");

                    pushStorage(3, frame);
                    addToString(3, "LTS\n");
                }
                break;
            case MORE:
                if (eqSymbolFound)
                {
                    addToString(frameStr, "GT");
                    threeAddress(frameStr, frame);
                }
                else
                {
                    pushStorage(frameStr, frame);
                    addToString(frameStr, "GTS\n");

                    pushStorage(3, frame);
                    addToString(3, "GTS\n");
                }
                break;
            case NOT_EQ:
                if (eqSymbolFound)
                {
                    addToString(frameStr, "EQ");
                    threeAddress(frameStr, frame);
                    addToString(frameStr, "NOT");
                    if (storage[0] != NULL && storage[0][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[0]);
                    if (storage[0] != NULL && storage[0][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[0]);
                    addToString(frameStr, "\n");
                }
                else
                {
                    pushStorage(frameStr, frame);
                    addToString(frameStr, "EQS\n");
                    addToString(frameStr, "NOTS\n");

                    pushStorage(3, frame);
                    addToString(3, "EQS\n");
                    addToString(3, "NOTS\n");
                }
                break;

            case LESS_EQ:
                randStr(randomVar1, 10);
                randStr(randomVar2, 10);

                addToString(frameStr, "DEFVAR");
                addToString(frameStr, frame);
                addToString(frameStr, randomVar1);
                addToString(frameStr, "\n");

                addToString(frameStr, "DEFVAR");
                addToString(frameStr, frame);
                addToString(frameStr, randomVar2);
                addToString(frameStr, "\n");

                if (eqSymbolFound)
                {
                    addToString(frameStr, "LT"); // LESS
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    if (storage[1] != NULL && storage[1][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[1]);
                    if (storage[2] != NULL && storage[2][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[2]);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "EQ"); // EQ
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    if (storage[1] != NULL && storage[1][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[1]);
                    if (storage[2] != NULL && storage[2][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[2]);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "OR"); // OR
                    if (storage[0] != NULL && storage[0][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[0]);
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    addToString(frameStr, "\n");

                    removeLastFromStorage();
                    removeLastFromStorage();
                    removeOperator();
                }
                else
                {
                    pushWithoutDeleting(frameStr, frame);

                    addToString(frameStr, "LTS\n"); // LESS
                    addToString(frameStr, "POPS");  // STORE
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    addToString(frameStr, "\n");

                    pushWithoutDeleting(frameStr, frame);

                    addToString(frameStr, "EQS\n"); // EQ
                    addToString(frameStr, "POPS");  // STORE
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "PUSHS");
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    addToString(frameStr, "\n");
                    addToString(frameStr, "PUSHS");
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "ORS\n"); // OR

                    //---
                    pushWithoutDeleting(3, frame);

                    addToString(3, "LTS\n"); // LESS
                    addToString(3, "POPS");  // STORE
                    addToString(3, frame);
                    addToString(3, randomVar1);
                    addToString(3, "\n");

                    pushWithoutDeleting(3, frame);

                    addToString(3, "EQS\n"); // EQ
                    addToString(3, "POPS");  // STORE
                    addToString(3, frame);
                    addToString(3, randomVar2);
                    addToString(3, "\n");

                    addToString(3, "PUSHS");
                    addToString(3, frame);
                    addToString(3, randomVar1);
                    addToString(3, "\n");
                    addToString(3, "PUSHS");
                    addToString(3, frame);
                    addToString(3, randomVar2);
                    addToString(3, "\n");

                    addToString(3, "ORS\n"); // OR

                    removeLastFromStorage();
                    removeLastFromStorage();
                    removeOperator();
                }
                break;
            case MORE_EQ:
                randStr(randomVar1, 10);
                randStr(randomVar2, 10);

                addToString(frameStr, "DEFVAR");
                addToString(frameStr, frame);
                addToString(frameStr, randomVar1);
                addToString(frameStr, "\n");

                addToString(frameStr, "DEFVAR");
                addToString(frameStr, frame);
                addToString(frameStr, randomVar2);
                addToString(frameStr, "\n");

                if (eqSymbolFound)
                {
                    addToString(frameStr, "GT"); // MORE
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    if (storage[1] != NULL && storage[1][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[1]);
                    if (storage[2] != NULL && storage[2][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[2]);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "EQ"); // EQ
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    if (storage[1] != NULL && storage[1][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[1]);
                    if (storage[2] != NULL && storage[2][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[2]);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "OR"); // OR
                    if (storage[0] != NULL && storage[0][0] == '-')
                    {
                        addToString(frameStr, frame);
                    }
                    else
                    {
                        addToString(frameStr, " ");
                    }
                    addToString(frameStr, storage[0]);
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    addToString(frameStr, "\n");

                    removeLastFromStorage();
                    removeLastFromStorage();
                    removeOperator();
                }
                else
                {
                    pushWithoutDeleting(frameStr, frame);

                    addToString(frameStr, "GTS\n"); // MORE
                    addToString(frameStr, "POPS");  // STORE
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    addToString(frameStr, "\n");

                    pushWithoutDeleting(frameStr, frame);

                    addToString(frameStr, "EQS\n"); // EQ
                    addToString(frameStr, "POPS");  // STORE
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "PUSHS");
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar1);
                    addToString(frameStr, "\n");
                    addToString(frameStr, "PUSHS");
                    addToString(frameStr, frame);
                    addToString(frameStr, randomVar2);
                    addToString(frameStr, "\n");

                    addToString(frameStr, "ORS\n"); // OR

                    // ---

                    pushWithoutDeleting(3, frame);

                    addToString(3, "GTS\n"); // MORE
                    addToString(3, "POPS");  // STORE
                    addToString(3, frame);
                    addToString(3, randomVar1);
                    addToString(3, "\n");

                    pushWithoutDeleting(3, frame);

                    addToString(3, "EQS\n"); // EQ
                    addToString(3, "POPS");  // STORE
                    addToString(3, frame);
                    addToString(3, randomVar2);
                    addToString(3, "\n");

                    addToString(3, "PUSHS");
                    addToString(3, frame);
                    addToString(3, randomVar1);
                    addToString(3, "\n");
                    addToString(3, "PUSHS");
                    addToString(3, frame);
                    addToString(3, randomVar2);
                    addToString(3, "\n");

                    addToString(3, "ORS\n"); // OR

                    removeLastFromStorage();
                    removeLastFromStorage();
                    removeOperator();
                }
                break;

            default:
                break;
            }
            removeOperator(); // removing operator
        }

        break;
        // end of case VAR_ID/INT/FLOAT

    case L_CPAR:
        cparCounter++;

        // if (inIf)
        // {
        //     addToString(frameStr, "DEFVAR ");
        //     if (IAmInFunction)
        //     {
        //         addToString(frameStr, "LF@ ");
        //     }else{
        //         addToString(frameStr, "GF@ ");
        //     }
        //     addToString(frameStr, "if");
        //     int RndStr=rand_str();
        //     addToString(frameStr, RndStr); // DEFVAR GF@ifRndStr | LF@ifRndStr
        //     addToString(frameStr, "\n");

        //     addToString(frameStr, "POP ");

        //                 addToString(frameStr, "DEFVAR ");
        //     if (IAmInFunction)
        //     {
        //         addToString(frameStr, "LF@ ");
        //     }else{
        //         addToString(frameStr, "GF@ ");
        //     }
        //     addToString(frameStr, "if");
        //     addToString(frameStr, RndStr); // POP GF@ifRndStr | LF@ifRndStr

        //     addToString(frameStr, "JUMPIFEQ ELSE");
        //     addToString(frameStr, RndStr); // JUMPIFEQ ELSERndStr
        //     addToString(frameStr, "true"); // JUMPIFEQ ELSERndStr true
        //     if (IAmInFunction)
        //     {
        //         addToString(frameStr, "LF@ ");
        //     }else{
        //         addToString(frameStr, "GF@ ");
        //     }

        //     addToString(frameStr, RndStr); // JUMPIFEQ ELSERndStr true GF@ifRndStr/LF@ifRndStr
        //     addToString(frameStr, "\n");
        // }

        break;

    case R_CPAR:

//----------ADD BY MATEJ---------------------
    if(inWhile !=0){
        inWhile -= 1;
        GetUniqueVarName();
        addToString(frameStr, "JUMP ");
        addToString(frameStr, listWhileLabels->firstElement->nextElement->nextElement->data);//JUMP WHILESTART UniqueName
        addToString(frameStr, "\n");

        addToString(frameStr, "LABEL $");
        addToString(frameStr, listWhileLabels->firstElement->nextElement->data);//LABEL $LOOPCOND UniqueName
        addToString(frameStr, "\n");

        //add from listCodeGen condition and delete it after
        if (listCodeGen->firstElement != NULL) {
            addToString(frameStr, listCodeGen->firstElement->data);//add condition
        }
            addToString(frameStr,"DEFVAR ");
            AddLForFG(frameStr,IAmInFunction);
            addToString(frameStr,"CONDVAR");
            sprintf(TmpWhileAndIf, "%d", UniqueVarName);
            addToString(frameStr, TmpWhileAndIf);// DEFVAR GF/LF@CONDVAR UniqueName
            addToString(frameStr, "\n");

        addToString(frameStr, "POPS ");
        AddLForFG(frameStr,IAmInFunction);
        addToString(frameStr,"CONDVAR");
        sprintf(TmpWhileAndIf, "%d", UniqueVarName);
        addToString(frameStr, TmpWhileAndIf);//POPS GF/LF@CONDVAR UniqueName
        addToString(frameStr, "\n");

        addToString(frameStr, "JUMPIFEQ ");
        addToString(frameStr, listWhileLabels->firstElement->data);
        AddLForFG(frameStr,IAmInFunction);
        addToString(frameStr, "CONDVAR");
        sprintf(TmpWhileAndIf, "%d", UniqueVarName);
        addToString(frameStr, TmpWhileAndIf);
        addToString(frameStr, " true");//JUMPIFEQ LOOPBODY UNIQUENAME true
        addToString(frameStr, "\n");

        DLL_DeleteFirst(0);
        DLL_DeleteFirst(2);
        DLL_DeleteFirst(2);
        DLL_DeleteFirst(2);
    }

    if(inIf !=0 && afterElse){//jsem za else vetvi
        inIf -= 1;
        GetUniqueVarName();

        addToString(frameStr, "JUMP $");
        addToString(frameStr, listIfLabels->firstElement->data);//LABEL AFTERELSE UniqueName
        addToString(frameStr, "\n");

        addToString(frameStr, "LABEL $");
        addToString(frameStr, listIfLabels->firstElement->nextElement->nextElement->data);//LABEL IFCOND UniqueName
        addToString(frameStr, "\n");

        // //add from listCodeGen condition and delete it after
        //add from listCodeGen condition and delete it after
        if (listCodeGen->firstElement != NULL) {
            addToString(frameStr, listCodeGen->firstElement->data);//add condition
        }
        addToString(frameStr,"DEFVAR ");
        AddLForFG(frameStr,IAmInFunction);
        addToString(frameStr,"CONDVAR");
        sprintf(TmpWhileAndIf, "%d", UniqueVarName);
        addToString(frameStr, TmpWhileAndIf);// DEFVAR GF/LF@CONDVAR UniqueName
        addToString(frameStr, "\n");

        addToString(frameStr, "POPS ");
        AddLForFG(frameStr,IAmInFunction);
        addToString(frameStr,"CONDVAR");
        sprintf(TmpWhileAndIf, "%d", UniqueVarName);
        addToString(frameStr, TmpWhileAndIf);//POPS GF/LF@CONDVAR UniqueName
        addToString(frameStr, "\n");

        addToString(frameStr, "JUMPIFEQ ");
        addToString(frameStr, listIfLabels->firstElement->nextElement->data);
        AddLForFG(frameStr,IAmInFunction);
        addToString(frameStr, "CONDVAR");
        sprintf(TmpWhileAndIf, "%d", UniqueVarName);
        addToString(frameStr, TmpWhileAndIf);
        addToString(frameStr, " true");//JUMPIFEQ STARTIF UNIQUENAME true
        addToString(frameStr, "\n");

        addToString(frameStr, "JUMP $");
        addToString(frameStr, listIfLabels->firstElement->nextElement->nextElement->nextElement->data);//LABEL ELSE UniqueName
        addToString(frameStr, "\n");

        addToString(frameStr, "LABEL $");
        addToString(frameStr, listIfLabels->firstElement->data);//LABEL AFTERELSE UniqueName
        addToString(frameStr, "\n");

        DLL_DeleteFirst(0);
        DLL_DeleteFirst(1);
        DLL_DeleteFirst(1);
        DLL_DeleteFirst(1);
        DLL_DeleteFirst(1);
        afterElse=false;

    }
//-------------------------------------------

        checkStorage();
        if (cparCounter)
        {
            cparCounter--;
        }
        if (!cparCounter && IAmInFunction)
        { // function ended
            if (IAmInFunctionDeclaration) {
                addToString(1, "POPFRAME\n");
                addToString(1, "RETURN\n");
            }

            IAmInFunction = 0;
            IAmInFunctionDeclaration = 0;
            addToString(2, inFunctionString);
            free(inFunctionString);
            inFunctionString = NULL;
        }
        else
        {
            // not function
            if (whileIfString != NULL)
            { // not else {}
                DLL_InsertFirst(0, NULL);
                listCodeGen->firstElement->data = malloc(sizeof(char) * (strlen(whileIfString) + 1));
                // vzdy bude null -> nastavene od insertFirst
                // if (listCodeGen->firstElement->data == NULL)
                // {
                //     exit(99);
                // }
                strcpy(listCodeGen->firstElement->data, whileIfString);
                free(whileIfString);
                whileIfString = NULL;
            }
        }
        resetGlobalValues();
        break;

    case L_PAR:
        lparCounter++;
        break;

    case R_PAR:
        // lparCounter == 1 -> because we can have R_PAR in a body of a function (recognizing body and params)
        if (IAmInFunctionDeclaration && lparCounter == 1) {
            for (int i = storageLen-1; i >= 0; i--) {
                addToString(1, "POPS");
                addToString(1, frame);
                addToString(1, storage[i]);
                addToString(1, "\n");

                removeLastFromStorage();
            }
        }

        if (storageLen == 1)
        { // if ($var1)
            addToString(frameStr, "PUSHS");
            if (storage[0][0] == '-')
            {
                if (callingFromGF && IAmInFunctionCall) {
                    addToString(frameStr, " GF@");
                } else {
                    addToString(frameStr, frame);
                }
            }
            else
            {
                addToString(frameStr, " ");
            }
            addToString(frameStr, storage[0]);
            addToString(frameStr, "\n");
            if (inIf || inWhile) {
                addToString(frameStr, "PUSHS int@0\n");
                addToString(frameStr, "LTS\n");
            }
            removeLastFromStorage();
        } if (storageLen == 2 && IAmInFunctionCall) {
            addToString(frameStr, "PUSHS ");
            if (storage[0][0] == '-')
            {
                if (callingFromGF) {
                    addToString(frameStr, " GF@");
                } else {
                    addToString(frameStr, frame);
                }
            }
            else
            {
                addToString(frameStr, " ");
            }
            addToString(frameStr, storage[storageLen-1]);
            addToString(frameStr, "\n");
            removeLastFromStorage();
        }

        checkStorage();

        if (IAmInFunctionCall)
        {
            addToString(frameStr, "CALL ");
            addToString(frameStr, functionName);
            addToString(frameStr, "\n");

            if (storage[0] != NULL) {
                addToString(frameStr, "POPS GF@");
                addToString(frameStr, storage[storageLen-1]);
                addToString(frameStr, "\n");
                removeLastFromStorage();
            }

            if (functionName != NULL)
            {
                free(functionName);
                functionName = NULL;
            }
        }

        resetGlobalValues();
        break;

    case SEMICOL:
        checkStorage();
        resetGlobalValues(); 
        break;
    case FUNCTION:
        IAmInFunctionDeclaration = 1;
        lparCounter = 0;
        break;
    case PLUS:
        addToOperator(PLUS);
        break;
    case MINUS:
        addToOperator(MINUS);
        break;
    case SLASH:
        addToOperator(SLASH);
        break;
    case MUL:
        addToOperator(MUL);
        break;
    case DOT:
        addToOperator(DOT);
        break;
    case EQ:
        eqSymbolFound = 1;
        break;
    case THREE_EQ:
        addToOperator(THREE_EQ);
        break;
    case LESS:
        addToOperator(LESS);
        break;
    case MORE:
        addToOperator(MORE);
        break;
    case NOT_EQ:
        addToOperator(NOT_EQ);
        break;
    case LESS_EQ:
        addToOperator(LESS_EQ);
        break;
    case MORE_EQ:
        addToOperator(MORE_EQ);
        break;
    case STRING:
        // string@"hello world"
        strcpy(var, " string@");
        strcat(var, token->val);
        store(var);

        if (operator== DOT)
        {
            addToString(frameStr, "CONCAT");
            threeAddress(frameStr, frame);
            removeOperator();
        }

        break;

    case ID:
        if (!strcmp(token->val, "reads")) {
            READS();
        } else if (!strcmp(token->val, "readi")) {
            READI();
        } else if (!strcmp(token->val, "readf")) {
            READF();
        } else if (!strcmp(token->val, "write")) {
            WRITE();
        } else if (!strcmp(token->val, "floatval")) {
            FLOATVAL();
        } else if (!strcmp(token->val, "intval")) {
            INTVAL();
        } else if (!strcmp(token->val, "strval")) {
            STRVAL();
        } else if (!strcmp(token->val, "strlen")) {
            STRLEN();
        } else if (!strcmp(token->val, "substring")) {
            SUBSTRING();
        } else if (!strcmp(token->val, "ord")) {
            ORD();
        } else if (!strcmp(token->val, "chr")) {
            CHR();
        }

        IAmInFunction = 1;
        if (functionName == NULL)
        {
            functionName = malloc(strlen(token->val) + 1);
            strcpy(functionName, token->val);
        }
        else
        {
            functionName = realloc(functionName, strlen(token->val) + 1);
            strcpy(functionName, token->val);
        }

        if (IAmInFunctionDeclaration) {
            addToString(1, "LABEL ");
            addToString(1, functionName);
            addToString(1, "\n");

            addToString(1, "CREATEFRAME\n");
            addToString(1, "PUSHFRAME\n");
        } else {
            IAmInFunctionCall = 1;
            if (frameStr == 0) {
                callingFromGF = 1;
            } else {
                callingFromGF = 0;
            }
        }

        break;

    case COMMA:
        if (!IAmInFunctionDeclaration) {
            addToString(frameStr, "PUSHS ");
            if (!strstr(storage[storageLen - 1], "@")) {
                if (callingFromGF) {
                    addToString(0, " GF@");
                } else {
                    AddLForFG(frameStr,IAmInFunction);
                }
            }
            addToString(frameStr, storage[storageLen - 1]);
            removeLastFromStorage();
            addToString(frameStr, "\n");
        }
        break;
    case COLON:
        break;
    case WHILE:
        inWhile +=1;
        GetUniqueName();//ziskani noveho jmena pro while
        //create char *string with name WHILESTART UniqueName
        NumberOfDigets=GetNumberOfDigets();
        WhileNames = malloc(sizeof(char) * (strlen("WHILESTART") + NumberOfDigets+1));
        strcpy(WhileNames, "WHILESTART");
        sprintf(TmpWhileAndIf, "%d", UniqueName);
        strcat(WhileNames, TmpWhileAndIf);
        
        //push string to DLL
        DLL_InsertFirst(2, WhileNames);
        free(WhileNames);

        //create char *string with name LOOPCOND UniqueName
        WhileNames = malloc(sizeof(char) * (strlen("LOOPCOND") + NumberOfDigets+1));
        strcpy(WhileNames, "LOOPCOND");
        sprintf(TmpWhileAndIf, "%d", UniqueName);
        strcat(WhileNames, TmpWhileAndIf);

        DLL_InsertFirst(2, WhileNames);
        free(WhileNames);
        
        //create char *string with name $LOOPBODY UniqueName
        WhileNames = malloc(sizeof(char) * (strlen("LOOPBODY") + NumberOfDigets+1));
        strcpy(WhileNames, "LOOPBODY");
        sprintf(TmpWhileAndIf, "%d", UniqueName);
        strcat(WhileNames, TmpWhileAndIf);

        DLL_InsertFirst(2, WhileNames);
        free(WhileNames);

        addToString(frameStr, "LABEL $");
        addToString(frameStr,listWhileLabels->firstElement->nextElement->nextElement->data);//LABEL $WHILESTARTNUM
        addToString(frameStr, "\n");

        addToString(frameStr, "JUMP ");
        addToString(frameStr,listWhileLabels->firstElement->nextElement->data);//JUMP LOOPCONDNUM
        addToString(frameStr, "\n");

        addToString(frameStr, "LABEL $");
        addToString(frameStr,listWhileLabels->firstElement->data);//LABEL $LOOPBODYNUM
        addToString(frameStr, "\n");

        break;
    case RETURN:
        Return = true;
        break;
    case IF:
        inIf +=1;
        GetUniqueName();//ziskani noveho jmena pro while

        //create char *string with name ELSE UniqueName
        NumberOfDigets=GetNumberOfDigets();
        WhileNames = malloc(sizeof(char) * (strlen("ELSE") + NumberOfDigets+1));
        strcpy(WhileNames, "ELSE");
        sprintf(TmpWhileAndIf, "%d", UniqueName);
        strcat(WhileNames, TmpWhileAndIf);

        DLL_InsertFirst(1, WhileNames);
        free(WhileNames);
        
        //create char *string with name IFCOND UniqueName
        NumberOfDigets=GetNumberOfDigets();
        WhileNames = malloc(sizeof(char) * (strlen("IFCOND") + NumberOfDigets+1));
        strcpy(WhileNames, "IFCOND");
        sprintf(TmpWhileAndIf, "%d", UniqueName);
        strcat(WhileNames, TmpWhileAndIf);

        //push string to DLL
        DLL_InsertFirst(1, WhileNames);
        free(WhileNames);

        //create char *string with name STARTIF UniqueName
        NumberOfDigets=GetNumberOfDigets();
        WhileNames = malloc(sizeof(char) * (strlen("STARTIF") + NumberOfDigets+1));
        strcpy(WhileNames, "STARTIF");
        sprintf(TmpWhileAndIf, "%d", UniqueName);
        strcat(WhileNames, TmpWhileAndIf);

        //push string to DLL
        DLL_InsertFirst(1, WhileNames);
        free(WhileNames);

        //create char *string with name AFTERELSE UniqueName
        NumberOfDigets=GetNumberOfDigets();
        WhileNames = malloc(sizeof(char) * (strlen("AFTERELSE") + NumberOfDigets+1));
        strcpy(WhileNames, "AFTERELSE");
        sprintf(TmpWhileAndIf, "%d", UniqueName);
        strcat(WhileNames, TmpWhileAndIf);

        //push string to DLL
        DLL_InsertFirst(1, WhileNames);
        free(WhileNames);

        addToString(frameStr, "JUMP ");
        addToString(frameStr, listIfLabels->firstElement->nextElement->nextElement->data);//JUMP IFCOND
        addToString(frameStr, "\n");

        addToString(frameStr, "LABEL $");
        addToString(frameStr, listIfLabels->firstElement->nextElement->data);//LABEL $STARTIF
        addToString(frameStr, "\n");

        break;
    case ELSE:
        addToString(frameStr, "JUMP ");
        addToString(frameStr, listIfLabels->firstElement->data);//JUMP AFTERELSE
        addToString(frameStr, "\n");

        addToString(frameStr, "LABEL ");
        addToString(frameStr, listIfLabels->firstElement->nextElement->nextElement->nextElement->data);
        addToString(frameStr, "\n");
        afterElse=true;
        break;

    default:
        break;
    }

    free(var);
    free(tmp);
}
