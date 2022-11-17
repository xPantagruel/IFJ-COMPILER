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

//todo zkontrolovat v zadani špatného formátu a navratove hodnoty
//function reads() : ?string
void READS(){
    addToString(generatedString,"LABEL $readS \n");
    addToString(generatedString,"CREATEFRAME\n");
    addToString(generatedString,"PUSHFRAME\n");
    addToString(generatedString,"DEFVAR LF@VarReadS\n");
    addToString(generatedString,"READ LF@VarReadS string\n");
    addToString(generatedString,"POPFRAME\n");
    addToString(generatedString,"RETURN\n");
}

//todo zkontrolovat v zadani špatného formátu a navratove hodnoty
//function readi() : ?int
void READI(){
    addToString(generatedString,"LABEL $readS \n");
    addToString(generatedString,"CREATEFRAME\n");
    addToString(generatedString,"PUSHFRAME\n");
    addToString(generatedString,"DEFVAR LF@VarReadI\n");
    addToString(generatedString,"READ LF@VarReadI int\n");
    addToString(generatedString,"POPFRAME\n");
    addToString(generatedString,"RETURN\n");
}

//todo zkontrolovat v zadani špatného formátu a navratove hodnoty
//function readf() : ?float
void READF(){
    addToString(generatedString,"LABEL $readS\n");
    addToString(generatedString,"CREATEFRAME\n");
    addToString(generatedString,"PUSHFRAME\n");
    addToString(generatedString,"DEFVAR LF@VarReadF\n");
    addToString(generatedString,"READ LF@VarReadF float\n");
    addToString(generatedString,"POPFRAME\n");
    addToString(generatedString,"RETURN\n");
}

//todo WRITE - zkontrolovat v zadani zda WRITE vypisuje spravne "%a" nebo to ma byt '%a'
//todo function write ( term1 , term2 , …, term𝑛 ) : void
//todo osetrit zda kdyz dostanu bool tak co dal 
//todo vypise write variable ???
void WRITE(){
    addToString(generatedString,"LABEL $write \n");
    addToString(generatedString,"CREATEFRAME\n");
    addToString(generatedString,"PUSHFRAME\n");
    addToString(generatedString,"DEFVAR LF@VarWrite\n");
    addToString(generatedString,"DEFVAR LF@VarType\n");

    //zjistit typ a zapis do VarType
    addToString(generatedString,"TYPE LF@VarType LF@VarWrite \n");

    //skoc podle hodnoty VarType
    addToString(generatedString,"JUMPIFEQ $INT LF@VarType int\n");// type == int
    addToString(generatedString,"JUMPIFEQ $FLOAT LF@VarType float\n");// type == float
    addToString(generatedString,"JUMPIFEQ $STRING LF@VarType string\n");// type == string
    addToString(generatedString,"JUMPIFEQ $NULL LF@VarType null\n");// type == NULL

    //int
    addToString(generatedString,"LABEL $INT\n");
    addToString(generatedString,"WRITE LF@VarWrite ”%d”\n");
    addToString(generatedString,"JUMP $END\n");

    //float
    addToString(generatedString,"LABEL $FLOAT \n");
    addToString(generatedString,"WRITE LF@VarWrite ”%a”\n");
    addToString(generatedString,"JUMP $END\n");

    //string 
    addToString(generatedString,"LABEL $STRING\n");
    addToString(generatedString,"WRITE LF@VarWrite\n");
    addToString(generatedString,"JUMP $END\n");

    //hodnota null dle tabulky 1.
    addToString(generatedString,"LABEL $NULL\n");
    addToString(generatedString,"WRITE " "\n");
    addToString(generatedString,"JUMP $END\n");

    //END
    addToString(generatedString,"LABEL $END\n");

    addToString(generatedString,"POPFRAME\n");
    addToString(generatedString,"RETURN\n");
}

//function floatval(term) : float
//todo zkontrolovat zda je dobre convert a vypi
void FLOATVAL(){
    addToString(generatedString,"LABEL $FLOATVAL \n");
    addToString(generatedString,"CREATEFRAME\n");
    addToString(generatedString,"PUSHFRAME\n");
    addToString(generatedString,"DEFVAR LF@VarFloatval\n");
    addToString(generatedString,"INT2FLOAT LF@VarFloatval LF@VarFloatval\n");//konvert na float
    addToString(generatedString,"WRITE LF@VarWrite\n");//vypise na vystup
    
    addToString(generatedString,"POPFRAME\n");
    addToString(generatedString,"RETURN\n");
}

//function intval(term) : int
//todo zkontrolovat zda je dobre convert a vypi
void INTVAL(){
    addToString(generatedString,"LABEL $INTVAL \n");
    addToString(generatedString,"CREATEFRAME\n");
    addToString(generatedString,"PUSHFRAME\n");
    addToString(generatedString,"DEFVAR LF@VarIntval\n");
    addToString(generatedString,"FLOAT2INT LF@VarIntval LF@VarIntval\n");//konvert na int
    addToString(generatedString,"WRITE LF@VarWrite\n");//vypise na vystup
    
    addToString(generatedString,"POPFRAME\n");
    addToString(generatedString,"RETURN\n");
}


// /** 
//  * Built-in function READS
//  * 
//  * 
//  */
// #define READS \
//     "LABEL reads \n" \
//     "PUSHFRAME \n" \
//     "DEFVAR LF@_ret \n" \
//     "READ LF@_ret string \n" \
//     "POPFRAME \n" \
//     "RETURN\n"

// /**
//  * Built-in function READI
//  * 
//  */
// #define READI \
//     "LABEL readi \n" \
//     "PUSHFRAME \n" \
//     "DEFVAR LF@_ret \n" \
//     "READ LF@_ret int \n" \
//     "POPFRAME \n" \
//     "RETURN\n"
// /**
//  * Built-in function READF
//  * 
//  */
// #define READF \
//     "LABEL readf \n" \
//     "PUSHFRAME \n" \
//     "DEFVAR LF@_ret \n" \
//     "READ LF@_ret float \n" \
//     "POPFRAME \n" \
//     "RETURN\n"

// /**
//  * Built-in function WRITE
//  * 
//  */
// #define WRITE \
//     "LABEL write \n" \
//     "PUSHFRAME \n" \
//     "DEFVAR LF@_arg \n" \
//     "POPS LF@_arg \n" \
//     "DEFVAR LF@_temp \n" \
//     "DEFVAR LF@_i \n" \
//     "MOVE LF@_i int@0 \n" \
//     "LABEL WHILE \n" \
//     "JUMPIFEQ EXIT LF@_i LF@_arg \n" \
//     "POPS LF@_temp \n" \
//     "WRITE LF@_temp \n" \ 
//     "ADD LF@_i LF@_i int@1 \n" \ 
//     "JUMP WHILE \n" \
//     "LABEL EXIT \n" \
//     "POPFRAME \n" \
//     "RETURN\n"

// /**
//  * Built-in function FLOATVAL, INTVAL, STRVAL
//  * 
//  * Part of bonus task STRNUM, not necessary rn
//  * 
//  */

// /**
//  * Built-in function STRLEN
//  * 
//  */
// #define STRLEN \
//     "LABEL strlen \n" \
//     "PUSHFRAME \n" \
//     "DEFVAR LF@_ret \n" \
//     "STRLEN LF@_ret LF@%0 \n" \
//     "POPFRAME \n" \
//     "RETURN\n"

// /**
//  * Built-in function SUBSTRING
//  * 
//  *
//  */
// #define SUBSTRING \
// "LABEL substr \n" \
// "PUSHFRAME \n" \
// "DEFVAR LF@_ret \n" \
// "MOVE LF@_ret string \n" \
// "DEFVAR LF@_length_str \n" \
// "CREATEFRAME \n" \
// "DEFVAR TF%0 \n "\
// "MOVE TF@%0 LF@%0 \n" \
// "CALL $length \n" \
// "MOVE LF@_length_str TF@%_ret \n" \
// "DEFVAR LF@_ret_cond \n" \
// "LT LF@_ret_cond LF@_length_str int@0 \n" \
// "JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
// "EQ LF@_ret_cond LF@_length_str int@0 \n" \
// "JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
// "LT LF@_ret_cond LF@%1 int@0 \n" \
// "JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
// "EQ LF@_ret_cond LF@%1 int@0 \n" \
// "JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
// "GT LF@_rt_cond LF@%1 LF@_length_str \n" \
// "JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
// "EQ LF@_ret_cond LF@%2 int@0 \n" \
// "JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
// "DEFVAR LF@_max_n \n" \
// "MOVE LF@_max_n LF@_length_str \n" \
// "SUB LF@_max_n LF@_max_n LF@%1 \n" \
// "ADD LF@_max_n LF@_max_n int@1 \n" \
// "DEFVAR LF@_edit_n_cond \n" \
// "LT LF@_edit_n_cond LF@%2 int@0 \n" \
// "JUMPIFEQ substr$edit_n LF@_edit_n_cond bool@true \n" \
// "GT LF@_edit_n_cond LF@%2 LF@_max_n \n" \
// "JUMPIFEQ substr$edit_n LF@_edit_n_cond bool@true \n" \
// "JUMP substr$process \n" \
// "LABEL substr$edit_n \n" \
// "MOVE LF@%2 LF@_max_n \n" \
// "LABEL substr$process \n" \
// "DEFVAR LF@_index \n" \
// "MOVE LF@_index LF@%1 \n" \
// "SUB LF@_index LF@_index int@1 \n" \
// "DEFVAR LF@_char \n" \
// "MOVE LF@_process_loop_cond \n" \
// "LABEL sbstr$process_loop \n" \
// "GETCHAR LF@_char LF@%0 LF@_index \n" \
// "CONCAT LF@%_retval LF@%_retval LF@_char \n" \
// "ADD LF@_index LF@_index int@1 \n" \
// "SUB LF@%2 LF@%2 int@1 \n" \
// "GT LF@_process_loop_cond LF@%2 int@0 \n" \
// "JUMPIFEQ substr$process_loop LF@_process_loop_cond bool@true \n" \
// "LABEL substr$return \n" \
// "POPFRAME \n" \
// "RETURN \n" \


// /**
//  * Built-in function ORD
//  * 
//  */
// #define ORD \
// "LABEL ord \n" \
// "PUSHFRAME \n" \
// "DEFVAR LF@_retval \n" \
// "MOVE LF@%_retval int@0 \n" \
// "DEFVAR LF@_cond_length \n" \
// "LT LF@_cond_length LF@%1 int@1 \n" \
// "JUMPIFEQ ord$return LF@_cond_length bool@true \n" \
// "DEFVAR LF@_length_str \n" \
// "CREATEFRAME \n" \
// "DEFVAR TF@%0 \n" \
// "MOVE TF@%0 LF@%0 \n" \
// "CALL length \n" \
// "MOVE LF@_length_str TF@%_retval \n" \
// "GT LF@_cond_length LF@%1 LF@_length_str \n" \
// "JUMPIFEQ ord$return LF@_cond_length bool@true \n" \
// "SUB LF@%1 LF@%1 int@1 \n" \
// "STRI2INT LF@%_retval LF@%0 LF@%1 \n" \
// "LABEL ord$return \n" \
// "POPFRAME \n" \
// "RETURN \n" \

// /**
//  * Built-in function CHR
//  * 
//  */
// #define CHR \
// "LABEL chr \n" \
// "PUSHFRAME \n" \
// "DEFVAR LF@%_ret \n" \
// "MOVE LF@%_ret string@ \n" \
// "DEFVAR LF@_cond_range \n" \
// "LT LF@_cond_range LF@%0 int@0 \n" \
// "JUMPIFEQ chr$return LF@_cond_range bool@true \n" \
// "GT LF@_cond_range LF@%0 int@255 \n" \
// "JUMPIFEQ chr$return LF@_cond_range bool@true \n" \
// "INT2CHAR LF@%_ret LF@%0 \n" \
// "LABEL chr$return \n" \
// "POPFRAME \n" \
// "RETURN\n" \


    //MARTIN
    // NOT_DEFINED,  // initial type    --
    // VAR_ID,       // $variable
    // SLASH,        // / (divide)
    // EOF_T,        // EOF             --
    // L_PAR,        // (               --
    // R_PAR,        // )               --
    // L_CPAR,       // {               --
    // R_CPAR,       // }               --
    // SEMICOL,      // ;               --
    // COMMA,        // ,
    // PLUS,         // +
    // MINUS,        // -
    // DOT,          // .
    // MUL,          // *
    // EQ,           // =
    // ID,           // write, reads..

    //MATEJ
    // FUNCTION,     // function
    // IF,           // if
    // ELSE,         // else
    // INT_TYPE,     // int
    // NULL_KEYWORD, // null
    // RETURN,       // return
    // STRING_TYPE,  // string
    // FLOAT_TYPE,   // float
    // VOID,         // void
    // WHILE,        // while
    // INT,          // 123e-1
    // FLOAT,        // 1.32e+32
    // THREE_EQ,     // ===
    // LESS,         // <
    // MORE,         // >
    // LESS_EQ,      // <=
    // MORE_EQ,      // >=
    // NOT_EQ,       // !==
    // STRING,       // "string \x1F"
    // COLON         // :


void addToString(char *str, char *newStr)
{
    if (str == NULL) {
        str = calloc(strlen(newStr) + 1, sizeof(char)); 
    } else {
        str = realloc(str, (strlen(str) + strlen(newStr) + 1)*sizeof(char));
    }

    if (str == NULL) {
        exit(99);
    }
    strcat(str, newStr);
}

void store(char *val) {
    if (storage == NULL) {
        storage = malloc(sizeof(char*));
    } else {
        storage = realloc(storage, (storageLen+1)*sizeof(char*));
    }

    storage[storageLen] = calloc(strlen(val)+1, sizeof(char));
    strcpy(storage[storageLen], val);

    storageLen++;

    //todo if malloc fail
    //todo free -> jednotky a cele
}

void removeLastFromStorage() {
    free(storage[storageLen-1]);
    storage[storageLen-1] = NULL;
    storageLen--;
}

void addToOperator(char *op) {
    operator = calloc(strlen(op) + 1, sizeof(char));
    strcpy(operator, op);
}

void removeOperator() {
    if (operator != NULL) {
        free(operator);
    }
    operator = NULL;
}

void resetGlobalValues() {
    removeOperator();
    if (storage != NULL) {
        free(storage[storageLen]);
        free(storage);
        storage = NULL;
    }
    storageLen = 0;
}

void codeGeneration(Token *token) {
    int defined = 0;
    char *frameStr;

    switch (token->t) {
    case NOT_DEFINED:
        break;
    case EOF_T:
        break;
    case VAR_ID:
        //todo typy??, globalne premenne?
        if (IAmInFunction) {
            if (strstr(inFunctionString, token->val)) {
                defined = 1;
            } 
        } else {
            if (strstr(generatedString, token->val)) { // todo ked bude merge tak moze byt v fun ale v global nie definovane
                defined = 1;
            } 
        }

        store(token->val);

        if (!defined) {
            if (IAmInFunction) {
                addToString(inFunctionString, "DEFVAR ");
                addToString(inFunctionString, token->val);
            } else {
                addToString(generatedString, "DEFVAR ");
                addToString(generatedString, token->val);
            }
        }

        if (operator != NULL) {
            if (IAmInFunction) {
                frameStr = inFunctionString;
            } else {
                frameStr = generatedString;
            }

            //bottom up

            removeOperator();
        }


        //ak neni v stringu -> vytvor (podla toho ci som vo fun)
        //uloz
        // ak mam ulozeny operator -> vyprintuj podla operatora a vymaz posledne 2 -> vymaz operator

        //ak ; ) } tak vyresetuj tieto premenne - storage, operator, storagelen ($var = 2)

        //merge fun string a global ked koniec fun
        break;

    case L_CPAR:
        cparCounter++;
        break;

    case R_CPAR:
        if (cparCounter) {
            cparCounter--;
        }
        if (!cparCounter) {
            IAmInFunction = 0;
        }
        resetGlobalValues();
        break;

    case L_PAR:
        break;

    case R_PAR:
        resetGlobalValues();
        break;

    case SEMICOL:
    
        resetGlobalValues();
        break;

    case FUNCTION:
        IAmInFunction = 1;
        break;
    
    default:
        break;
    }
}

//todo remove todos
//todo komentare
//todo escape seq -> niektore nemaju byt premenene
//todo prejst zadanie znova
//todo pridat do parsru volanie codeGeneration
//todo free infunction string
