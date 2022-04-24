#include "unbounded_int.h"
#include <assert.h>
#include <stdio.h>
#include <ctype.h>

unbounded_int *init_unbounded_int() {
    unbounded_int *res = malloc(sizeof(unbounded_int));
    assert(res != NULL);
    // if(res == NULL) { return unbounded_int a = {.signe = '*', .premier = NULL, .dernier = NULL, .len = 0}; }
    res->premier = NULL;
    res->dernier = NULL;
    res->len = 0;
    res->signe = '+';
    return res;
}

void add_chiffre_to_unbounded_int_at_end(unbounded_int *n, char c) {
    chiffre *ch = char_to_chiffre(c);
    if(n->premier == NULL) {
        n->premier = n->dernier = ch;
        n->len = 1;
        return;
    }
    ch->precedent = n->dernier;
    n->dernier->suivant = ch;
    n->dernier = ch;
    n->len++;
}

chiffre *char_to_chiffre(char c) {
    chiffre *res = malloc(sizeof(chiffre));
    assert(res != NULL);
    res->c = c;
    res->precedent = res->suivant = NULL;
    return res;
}

unbounded_int string2unbounded_int(const char *e) {
    unbounded_int *res = init_unbounded_int();
    size_t i = 0;
    if(e[i] == '+' || e[i] == '-') {
        res->signe = e[i];
        i++;
    }
    while (e[i] != '\0') {
        if (!isdigit(e[i])) {
            
        }
        add_chiffre_to_unbounded_int_at_end(res, e[i]);
        i++;
    }
    return *res;
}

char *unbounded_int2string(unbounded_int n) {
    char *res = malloc(sizeof(char) * (n.len + 2));
    assert(res != NULL);
    if(n.premier == NULL || n.signe == '*') {
        res[0] = '0';
        res[1] = '\0';
        return res;
    }
    res[0] = n.signe;
    size_t i = 1;
    chiffre *tmp = n.premier;
    while (tmp != NULL) {
        res[i] = tmp->c;
        tmp = tmp->suivant;
        i++;
    }
    res[i] = '\0';
    return res;
}

void add_chiffre_to_unbounded_int_at_start(unbounded_int *n, char c) {
    chiffre *ch = char_to_chiffre(c);
    if(n->premier == NULL) {
        n->premier = n->dernier = ch;
        n->len = 1;
        return;
    }
    ch->suivant = n->premier;
    n->premier->precedent = ch;
    n->premier = ch;
    n->len++;
}

//prend un long long en parametre et renvoi un unbounded_int
unbounded_int ll2unbounded_int(long long i) {
    unbounded_int *res = init_unbounded_int();
    if(i < 0) {
        res->signe = '-';
        i = -i;
    }
    while(i > 0) {
        add_chiffre_to_unbounded_int_at_start(res, i % 10 + '0');
        i /= 10;
    }
    return *res;
}

int main(int argc, char const *argv[]) {
    unbounded_int t = unbounded_int_error;
    unbounded_int n = string2unbounded_int("+123456789");
    printf("%s\n", unbounded_int2string(n));
    return 0;
}
