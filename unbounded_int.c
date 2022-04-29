#include "unbounded_int.h"

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

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



unbounded_int verify_0_unbounded_int(unbounded_int a) {
    if(a.premier == NULL) {
        return a;
    }
    chiffre *tmp = a.premier;
    if(tmp->c == '0') {
        while (tmp->suivant != NULL) {
            if(tmp->c == '0') {
                a.premier = a.premier->suivant;
                free(a.premier->precedent);
                a.premier->precedent = NULL;
                a.len--;
            }
            else {
                return a;
            }
            tmp = a.premier;
        }
    }
    return a;
}

static chiffre *get_chiffre_at_unit(unbounded_int *n,int p) {
    if (p > n -> len || p < 0) return NULL;
    chiffre *tmp = n -> dernier;
    for (int i = 0 ; i < p; i++) {
        tmp = tmp -> precedent;
    }
    return tmp;

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
                if(a.signe == '+') {
                    return tmpa->c < tmpb->c ?-1:1;
                }
                return tmpa->c < tmpb->c ?1:-1;
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

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
    if (a.signe == '*' || b.signe == '*') return unbounded_int_error;
    if (b.signe != a.signe) {
        if(b.signe == '-') {
            unbounded_int b2 = b;
            b2.signe = '+';
            return unbounded_int_difference(a,b2);
        }
        unbounded_int a2 = a;
        a2.signe = '+';
        return unbounded_int_difference(b,a2);
    }
    unbounded_int res = init_unbounded_int();
    res.signe = a.signe;
    int retenue = 0;
    chiffre *tmpa = a.dernier;
    chiffre *tmpb = b.dernier;
    while (tmpa != NULL && tmpb != NULL) {
        int tmp = tmpa->c + tmpb->c - 2 * '0' + retenue;
        retenue = tmp/10;
        add_char_to_unbounded_int_at_start(&res, tmp%10 + '0');
        tmpa = tmpa->precedent;
        tmpb = tmpb->precedent;
    }
    while (tmpa != NULL) {
        int tmp = tmpa->c - '0' + retenue;
        retenue = tmp/10;
        add_char_to_unbounded_int_at_start(&res, tmp%10 + '0');
        tmpa = tmpa->precedent;
    }
    while (tmpb != NULL) {
        int tmp = tmpb->c - '0' + retenue;
        retenue = tmp/10;
        add_char_to_unbounded_int_at_start(&res, tmp%10 + '0');
        tmpb = tmpb->precedent;
    }
    if(retenue != 0) {
        add_char_to_unbounded_int_at_start(&res, retenue + '0');
    }
    return res;
}

unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b) {
    if (a.signe == '*' || b.signe == '*') return unbounded_int_error;
    if (b.signe != a.signe) {
        if (b.signe == '-') {
            unbounded_int b2 = b;
            b2.signe = '+';
            return unbounded_int_somme(a, b2);
        }
        unbounded_int b2 = b;
        b2.signe = '-';
        return unbounded_int_somme(a, b2);
    }
    unbounded_int res = init_unbounded_int();
    int signe = unbounded_int_cmp_unbounded_int(a,b);
    if(signe == 0) {
        add_char_to_unbounded_int_at_start(&res, '0');
        return res;
    }
    if(signe == -1 && a.signe == '+') {
        res = unbounded_int_difference(b, a);
        res.signe = '-';
        return verify_0_unbounded_int(res); 
    }
    if(signe == 1 && a.signe == '-') {
        res = unbounded_int_difference(b, a);
        res.signe = '+';
        return verify_0_unbounded_int(res);
    }
    if(signe == 1 && a.signe == '+') {
        res.signe = '+';
    }
    if(signe == -1 && a.signe == '-') {
        res.signe = '-';
    }
    int retenue = 0;
    chiffre *tmpa = a.dernier;
    chiffre *tmpb = b.dernier;
    while (tmpa != NULL && tmpb != NULL) {
        int tmp = tmpa->c - tmpb->c - retenue;
        retenue = tmp>=0?0:1;
        add_char_to_unbounded_int_at_start(&res, tmp>=0?(tmp+'0'):(abs(10+tmp) + '0'));
        tmpa = tmpa->precedent;
        tmpb = tmpb->precedent;
    }
    while (tmpa != NULL) {
        int tmp = tmpa->c - '0' - retenue;
        retenue = tmp>=0?0:1;
        add_char_to_unbounded_int_at_start(&res, tmp>=0?(tmp+'0'):(abs(10+tmp) + '0') + '0');
        tmpa = tmpa->precedent;
    }
    while (tmpb != NULL) {
        int tmp = tmpb->c - '0' - retenue;
        retenue = tmp>=0?0:1;
        add_char_to_unbounded_int_at_start(&res, tmp>=0?(tmp+'0'):(abs(10+tmp) + '0') + '0');
        tmpb = tmpb->precedent;
    }
    return verify_0_unbounded_int(res);
}

unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b) {
    if (a.signe == '*' || b.signe == '*') return unbounded_int_error;
    unbounded_int c = init_unbounded_int();
    size_t lena = a.len;
    size_t lenb = b.len;
    for (int i = 0; i < lena + lenb; i++) {
        add_char_to_unbounded_int_at_start(&c,'0');
    }
    for (int j = 0 ; j < lenb; j++) { 
        int r = 0 ;
        if (get_chiffre_at_unit(&b,j) -> c == '0' ) continue;
        for (int i = 0; i < lena; i++) {
            int cij = get_chiffre_at_unit(&c,i+j) -> c  - '0' ;
            int ai = get_chiffre_at_unit(&a,i) -> c - '0';
            int bj = get_chiffre_at_unit(&b,j) -> c  - '0';
            int v = cij + ai  * bj  + r;
            get_chiffre_at_unit(&c,i+j) -> c = (v %10) + '0';
            r = v/10;
        }
    get_chiffre_at_unit(&c,j+lena) -> c = r + '0';
    }
    c.signe = (a.signe == b.signe) ? '+' : '-';
    return verify_0_unbounded_int(c);
}

int main(int argc, char const *argv[]) {
    unbounded_int a = string2unbounded_int("-30");
    unbounded_int b = string2unbounded_int("-15900");
    unbounded_int c = unbounded_int_somme(a,b);
    unbounded_int d = unbounded_int_produit(a,b);
    // printf("%c\n", get_chiffre_at_unit(&c,2) -> c);
    printf("%s\n", unbounded_int2string(d));

    return 0;
}
