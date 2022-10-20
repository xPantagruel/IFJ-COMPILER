#include "symtable.h"

//funkcia na vytvorenie hashu pre určitý kľúč
size_t htab_hash_function(htab_key_t str) {
    uint32_t h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}

//funkcia na vytvorenie hashovacej tabuľky o veľkosti n
htab_t *htab_init(size_t n) {
    htab_t *tab = malloc(sizeof(htab_t));
    if (tab == NULL) {
        return NULL;
    }
    tab->ptr_arr = malloc(n*sizeof(htab_item_t*));
    if (tab->ptr_arr == NULL) {
        return NULL;
    } else {
        tab->size = 0;
        tab->arr_size = n;
        for (size_t i = 0; i < n; i++) {
            tab->ptr_arr[i] = NULL;
        }
    }
    return tab;
} 

//funkcia na zistenie veľkosti pola
size_t htab_bucket_count(const htab_t * t) {
    return t->arr_size;
}

size_t htab_size(const htab_t * t) {
    return t->size;
}

//funckia na nájdenie kľúča v tabuľke
htab_pair_t *htab_find(htab_t *t, htab_key_t key) {
    size_t index = htab_hash_function(key) % htab_bucket_count(t);
    //hľadanie kľúča
    if (t->ptr_arr[index] != NULL) {
            for (htab_item_t *tmp = t->ptr_arr[index]; tmp != NULL; tmp = tmp->next) {
                if(!strcmp(tmp->pair->key, key)) {
                    return tmp->pair;
                }
            }
            //kľúč nenájdený
            return NULL;
    //kľúč nenájdený
    } else {
        return NULL;
    }
}

void htab_free(htab_t *t) {
    htab_clear(t);
    free(t->ptr_arr);
    free(t);
}

//funkcia na vymazanie všetkých kľúčov
void htab_clear(htab_t * t) {
    for (size_t i = 0 ; i < t->arr_size; i++) {
        while (t->ptr_arr[i] != NULL) {
            htab_erase(t, t->ptr_arr[i]->pair->key);
        }
    }
}

//funkcia na vymazanie konkrétneho kľúča
bool htab_erase(htab_t * t, htab_key_t key) {
    size_t index = htab_hash_function(key) % htab_bucket_count(t);
    //ak existuje nejaký kľúč na indexe
    if (t->ptr_arr[index] != NULL) {
        int count = 0;
        //premenná na uchovávanie predošlej položky
        htab_item_t *previous;
        //prechod položkami
        for (htab_item_t *tmp = t->ptr_arr[index]; tmp != NULL; tmp = tmp->next) {
            //ak je kľúč nájdený
            if (!strcmp(tmp->pair->key, key)) {
                //ak sa jedná o prvú položku
                if (count == 0) {
                    htab_item_t *store = t->ptr_arr[index];
                    t->ptr_arr[index] = tmp->next; 
                    free((char *)store->pair->key);
                    free(store->pair);
                    free(store);
                } else {
                    //ak sa jedná o poslednú položku
                    if (tmp->next == NULL) {
                        previous->next = NULL;
                        free((char *)tmp->pair->key);
                        free(tmp->pair);
                        free(tmp);
                    //ak sa jedná o položku v strede
                    } else {
                        htab_item_t *store = tmp;
                        previous->next = tmp->next;
                        free((char *)store->pair->key);
                        free(store->pair);
                        free(store);
                    }    
                }
                t->size = t->size - 1;
                return true;
            }
            count++;
            previous = tmp;
        }
        return false;
    } else {
        return false;
    }
}   

//funkcia na pridanie kľúča do tabuľky
htab_pair_t *htab_lookup_add(htab_t *t, htab_key_t key) {
    //zistenie indexu na ktorý bude kľúč uložený
    size_t index = htab_hash_function(key) % htab_bucket_count(t);

    char *str;

    //ak sa na indexe nenacháza žiaden uložený kľúč
    if (t->ptr_arr[index] == NULL) {
        t->ptr_arr[index] = malloc(sizeof(htab_item_t));
        if (t->ptr_arr[index] == NULL) {
            return NULL;
        }
        t->ptr_arr[index]->pair = malloc(sizeof(htab_pair_t));
        if (t->ptr_arr[index]->pair == NULL) {
            return NULL;
        }
        //skopirovanie kľúča
        str = malloc(strlen(key)*sizeof(htab_key_t));
        if (str == NULL) {
            return NULL;
        }
        strcpy(str, key);
        str[strlen(key)] = '\0';

        t->ptr_arr[index]->pair->key = str;
        t->ptr_arr[index]->pair->value = 0;
        t->ptr_arr[index]->next = NULL;
        t->size = t->size + 1;
        return t->ptr_arr[index]->pair;
    //ak sa na indexe náchádza uložený kľúč
    } else {
        for (htab_item_t *tmp = t->ptr_arr[index]; tmp != NULL; tmp = tmp->next) {
            //ak kľúč existuje v tabuľke
            if (!strcmp(tmp->pair->key, key)){
                return tmp->pair;
            //ak kľúč neexistuje v tabuľke
            } else {
                if (tmp->next == NULL) {
                    tmp->next = malloc(sizeof(htab_item_t));
                    if (tmp->next == NULL) {
                        return NULL;
                    }
                    tmp->next->pair = malloc(sizeof(htab_pair_t));
                    if (tmp->next->pair == NULL) {
                        return NULL;
                    }
                    //skopírovanie kľúča
                    str = malloc(strlen(key)*sizeof(htab_key_t));
                    if (str == NULL) {
                        return NULL;
                    }
                    strcpy(str, key);
                    str[strlen(key)] = '\0';
                    tmp->next->pair->key = str;
                    tmp->next->next = NULL;
                    tmp->next->pair->value = 0;
                    t->size = t->size + 1;
                    return tmp->next->pair;
                }
            }
        }
    }  
    return NULL;
}


//TODO ZMENIT htab_pair STRUKTURU ABY VYHOVOVALA
//TODO komentare
