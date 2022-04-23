#include <stdlib.h>

typedef struct {
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

