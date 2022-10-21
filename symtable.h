#include <string.h>     // size_t
#include <stdbool.h>    // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Tabulka:
typedef struct htab htab_t;     // typedef podle zadání

// Typy:
typedef const char * htab_key_t;        // typ klíče
typedef int htab_value_t;               // typ hodnoty

// Dvojice dat v tabulce:
typedef struct htab_pair { 
    htab_key_t    key;          // klíč
    htab_value_t  value;        // asociovaná hodnota
} htab_pair_t;                  // typedef podle zadání

//definícia štruktúry pre položku v poli
typedef struct htab_item {
    htab_pair_t *pair;
    struct htab_item *next;
} htab_item_t;

//definícia štruktúry pre položku v poli
struct htab {
    size_t size;
    size_t arr_size;
    htab_item_t **ptr_arr;
};

// Rozptylovací (hash) funkce (stejná pro všechny tabulky v programu)
// Pokud si v programu definujete stejnou funkci, použije se ta vaše.
size_t htab_hash_function(htab_key_t str);

// Funkce pro práci s tabulkou:
htab_t *htab_init(size_t n);                    // konstruktor tabulky
size_t htab_size(const htab_t * t);             // počet záznamů v tabulce
size_t htab_bucket_count(const htab_t * t);     // velikost pole

htab_pair_t * htab_find(htab_t * t, htab_key_t key);  // hledání
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key);

bool htab_erase(htab_t * t, htab_key_t key);    // ruší zadaný záznam

void htab_clear(htab_t * t);    // ruší všechny záznamy
void htab_free(htab_t * t);     // destruktor tabulky
