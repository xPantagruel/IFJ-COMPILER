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
    addToString(0,"LABEL $readS \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarReadS\n");
    addToString(0,"READ LF@VarReadS string\n");
    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//todo zkontrolovat v zadani špatného formátu a navratove hodnoty
//function readi() : ?int
void READI(){
    addToString(0,"LABEL $readS \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarReadI\n");
    addToString(0,"READ LF@VarReadI int\n");
    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//todo zkontrolovat v zadani špatného formátu a navratove hodnoty
//function readf() : ?float
void READF(){
    addToString(0,"LABEL $readS\n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarReadF\n");
    addToString(0,"READ LF@VarReadF float\n");
    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//todo WRITE - zkontrolovat v zadani zda WRITE vypisuje spravne "%a" nebo to ma byt '%a'
//todo function write ( term1 , term2 , …, term𝑛 ) : void
//todo osetrit zda kdyz dostanu bool tak co dal 
//todo vypise write variable ???
void WRITE(){
    addToString(0,"LABEL $write \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarWrite\n");
    addToString(0,"DEFVAR LF@VarType\n");

    //zjistit typ a zapis do VarType
    addToString(0,"TYPE LF@VarType LF@VarWrite \n");

    //skoc podle hodnoty VarType
    addToString(0,"JUMPIFEQ $INT LF@VarType int\n");// type == int
    addToString(0,"JUMPIFEQ $FLOAT LF@VarType float\n");// type == float
    addToString(0,"JUMPIFEQ $STRING LF@VarType string\n");// type == string
    addToString(0,"JUMPIFEQ $NULL LF@VarType null\n");// type == NULL

    //int
    addToString(0,"LABEL $INT\n");
    addToString(0,"WRITE LF@VarWrite ”%d”\n");
    addToString(0,"JUMP $END\n");

    //float
    addToString(0,"LABEL $FLOAT \n");
    addToString(0,"WRITE LF@VarWrite ”%a”\n");
    addToString(0,"JUMP $END\n");

    //string 
    addToString(0,"LABEL $STRING\n");
    addToString(0,"WRITE LF@VarWrite\n");
    addToString(0,"JUMP $END\n");

    //hodnota null dle tabulky 1.
    addToString(0,"LABEL $NULL\n");
    addToString(0,"WRITE " "\n");
    addToString(0,"JUMP $END\n");

    //END
    addToString(0,"LABEL $END\n");

    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//function floatval(term) : float
//todo zkontrolovat zda je dobre convert a vypi
void FLOATVAL(){
    addToString(0,"LABEL $FLOATVAL \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarFloatval\n");
    addToString(0,"INT2FLOAT LF@VarFloatval LF@VarFloatval\n");//konvert na float
    addToString(0,"WRITE LF@VarWrite\n");//vypise na vystup
    
    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//function intval(term) : int
//todo zkontrolovat zda je dobre convert a vypi
void INTVAL(){
    addToString(0,"LABEL $INTVAL \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarIntval\n");
    addToString(0,"FLOAT2INT LF@VarIntval LF@VarIntval\n");//konvert na int
    addToString(0,"WRITE LF@VarWrite\n");//vypise na vystup
    
    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//function strval(term) : string –
void STRVAL(){
    addToString(0,"LABEL $STRVAL \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarStrval\n");
    addToString(0,"DEFVAR LF@VarType\n");

    //zjistit typ a zapis do VarType
    addToString(0,"TYPE LF@VarType LF@VarWrite \n");
    addToString(0,"JUMPIFEQ $NULL LF@VarType null\n");// type == null

    addToString(0,"WRITE LF@VarWrite\n");//vypise na vystup
    addToString(0,"JUMP $END\n");
    
    //hodnota null dle tabulky 1.
    addToString(0,"LABEL $NULL\n");
    addToString(0,"WRITE " "\n");
    addToString(0,"JUMP $END\n");

    //END
    addToString(0,"LABEL $END\n");

    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//function strlen(string $𝑠) : int
void STRLEN(){
    addToString(0,"LABEL $STRLEN \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarStrlen\n");
    addToString(0,"DEFVAR LF@Length\n");
    addToString(0,"STRLEN LF@Length LF@VarStrlen\n");
    addToString(0,"WRITE LF@Length\n");//vypise na vystup
    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//• function substring(string $𝑠, int $𝑖, int $𝑗) : ?string –
void SUBSTRING(){

}

//function ord(string $c) : int –
void ORD(){
    addToString(0,"LABEL $ORD \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarOrd\n");
    addToString(0,"DEFVAR LF@Length\n");
    addToString(0,"STRLEN LF@Length LF@VarOrd\n");

    addToString(0,"JUMPIFEQ int@0 LF@Length END\n");// string je prazdny
    addToString(0,"DEFVAR LF@Ord\n");

    addToString(0,"GETCHAR LF@Ord LF@VarOrd int@0\n");//ziskani prvniho znaku
    addToString(0,"WRITE LF@Ord\n");//vypise na vystup

    //END
    addToString(0,"LABEL $END\n");
    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
}

//function chr(int $i) : string –
//todo doresit 
void CHR(){
    addToString(0,"LABEL $CHR \n");
    addToString(0,"CREATEFRAME\n");
    addToString(0,"PUSHFRAME\n");
    addToString(0,"DEFVAR LF@VarChr\n");


    addToString(0,"POPFRAME\n");
    addToString(0,"RETURN\n");
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
    // VAR_ID,       // $variable       -- 
    // SLASH,        // / (divide)      --
    // EOF_T,        // EOF             --
    // L_PAR,        // (               --
    // R_PAR,        // )               --
    // L_CPAR,       // {               --
    // R_CPAR,       // }               --
    // SEMICOL,      // ;               --
    // PLUS,         // +               --
    // MINUS,        // -               --
    // DOT,          // .               --
    // MUL,          // *               --
    // EQ,           // =               --
    // INT,          // 123e-1          -- //todo typ
    // FLOAT,        // 1.32e+32        -- //todo typ
    // THREE_EQ,     // ===             --
    // LESS,         // <               --
    // MORE,         // >               --
    // LESS_EQ,      // <=              --
    // MORE_EQ,      // >=              --
    // NOT_EQ,       // !==             --
    // STRING,       // "string \x1F"   -- //todo typ??

    //MATEJ
    // FUNCTION,     // function        
    // ID,           // write, reads..  --DONE NOT TESTED
    // COMMA,        // ,               --DONE NOT TESTED ->only empty case nothing to be done here
    // COLON         // :               --DONE NOT TESTED ->only empty case nothing to be done here
    // IF,           // if                  --FUCK UP if i have if else and nested if else is fucked up becouse i need to store somewhere names of labels
    // ELSE,         // else                --FUCK UP
    // INT_TYPE,     // int             --DONE NOT TESTED ->only empty case nothing to be done here
    // NULL_KEYWORD, // null            --DONE NOT TESTED added CASE next to VAR_ID
    // RETURN,       // return          
    // STRING_TYPE,  // string          --DONE NOT TESTED ->only empty case nothing to be done here
    // FLOAT_TYPE,   // float           --DONE NOT TESTED ->only empty case nothing to be done here
    // VOID,         // void            --DONE NOT TESTED ->only empty case nothing to be done here
    // WHILE,        // while


void addToString(int str, char *newStr)
{
    if (newStr != NULL) {
        if (str == 0) { // generatedString
            // creating space for new string
            if (generatedString == NULL) {
                generatedString = calloc(strlen(newStr) + 1, sizeof(char)); 
            } else {
                generatedString = realloc(generatedString, (strlen(generatedString) + strlen(newStr) + 1)*sizeof(char));
            }

            if (generatedString == NULL) {
                exit(99);
            }
            strcat(generatedString, newStr);

        } else if (str == 1) { // inFunctionString
            // creating space for new string
            if (inFunctionString == NULL) {
                inFunctionString = calloc(strlen(newStr) + 1, sizeof(char)); 
            } else {
                inFunctionString = realloc(inFunctionString, (strlen(inFunctionString) + strlen(newStr) + 1)*sizeof(char));
            }

            if (inFunctionString == NULL) {
                exit(99);
            }
            strcat(inFunctionString, newStr);

        } else if (str == 2){ // allFunctionsString
            // creating space for new string
            if (allFunctionsString == NULL) {
                allFunctionsString = calloc(strlen(newStr) + 1, sizeof(char)); 
            } else {
                allFunctionsString = realloc(allFunctionsString, (strlen(allFunctionsString) + strlen(newStr) + 1)*sizeof(char));
            }

            if (allFunctionsString == NULL) {
                exit(99);
            }
            strcat(allFunctionsString, newStr);
        }
    }
}

void store(char *val) {
    //creating space for new item
    if (storage == NULL) {
        storage = malloc(sizeof(char*));
    } else {
        storage = realloc(storage, (storageLen+1)*sizeof(char*));
    }

    if (storage == NULL) {
        exit(99);
    }

    storage[storageLen] = calloc(strlen(val)+1, sizeof(char));
    if (storage[storageLen] == NULL) {
        exit(99);
    }

    strcpy(storage[storageLen], val);

    storageLen++;
}

void removeLastFromStorage() {
    if (storageLen) {
        free(storage[storageLen-1]);
        storage[storageLen-1] = NULL;
        storageLen--;
    }
}

void addToOperator(enum type t) {
    operator = t;
}

void removeOperator() {
    operator = NOT_DEFINED;
}

void resetGlobalValues() {
    removeOperator();
    while (storage != NULL) {
        removeLastFromStorage();
        if (storageLen == 0) {
            free(storage);
            storage = NULL;
        }
    }
    storageLen = 0;
    eqSymbolFound = 0;
}

void checkStorage() {
    int frameStr = 0; 
    char frame[] = " GF@";
    if (IAmInFunction) {
        frameStr = 1; // inFunctionString
        strcpy(frame, " LF@");
    }

    if (storageLen == 2) { // case $var = 1;
        addToString(frameStr, "MOVE");
        if (storage[0] != NULL && storage[0][0] == '-') { // if first letter is '-' -> it is variable
            addToString(frameStr, frame);
        } else {
            addToString(frameStr, " ");
        }
        addToString(frameStr, storage[0]);
        if (storage[1] != NULL && storage[1][0] == '-') { // if first letter is '-' -> it is variable
            addToString(frameStr, frame);
        } else {
            addToString(frameStr, " ");
        }
        addToString(frameStr, storage[1]);
        addToString(frameStr, "\n");

        removeLastFromStorage();
        removeLastFromStorage();
    }
}

void threeAddress(int frameStr, char *frame) {
    if (storage[0] != NULL && storage[0][0] == '-') { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    } else {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[0]);
    if (storage[1] != NULL && storage[1][0] == '-') { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    } else {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[1]);
    if (storage[2] != NULL && storage[2][0] == '-') { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    } else {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[2]);
    addToString(frameStr, "\n");

    removeLastFromStorage();
    removeLastFromStorage();
    removeOperator();
} 

void pushStorage(int frameStr, char *frame) {
    addToString(frameStr, "PUSHS");
    if (storage[0] != NULL && storage[0][0] == '-') { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    } else {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[0]);
    addToString(frameStr, "\n");
    addToString(frameStr, "PUSHS");
    if (storage[1] != NULL && storage[1][0] == '-') { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    } else {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[1]);
    addToString(frameStr, "\n");
    
    removeLastFromStorage();
    removeLastFromStorage();
    removeOperator();
}

void pushWithoutDeleting(int frameStr, char *frame) {
    addToString(frameStr, "PUSHS");
    if (storage[0] != NULL && storage[0][0] == '-') { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    } else {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[0]);
    addToString(frameStr, "\n");
    addToString(frameStr, "PUSHS");
    if (storage[1] != NULL && storage[1][0] == '-') { // if first letter is '-' -> it is variable
        addToString(frameStr, frame);
    } else {
        addToString(frameStr, " ");
    }
    addToString(frameStr, storage[1]);
    addToString(frameStr, "\n");
}

void codeGeneration(Token *token) {
    int defined = 0; // auxiliary variable to know if variable was defined or not
    int frameStr = 0; // generatedString
    char frame[5]; // string of actual frame LF/GF/TF

    // checking if we are in function -> chaning frame
    if (IAmInFunction) {
        strcpy(frame, " LF@");
    } else {
        strcpy(frame, " GF@");
    }
    
    // auxiliary variable for declarating new variables
    char tmp[strlen(token->val) + strlen("DEFVAR GF@") + 1];
    strcpy(tmp, "DEFVAR");

    // auxiliary variable which will be send to threeAddress function as newStr
    char var[strlen(token->val) + strlen(" string@") + 1]; // max lenght which can occur

    switch (token->t) { // switch by token type
    case NOT_DEFINED:
        break;

    case EOF_T: 
        // end of file -> we want to print generated code
        addToString(2, generatedString); //merge
       
        if (generatedString != NULL) {
             //free(generatedString); 
        }

        printf("\n---\n"); //todo
        printf("%s\n", allFunctionsString);
        break;

    case VAR_ID: case INT: case FLOAT: case NULL_KEYWORD:
        //todo typy
        // setting prefix

        //dosiel mi parameter -> dam na stack 

        if (token->t == VAR_ID) { // variable -> GF@-var1
            strcpy(var, "-"); 
            strcat(var, token->val);
        } else if (token->t == INT) { // int -> int@-7
            strcpy(var, " int@"); 
            strcat(var, token->val);
        } else if (token->t == FLOAT) { // float -> float@3.42
            strcpy(var, " float@");
            strcat(var, token->val);
        } else {
            strcpy(var, token->val);
        }

        if (token->t == VAR_ID) { // token is variable -> setting up correct frame
            if (IAmInFunction) {
                strcat(tmp, " LF@");
                strcat(tmp, var);
                if (inFunctionString) {
                    if (strstr(inFunctionString, tmp)) { // checking if variable was defined 
                        defined = 1;
                    }
                }
                frameStr = 1;
            } else {
                strcat(tmp, " GF@");
                strcat(tmp, var);
                if (generatedString) {
                    if (strstr(generatedString, tmp)) { // checking if variable was defined 
                        defined = 1;
                    } 
                }
            }
        } else {
            defined = 1; // not variable
        }

        // storing variable
        store(var);

        // variable wasn't defined -> define it
        if (!defined) {
            addToString(frameStr, "DEFVAR");
            addToString(frameStr, frame);
            addToString(frameStr, var);
            addToString(frameStr, "\n");
        }

        // if operator was set (it means that in storage we have at least 2 items)
        if (operator != NOT_DEFINED) {
            switch(operator) {
                case PLUS:
                    if (eqSymbolFound) { // $var = $var1 + 2;
                        addToString(frameStr, "ADD");
                        threeAddress(frameStr, frame);
                    } else { // $var1 + 2 (without '=' -> it means that we are for example in condition, so we store result to stack)
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "ADDS\n");
                        addToString(frameStr, "PUSHS int@1\n");
                        addToString(frameStr, "ANDS\n");
                    }
                    break;
                case MINUS:
                    if (eqSymbolFound) {
                        addToString(frameStr, "SUB");
                        threeAddress(frameStr, frame);
                    } else {
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "SUBS\n");
                        addToString(frameStr, "PUSHS int@1\n");
                        addToString(frameStr, "ANDS\n");
                    }
                    break;
                case SLASH:
                    //todo idiv
                    if (eqSymbolFound) {
                        addToString(frameStr, "DIV");
                        threeAddress(frameStr, frame);
                    } else {
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "DIVS\n");
                        addToString(frameStr, "PUSHS int@1\n");
                        addToString(frameStr, "ANDS\n");
                    }
                    break;
                case MUL:
                    if (eqSymbolFound) {
                        addToString(frameStr, "MUL");
                        threeAddress(frameStr, frame);
                    } else {
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "MULS\n");
                        addToString(frameStr, "PUSHS int@1\n");
                        addToString(frameStr, "ANDS\n");
                    }
                    break;
                case DOT:
                    addToString(frameStr, "CONCAT");
                    threeAddress(frameStr, frame);
                    break;
                case THREE_EQ:
                    if (eqSymbolFound) {
                        addToString(frameStr, "EQ");
                        threeAddress(frameStr, frame);
                    } else {
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "EQS\n");  
                    }
                    break;
                case LESS:
                    if (eqSymbolFound) {
                        addToString(frameStr, "LT");
                        threeAddress(frameStr, frame);
                    } else {
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "LTS\n");  
                    }
                    break;
                case MORE:
                    if (eqSymbolFound) {
                        addToString(frameStr, "GT");
                        threeAddress(frameStr, frame);
                    } else {
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "GTS\n");  
                    }
                    break;
                case NOT_EQ:
                    if (eqSymbolFound) {
                        addToString(frameStr, "EQ");
                        threeAddress(frameStr, frame);
                        addToString(frameStr, "NOT");
                        if (storage[0] != NULL && storage[0][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[0]);
                        if (storage[0] != NULL && storage[0][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[0]);
                        addToString(frameStr, "\n");   

                    } else {
                        pushStorage(frameStr, frame);
                        addToString(frameStr, "EQS\n");
                        addToString(frameStr, "NOTS\n");  
                    }
                    break;

                case LESS_EQ:
                    addToString(frameStr, "DEFVAR");
                    addToString(frameStr, frame);
                    addToString(frameStr, "_aux1");
                    addToString(frameStr, "\n");

                    addToString(frameStr, "DEFVAR");
                    addToString(frameStr, frame);
                    addToString(frameStr, "_aux2");
                    addToString(frameStr, "\n");

                    if (eqSymbolFound) {
                        addToString(frameStr, "LT"); //LESS
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1");
                        if (storage[1] != NULL && storage[1][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[1]);
                        if (storage[2] != NULL && storage[2][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[2]);
                        addToString(frameStr, "\n");

                        addToString(frameStr, "EQ"); //EQ
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2");
                        if (storage[1] != NULL && storage[1][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[1]);
                        if (storage[2] != NULL && storage[2][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[2]);
                        addToString(frameStr, "\n");

                        addToString(frameStr, "OR"); //OR
                        if (storage[0] != NULL && storage[0][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[0]);
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1");
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2");
                        addToString(frameStr, "\n");
                                   
                        removeLastFromStorage();
                        removeLastFromStorage();
                        removeOperator();

                    } else {
                        pushWithoutDeleting(frameStr, frame);

                        addToString(frameStr, "LTS\n"); //LESS
                        addToString(frameStr, "POPS"); //STORE
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1\n");

                        pushWithoutDeleting(frameStr, frame);

                        addToString(frameStr, "EQS\n"); //EQ
                        addToString(frameStr, "POPS"); //STORE
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2\n");

                        addToString(frameStr, "PUSHS");
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1");
                        addToString(frameStr, "\n");
                        addToString(frameStr, "PUSHS");
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2");
                        addToString(frameStr, "\n");

                        addToString(frameStr, "ORS\n"); //OR

                        removeLastFromStorage();
                        removeLastFromStorage();
                        removeOperator();
                    }
                    break;
                case MORE_EQ:
                    addToString(frameStr, "DEFVAR");
                    addToString(frameStr, frame);
                    addToString(frameStr, "_aux1");
                    addToString(frameStr, "\n");

                    addToString(frameStr, "DEFVAR");
                    addToString(frameStr, frame);
                    addToString(frameStr, "_aux2");
                    addToString(frameStr, "\n");

                    if (eqSymbolFound) {
                        addToString(frameStr, "GT"); //MORE
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1");
                        if (storage[1] != NULL && storage[1][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[1]);
                        if (storage[2] != NULL && storage[2][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[2]);
                        addToString(frameStr, "\n");

                        addToString(frameStr, "EQ"); //EQ
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2");
                        if (storage[1] != NULL && storage[1][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[1]);
                        if (storage[2] != NULL && storage[2][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[2]);
                        addToString(frameStr, "\n");

                        addToString(frameStr, "OR"); //OR
                        if (storage[0] != NULL && storage[0][0] == '-') {
                            addToString(frameStr, frame);
                        } else {
                            addToString(frameStr, " ");
                        }
                        addToString(frameStr, storage[0]);
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1");
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2");
                        addToString(frameStr, "\n");
                                   
                        removeLastFromStorage();
                        removeLastFromStorage();
                        removeOperator();

                    } else {
                        pushWithoutDeleting(frameStr, frame);

                        addToString(frameStr, "GTS\n"); //MORE
                        addToString(frameStr, "POPS"); //STORE
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1\n");

                        pushWithoutDeleting(frameStr, frame);

                        addToString(frameStr, "EQS\n"); //EQ
                        addToString(frameStr, "POPS"); //STORE
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2\n");

                        addToString(frameStr, "PUSHS");
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux1");
                        addToString(frameStr, "\n");
                        addToString(frameStr, "PUSHS");
                        addToString(frameStr, frame);
                        addToString(frameStr, "_aux2");
                        addToString(frameStr, "\n");

                        addToString(frameStr, "ORS\n"); //OR
                        
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
        checkStorage();
        if (cparCounter) {
            cparCounter--;
        }
        if (!cparCounter && IAmInFunction) { // function declaration ended
            IAmInFunction = 0;
            addToString(2, inFunctionString);
            free(inFunctionString);
        }
        resetGlobalValues();
        break;

    case L_PAR:
        break;

    case R_PAR:
        checkStorage();

        if (storageLen == 1) { // if ($var1)
            addToString(frameStr, "PUSHS"); 
            if (storage[0][0] == '-') {
                addToString(frameStr, frame);
            } else {
                addToString(frameStr, " ");
            }
            addToString(frameStr, storage[0]);
            addToString(frameStr, "\n");
            addToString(frameStr, "PUSHS int@1\n");
            addToString(frameStr, "ANDS\n");
            removeLastFromStorage();
        }

        if(IAmInFunction){
            addToString(frameStr,"CALL $");
            addToString(frameStr, functionName);
            addToString(frameStr, "\n");
            
            if(functionName != NULL)
            {            
                free(functionName);
                functionName=NULL;
            }
        }

        resetGlobalValues();
        break;

    case SEMICOL:
        checkStorage();
        resetGlobalValues();
        break;

    case FUNCTION:
        IAmInFunction = 1;
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

        if (operator == DOT) {
            addToString(frameStr, "CONCAT");
            threeAddress(frameStr, frame);
            removeOperator();
        }

        break;

    case ID:
        if (IAmInFunction) {
            if (functionName == NULL) {
                functionName = malloc(strlen(token->val) + 1);
                strcpy(functionName, token->val);
            } else {
                functionName = realloc(functionName, strlen(token->val) + 1);
                strcpy(functionName, token->val);
            }
        }
        break;

    case COMMA:    
        break; 

    case COLON:
        break;

    case RETURN:
        addToString(frameStr, "PUSH\n");
        break;
    case IF:
        if(inIf == false)
        {
            inIf = true;
        }
        break;
    default:
        break;
    }
}

//todo remove todos
//todo escape seq -> niektore nemaju byt premenene
//todo prejst zadanie znova
//todo pridat do parsru volanie codeGeneration
//todo remove zo scanneru
//todo test an merlin + interpret
