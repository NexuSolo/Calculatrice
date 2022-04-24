#include <stdlib.h>

typedef struct chiffre{
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct {
    char signe;
    size_t len;
    chiffre *premier;
    chiffre *dernier;
} unbounded_int;

static unbounded_int *init_unbounded_int();

static void add_chiffre_to_unbounded_int_at_end(unbounded_int *n, char c);

static void add_chiffre_to_unbounded_int_at_start(unbounded_int *n, char c);

static chiffre *char_to_chiffre(char c);

static unbounded_int string2unbounded_int(const char *e);

static unbounded_int ll2unbounded_int(long long i);

static unbounded_int unbounded_int_error = {.signe = '*', .len = 0, .premier = NULL, .dernier = NULL};