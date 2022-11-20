#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <string.h>  // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "frames.h"

#define SYMTABLE_SIZE 100

enum VarType
{
    FLOAT_PARAM = 16,
    INT_PARAM = 19,
    STRING_PARAM = 22,
};

// Tabulka:
typedef struct htab htab_t; // typedef podle zadání

// Typy:
typedef const char *htab_key_t; // typ klíče
typedef int htab_value_t;       // typ hodnoty
typedef struct frame frame_t;
typedef struct function_param function_param_t;
typedef struct htab_function htab_function_t;
typedef struct htab_variable htab_variable_t;

extern htab_t *symTable;
// Dvojice dat v tabulce:
typedef struct htab_pair
{
    htab_key_t key; // klíč
    htab_function_t *function;
    htab_variable_t *variable;
} htab_pair_t; // typedef podle zadání

// definícia štruktúry pre položku v poli
typedef struct htab_item
{
    htab_pair_t *pair;
    struct htab_item *next;
} htab_item_t;

typedef struct function_param
{
    enum VarType t;
    char *name;
    bool canBeNull;
} function_param_t;

typedef struct htab_function
{
    function_param_t **params;
    char *name;
    int paramCount;
    function_param_t *returnType;
} htab_function_t;

typedef struct htab_variable
{
    char *name;
    Frame *frame;
    enum VarType t;
    bool canBeNull;
} htab_variable_t;

// definícia štruktúry pre položku v poli
struct htab
{
    size_t size;
    size_t arr_size;
    htab_item_t **ptr_arr;
};

// Rozptylovací (hash) funkce (stejná pro všechny tabulky v programu)
// Pokud si v programu definujete stejnou funkci, použije se ta vaše.
size_t htab_hash_function(htab_key_t str);

// Funkce pro práci s tabulkou:
htab_t *htab_init(size_t n);               // konstruktor tabulky
size_t htab_size(const htab_t *t);         // počet záznamů v tabulce
size_t htab_bucket_count(const htab_t *t); // velikost pole

htab_pair_t *htab_find(htab_t *t, htab_key_t key); // hledání
htab_pair_t *htab_lookup_add(htab_t *t, htab_key_t key);
htab_pair_t *htab_add_function(htab_t *t, htab_key_t name, function_param_t *returnType, function_param_t **params, int paramCount);
function_param_t *htab_add_parameter(htab_function_t *function);
function_param_t *htab_add_return_type(htab_function_t *function);
htab_pair_t *htab_add_variable(htab_t *t, htab_key_t name, frame_t *frame, enum VarType type);
htab_pair_t *htab_search(htab_t *t, htab_key_t key);
int htab_erase_function(htab_function_t *f, int paramCount);
int htab_erase_variable(htab_variable_t *v);

bool htab_erase(htab_t *t, htab_key_t key); // ruší zadaný záznam
void htab_print(htab_t *t);
void htab_clear(htab_t *t); // ruší všechny záznamy
void htab_free(htab_t *t);  // destruktor tabulky
#endif
