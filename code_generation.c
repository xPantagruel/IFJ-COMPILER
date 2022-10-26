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

/** 
 * Built-in function READS
 * 
 * 
 */
#define READS \
    "LABEL reads \n" \
    "PUSHFRAME \n" \
    "DEFVAR LF@_ret \n" \
    "READ LF@_ret string \n" \
    "POPFRAME \n" \
    "RETURN\n"

/**
 * Built-in function READI
 * 
 */
#define READI \
    "LABEL readi \n" \
    "PUSHFRAME \n" \
    "DEFVAR LF@_ret \n" \
    "READ LF@_ret int \n" \
    "POPFRAME \n" \
    "RETURN\n"
/**
 * Built-in function READF
 * 
 */
#define READF \
    "LABEL readf \n" \
    "PUSHFRAME \n" \
    "DEFVAR LF@_ret \n" \
    "READ LF@_ret float \n" \
    "POPFRAME \n" \
    "RETURN\n"

/**
 * Built-in function WRITE
 * 
 */
#define WRITE \
    "LABEL write \n" \
    "PUSHFRAME \n" \
    "DEFVAR LF@_arg \n" \
    "POPS LF@_arg \n" \
    "DEFVAR LF@_temp \n" \
    "DEFVAR LF@_i \n" \
    "MOVE LF@_i int@0 \n" \
    "LABEL WHILE \n" \
    "JUMPIFEQ EXIT LF@_i LF@_arg \n" \
    "POPS LF@_temp \n" \
    "WRITE LF@_temp \n" \ 
    "ADD LF@_i LF@_i int@1 \n" \ 
    "JUMP WHILE \n" \
    "LABEL EXIT \n" \
    "POPFRAME \n" \
    "RETURN\n"

/**
 * Built-in function FLOATVAL, INTVAL, STRVAL
 * 
 * Part of bonus task STRNUM, not necessary rn
 * 
 */

/**
 * Built-in function STRLEN
 * 
 */
#define STRLEN \
    "LABEL strlen \n" \
    "PUSHFRAME \n" \
    "DEFVAR LF@_ret \n" \
    "STRLEN LF@_ret LF@%0 \n" \
    "POPFRAME \n" \
    "RETURN\n"

/**
 * Built-in function SUBSTRING
 * 
 *
 */
#define SUBSTRING \
"LABEL substr \n" \
"PUSHFRAME \n" \
"DEFVAR LF@_ret \n" \
"MOVE LF@_ret string \n" \
"DEFVAR LF@_length_str \n" \
"CREATEFRAME \n" \
"DEFVAR TF%0 \n "\
"MOVE TF@%0 LF@%0 \n" \
"CALL $length \n" \
"MOVE LF@_length_str TF@%_ret \n" \
"DEFVAR LF@_ret_cond \n" \
"LT LF@_ret_cond LF@_length_str int@0 \n" \
"JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
"EQ LF@_ret_cond LF@_length_str int@0 \n" \
"JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
"LT LF@_ret_cond LF@%1 int@0 \n" \
"JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
"EQ LF@_ret_cond LF@%1 int@0 \n" \
"JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
"GT LF@_rt_cond LF@%1 LF@_length_str \n" \
"JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
"EQ LF@_ret_cond LF@%2 int@0 \n" \
"JUMPIFEQ substr$return LF@_ret_cond bool@true \n" \
"DEFVAR LF@_max_n \n" \
"MOVE LF@_max_n LF@_length_str \n" \
"SUB LF@_max_n LF@_max_n LF@%1 \n" \
"ADD LF@_max_n LF@_max_n int@1 \n" \
"DEFVAR LF@_edit_n_cond \n" \
"LT LF@_edit_n_cond LF@%2 int@0 \n" \
"JUMPIFEQ substr$edit_n LF@_edit_n_cond bool@true \n" \
"GT LF@_edit_n_cond LF@%2 LF@_max_n \n" \
"JUMPIFEQ substr$edit_n LF@_edit_n_cond bool@true \n" \
"JUMP substr$process \n" \
"LABEL substr$edit_n \n" \
"MOVE LF@%2 LF@_max_n \n" \
"LABEL substr$process \n" \
"DEFVAR LF@_index \n" \
"MOVE LF@_index LF@%1 \n" \
"SUB LF@_index LF@_index int@1 \n" \
"DEFVAR LF@_char \n" \
"MOVE LF@_process_loop_cond \n" \
"LABEL sbstr$process_loop \n" \
"GETCHAR LF@_char LF@%0 LF@_index \n" \
"CONCAT LF@%_retval LF@%_retval LF@_char \n" \
"ADD LF@_index LF@_index int@1 \n" \
"SUB LF@%2 LF@%2 int@1 \n" \
"GT LF@_process_loop_cond LF@%2 int@0 \n" \
"JUMPIFEQ substr$process_loop LF@_process_loop_cond bool@true \n" \
"LABEL substr$return \n" \
"POPFRAME \n" \
"RETURN \n" \


/**
 * Built-in function ORD
 * 
 */
#define ORD \
"LABEL ord \n" \
"PUSHFRAME \n" \
"DEFVAR LF@_retval \n" \
"MOVE LF@%_retval int@0 \n" \
"DEFVAR LF@_cond_length \n" \
"LT LF@_cond_length LF@%1 int@1 \n" \
"JUMPIFEQ ord$return LF@_cond_length bool@true \n" \
"DEFVAR LF@_length_str \n" \
"CREATEFRAME \n" \
"DEFVAR TF@%0 \n" \
"MOVE TF@%0 LF@%0 \n" \
"CALL length \n" \
"MOVE LF@_length_str TF@%_retval \n" \
"GT LF@_cond_length LF@%1 LF@_length_str \n" \
"JUMPIFEQ ord$return LF@_cond_length bool@true \n" \
"SUB LF@%1 LF@%1 int@1 \n" \
"STRI2INT LF@%_retval LF@%0 LF@%1 \n" \
"LABEL ord$return \n" \
"POPFRAME \n" \
"RETURN \n" \

/**
 * Built-in function CHR
 * 
 */
#define CHR \
"LABEL chr \n" \
"PUSHFRAME \n" \
"DEFVAR LF@%_ret \n" \
"MOVE LF@%_ret string@ \n" \
"DEFVAR LF@_cond_range \n" \
"LT LF@_cond_range LF@%0 int@0 \n" \
"JUMPIFEQ chr$return LF@_cond_range bool@true \n" \
"GT LF@_cond_range LF@%0 int@255 \n" \
"JUMPIFEQ chr$return LF@_cond_range bool@true \n" \
"INT2CHAR LF@%_ret LF@%0 \n" \
"LABEL chr$return \n" \
"POPFRRAME \n" \
"RETURN\n" \
