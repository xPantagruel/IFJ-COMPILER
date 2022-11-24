
#include "symtable.h"

htab_t *symTable;

// funkcia na vytvorenie hashu pre určitý kľúč
size_t htab_hash_function(htab_key_t str)
{
    uint32_t h = 0;
    const unsigned char *p;
    for (p = (const unsigned char *)str; *p != '\0'; p++)
        h = 65599 * h + *p;
    return h;
}

// funkcia na vytvorenie hashovacej tabuľky o veľkosti n
htab_t *htab_init(size_t n)
{
    htab_t *tab = malloc(sizeof(htab_t));
    if (tab == NULL)
    {
        return NULL;
    }
    tab->ptr_arr = malloc(n * sizeof(htab_item_t *));
    if (tab->ptr_arr == NULL)
    {
        return NULL;
    }
    else
    {
        tab->size = 0;
        tab->arr_size = n;
        for (size_t i = 0; i < n; i++)
        {
            tab->ptr_arr[i] = NULL;
        }
    }
    return tab;
}

// funkcia na zistenie veľkosti pola
size_t htab_bucket_count(const htab_t *t)
{
    return t->arr_size;
}

size_t htab_size(const htab_t *t)
{
    return t->size;
}

// funckia na nájdenie kľúča v tabuľke
htab_pair_t *htab_find(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);
    // hľadanie kľúča
    if (t->ptr_arr[index] != NULL)
    {
        for (htab_item_t *tmp = t->ptr_arr[index]; tmp != NULL; tmp = tmp->next)
        {
            if (!strcmp(tmp->pair->key, key))
            {
                return tmp->pair;
            }
        }
        // kľúč nenájdený
        return NULL;
        // kľúč nenájdený
    }
    else
    {
        return NULL;
    }
}

void htab_free(htab_t *t)
{
    htab_clear(t);
    free(t->ptr_arr);
    free(t);
}

// funkcia na vymazanie všetkých kľúčov
void htab_clear(htab_t *t)
{
    for (size_t i = 0; i < t->arr_size; i++)
    {
        while (t->ptr_arr[i] != NULL)
        {
            htab_erase(t, t->ptr_arr[i]->pair->key);
        }
    }
}

// funkcia na vymazanie konkrétneho kľúča
bool htab_erase(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);
    // ak existuje nejaký kľúč na indexe
    if (t->ptr_arr[index] != NULL)
    {
        int count = 0;
        // premenná na uchovávanie predošlej položky
        htab_item_t *previous;
        // prechod položkami
        for (htab_item_t *tmp = t->ptr_arr[index]; tmp != NULL; tmp = tmp->next)
        {
            // ak je kľúč nájdený
            if (!strcmp(tmp->pair->key, key))
            {
                // ak sa jedná o prvú položku
                if (count == 0)
                {

                    htab_item_t *store = t->ptr_arr[index];
                    t->ptr_arr[index] = tmp->next;
                    if (store->pair->function)
                    {
                        htab_erase_function(store->pair->function, store->pair->function->paramCount);
                    }
                    if (store->pair->variable)
                    {
                        htab_erase_variable(store->pair->variable);
                        store->pair->variable = NULL;
                    }

                    free((char *)store->pair->key);
                    free(store->pair);
                    free(store);
                }
                else
                {
                    // ak sa jedná o poslednú položku
                    if (tmp->next == NULL)
                    {
                        previous->next = NULL;

                        if (tmp->pair->function)
                        {
                            htab_erase_function(tmp->pair->function, tmp->pair->function->paramCount);
                        }
                        if (tmp->pair->variable)
                        {

                            htab_erase_variable(tmp->pair->variable);
                            tmp->pair->variable = NULL;
                        }
                        free((char *)tmp->pair->key);
                        free(tmp->pair);
                        free(tmp);
                        // ak sa jedná o položku v strede
                    }
                    else
                    {
                        htab_item_t *store = tmp;
                        previous->next = tmp->next;

                        if (store->pair->function)
                        {
                            htab_erase_function(store->pair->function, store->pair->function->paramCount);
                        }
                        if (store->pair->variable)
                        {
                            htab_erase_variable(store->pair->variable);
                            store->pair->variable = NULL;
                        }
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
    }
    else
    {
        return false;
    }
}

// funkcia na pridanie kľúča do tabuľky
htab_pair_t *htab_lookup_add(htab_t *t, htab_key_t key, Frame *frame)
{
    // zistenie indexu na ktorý bude kľúč uložený
    size_t index = htab_hash_function(key) % htab_bucket_count(t);

    char *str;

    // ak sa na indexe nenacháza žiaden uložený kľúč
    if (t->ptr_arr[index] == NULL)
    {
        t->ptr_arr[index] = calloc(1, sizeof(htab_item_t));
        if (t->ptr_arr[index] == NULL)
        {
            return NULL;
        }
        t->ptr_arr[index]->pair = calloc(1, sizeof(htab_pair_t));
        if (t->ptr_arr[index]->pair == NULL)
        {
            return NULL;
        }
        // skopirovanie kľúča
        str = malloc((strlen(key) + 1) * sizeof(char));
        if (str == NULL)
        {
            return NULL;
        }
        strcpy(str, key);
        str[strlen(key)] = '\0';
        t->ptr_arr[index]->pair->function = NULL;
        t->ptr_arr[index]->pair->variable = NULL;
        t->ptr_arr[index]->pair->key = str;
        t->size = t->size + 1;
        return t->ptr_arr[index]->pair;
        // ak sa na indexe náchádza uložený kľúč
    }
    else
    {
        for (htab_item_t *tmp = t->ptr_arr[index]; tmp != NULL; tmp = tmp->next)
        {
            // ak kľúč existuje v tabuľke
            if (!strcmp(tmp->pair->key, key))
            {
                if (frame)
                {
                    if (frame == tmp->pair->variable->frame)
                    {
                        return tmp->pair;
                    }
                }
                else
                {
                    return tmp->pair;
                }
                // ak kľúč neexistuje v tabuľke
            }
            else
            {
                if (tmp->next == NULL)
                {
                    tmp->next = malloc(sizeof(htab_item_t));
                    if (tmp->next == NULL)
                    {
                        return NULL;
                    }
                    tmp->next->pair = malloc(sizeof(htab_pair_t));
                    if (tmp->next->pair == NULL)
                    {
                        return NULL;
                    }
                    // skopírovanie kľúča
                    str = malloc(strlen(key) * sizeof(htab_key_t));
                    if (str == NULL)
                    {

                        return NULL;
                    }
                    strcpy(str, key);
                    str[strlen(key)] = '\0';
                    tmp->next->pair->key = str;
                    tmp->next->next = NULL;
                    t->size = t->size + 1;

                    return tmp->next->pair;
                }
            }
        }
    }

    return NULL;
}

htab_pair_t *htab_add_function(htab_t *t, htab_key_t name, function_param_t *returnType, function_param_t **params, int paramCount)
{
    htab_pair_t *pair = htab_lookup_add(t, name, NULL);
    if (pair->function || pair->variable)
    {
        return NULL;
    }

    pair->function = calloc(1, sizeof(htab_function_t));

    pair->function->name = calloc(strlen(name) + 1, sizeof(char));
    if (!pair->function->name)
    {
        return NULL;
    }

    strcpy(pair->function->name, name);
    pair->function->params = params;
    pair->function->paramCount = paramCount;
    pair->function->returnType = returnType;
    return pair;
}

int htab_erase_function(htab_function_t *f, int paramCount)
{
    for (int i = 0; i < paramCount; i++)
    {
        free(f->params[i]->name);
        free(f->params[i]);
    }

    free(f->name);
    free(f->returnType->name);
    free(f->returnType);
    f = NULL;
    return 0;
}

htab_pair_t *htab_add_variable(htab_t *t, htab_key_t name, Frame *frame, enum type type)
{

    htab_pair_t *pair = htab_lookup_add(t, name, peekFrame(frameStack));

    if (!pair)
    {
        exit(99);
    }

    pair->variable = calloc(1, sizeof(htab_variable_t));
    if (!pair->variable)
    {
        // ERROR(99)
        return NULL;
    }

    pair->variable->name = calloc(strlen(name) + 1, sizeof(char));
    if (!pair->variable->name)
    {
        // ERROR(99)
        return NULL;
    }

    strcpy(pair->variable->name, name);
    pair->variable->frame = frame;
    pair->variable->t = type;
    addVariableToFrame(frame, pair);
    return pair;
}

function_param_t *htab_add_parameter(htab_function_t *function)
{

    function->params = realloc(function->params, (function->paramCount + 1) * sizeof(function_param_t));

    function->params[function->paramCount] = calloc(1, sizeof(function_param_t));
    function->paramCount++;

    return function->params[function->paramCount - 1];
}

function_param_t *htab_add_return_type(htab_function_t *function)
{
    function->returnType = calloc(1, sizeof(function_param_t));
    return function->returnType;
}

int htab_erase_variable(htab_variable_t *v)
{

    free(v->name);
    free(v);
    return 1;
}

void htab_print_variable(htab_variable_t *var)
{
    if (var->name)
        printf("variable: %s\n", var->name);
    if (var->t != -1)
    {
        printf("type: %d\n", var->t);
    }
    if (var->frame && var->frame->name)
        printf("frame: %s\n", var->frame->name);
}
void htab_print_function(htab_function_t *func)
{
    printf("function: %s\n", func->name);
    if (func->paramCount > 0)
    {
        printf("params:\n");
    }

    for (int i = 0; i < func->paramCount; i++)
    {
        if (func->params[i]->name != NULL)
        {
            printf("\t•name: %s\n", func->params[i]->name);
        }

        printf("\t•type: %d\n", func->params[i]->t);
        printf("\t•can be null?: %d\n\n", func->params[i]->canBeNull);
    }
    if (func->returnType)
    {
        printf("return type: %d\n", func->returnType->t);
    }
}

htab_pair_t *htab_search(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);
    htab_item_t *tmp = t->ptr_arr[index];
    while (tmp)
    {
        if (strcmp(tmp->pair->key, key) == 0)
        {
            return tmp->pair;
        }
    }
    return NULL;
}

void htab_print(htab_t *t)
{
    for (int i = 0; i < SYMTABLE_SIZE; i++)
    {

        htab_item_t *item = t->ptr_arr[i];
        if (item)
        {
            printf("----------\n");
            while (item)
            {
                printf("key: %s\n", item->pair->key);
                if (item->pair->function)
                {
                    htab_print_function(item->pair->function);
                }
                if (item->pair->variable)
                {
                    htab_print_variable(item->pair->variable);
                }
                item = item->next;
            }
            printf("----------\n\n");
            {
                /* code */
            }
        }
    }
}

void addBuiltInToSymtable()
{
    // reads
    htab_pair_t *fun = htab_add_function(symTable, "reads", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->canBeNull = true;
    fun->function->returnType->t = STRING_PARAM;

    // readi
    fun = htab_add_function(symTable, "readi", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->canBeNull = true;
    fun->function->returnType->t = INT_PARAM;

    // readf
    fun = htab_add_function(symTable, "readf", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->canBeNull = true;
    fun->function->returnType->t = FLOAT_PARAM;

    // floatval
    fun = htab_add_function(symTable, "floatval", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->t = FLOAT_PARAM;
    function_param_t *param = htab_add_parameter(fun->function);
    fun->function->params[0]->name = calloc(strlen("term") + 1, sizeof(char));
    strcpy(fun->function->params[0]->name, "term");
    fun->function->params[0]->t = ANY;
    fun->function->params[0]->canBeNull = true;

    // intval
    fun = htab_add_function(symTable, "intval", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->t = INT_PARAM;
    param = htab_add_parameter(fun->function);
    fun->function->params[0]->name = calloc(strlen("term") + 1, sizeof(char));
    strcpy(fun->function->params[0]->name, "term");
    fun->function->params[0]->t = ANY;
    fun->function->params[0]->canBeNull = true;

    // stringval
    fun = htab_add_function(symTable, "stringval", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->t = STRING_PARAM;
    param = htab_add_parameter(fun->function);
    fun->function->params[0]->name = calloc(strlen("term") + 1, sizeof(char));
    strcpy(fun->function->params[0]->name, "term");
    fun->function->params[0]->t = ANY;
    fun->function->params[0]->canBeNull = true;

    // strlen
    fun = htab_add_function(symTable, "strlen", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->t = INT_PARAM;
    param = htab_add_parameter(fun->function);
    fun->function->params[0]->name = calloc(strlen("s") + 1, sizeof(char));
    strcpy(fun->function->params[0]->name, "s");
    fun->function->params[0]->t = STRING;

    // substring
    fun = htab_add_function(symTable, "substring", NULL, NULL, 0);
    if (!fun)
    {
        exit(99);
    }
    htab_add_return_type(fun->function);
    fun->function->returnType->t = STRING_PARAM;
    fun->function->returnType->canBeNull = true;
    param = htab_add_parameter(fun->function);
    fun->function->params[0]->name = calloc(strlen("s") + 1, sizeof(char));
    strcpy(fun->function->params[0]->name, "s");
    fun->function->params[0]->t = STRING;
    function_param_t *param2 = htab_add_parameter(fun->function);
    fun->function->params[0]->name = calloc(strlen("i") + 1, sizeof(char));
    strcpy(fun->function->params[0]->name, "i");
    fun->function->params[0]->t = INT;
    function_param_t *param3 = htab_add_parameter(fun->function);
    fun->function->params[0]->name = calloc(strlen("j") + 1, sizeof(char));
    strcpy(fun->function->params[0]->name, "j");
    fun->function->params[0]->t = INT;
}

// TODO ZMENIT htab_pair STRUKTURU ABY VYHOVOVALA
// TODO komentare
