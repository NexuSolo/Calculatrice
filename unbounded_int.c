#include "unbounded_int.h"

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

unbounded_int init_unbounded_int() {
    return (unbounded_int) {.signe = '+', .len = 0, .premier = NULL, .dernier = NULL};
}

void add_char_to_unbounded_int_at_start(unbounded_int *n, char c) {
    chiffre *ch = char_to_chiffre(c);
    if (ch == NULL || n->signe == '*') {
        *n = unbounded_int_error;
        return;
    }
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

void add_char_to_unbounded_int_at_end(unbounded_int *n, char c) {
    chiffre *ch = char_to_chiffre(c);
    if (ch == NULL || n->signe == '*') {
        *n = unbounded_int_error;
        return;
    }
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
    if (res == NULL) {
        return NULL;
    }
    res->c = c;
    res->precedent = res->suivant = NULL;
    return res;
}

unbounded_int string2unbounded_int(const char *e) {
    unbounded_int res = init_unbounded_int();
    size_t i = 0;
    bool premier = true;
    if(e[i] == '+' || e[i] == '-') {
        res.signe = e[i];
        i++;
    }
    while (e[i] != '\0') {
        if (isdigit(e[i])) {
            if(!premier) {
                add_char_to_unbounded_int_at_end(&res, e[i]);
            }
            else if (e[i] != '0') {
                add_char_to_unbounded_int_at_end(&res, e[i]);
                premier = false;
            }
            else if (e[i + 1] == '\0') {
                add_char_to_unbounded_int_at_end(&res, e[i]);
            }
            i++;
        }
        else {
            return unbounded_int_error;
        }
    }
    return res;
}

unbounded_int ll2unbounded_int(long long i) {
    unbounded_int res = init_unbounded_int();
    if(i == 0) {
        add_char_to_unbounded_int_at_end(&res, '0');
        return res;
    }
    if(i < 0) {
        res.signe = '-';
        i = -i;
    }
    while(i > 0) {
        add_char_to_unbounded_int_at_start(&res, i % 10 + '0');
        i /= 10;
    }
    return res;
}

char *unbounded_int2string(unbounded_int n) {
    char *res = malloc(sizeof(char) * (n.len + 2));
    assert(res != NULL);
    if(n.premier == NULL || n.signe == '*') {
        res[0] = '0';
        res[1] = '\0';
        return res;
    }
    size_t i = 0;
    if(n.signe == '-') {
        if(n.premier->c != '0') {
            res[0] = n.signe;
            i++;
        }
    }
    chiffre *tmp = n.premier;
    while (tmp != NULL) {
        res[i] = tmp->c;
        tmp = tmp->suivant;
        i++;
    }
    res[i] = '\0';
    return res;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if (a.signe == b.signe) {
        if (a.len != b.len) {
            if(a.signe == '+') {
                return a.len < b.len ?-1:1;
            }
            return a.len < b.len ?1:-1;
        }
        chiffre *tmpa = a.premier;
        chiffre *tmpb = b.premier;
        for (int i = 0; i < a.len; i++) {
            if(tmpa->c != tmpb->c) {
                return tmpa->c < tmpb->c ?-1:1;
            }
            tmpa = tmpa->suivant;
            tmpb = tmpb->suivant;
        }
        return 0;
    }
    else {
        return a.signe == '+' ?1:-1;
    }
}

int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    return unbounded_int_cmp_unbounded_int(a,ll2unbounded_int(b));
}

int main(int argc, char const *argv[]) {
    unbounded_int n = string2unbounded_int("100");
    long long n2 = -10;
    printf("%d\n", unbounded_int_cmp_ll(n, n2));
    return 0;
}
