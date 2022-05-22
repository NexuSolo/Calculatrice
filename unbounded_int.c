#include "unbounded_int.h"

/**
 * Il initialise un unbounded_int
 * 
 * @return Un pointeur vers une structure de type unbounded_int.
 */
unbounded_int init_unbounded_int() {
    return (unbounded_int) {.signe = '+', .len = 0, .premier = NULL, .dernier = NULL};
}

/**
 * Il ajoute un chiffre au début d'un unbounded_int
 * 
 * @param n le unbounded_int auquel ajouter le char
 * @param c le caractère à ajouter à l'unbounded_int
 * 
 * @return Un pointeur vers le résultat.
 */
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

/**
 * Il ajoute un chiffre à la fin d'un unbounded_int
 * 
 * @param n le unbounded_int auquel ajouter le char
 * @param c le caractère à ajouter à l'unbounded_int
 * 
 * @return Un pointeur vers le résultat.
 */
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

/**
 * Il crée une nouvelle structure de chiffre, l'initialise avec le caractère donné et renvoie un
 * pointeur vers celui-ci
 * 
 * @param c le caractère à convertir en chiffre
 * 
 * @return Un pointeur vers un chiffre.
 */
chiffre *char_to_chiffre(char c) {
    chiffre *res = malloc(sizeof(chiffre));
    assert(res != NULL);
    res->c = c;
    res->precedent = res->suivant = NULL;
    return res;
}

/**
 * Il supprime tous les zéros non significatifs d'un unbounded_int
 * 
 * @param a le unbounded_int à vérifier
 * 
 * @return le unbounded_int a.
 */
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

/**
 * Il renvoie le chiffre à la position donnée dans le unbounded_int
 * 
 * @param n le unbounded_int dont nous voulons obtenir le chiffre
 * @param p la position du chiffre que vous voulez obtenir
 * 
 * @return Un pointeur vers le chiffre à la position p dans unbounded_int n.
 */
chiffre *get_chiffre_at_unit(unbounded_int *n,int p) {
    if (p > n -> len || p < 0) return NULL;
    chiffre *tmp = n -> dernier;
    for (int i = 0 ; i < p; i++) {
        tmp = tmp -> precedent;
    }
    return tmp;
}

/**
 * Il convertit un unbounded_int en une chaîne de caractère
 * 
 * @param n le unbounded_int à convertir en chaîne de caractère
 * 
 * @return La converstion de l'unbounded_int en chaîne de caractère.
 */
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

/**
 * Il convertit une chaîne en unbounded_int
 * 
 * @param e la chaîne à convertir
 * 
 * @return Un unbounded_int
 */
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



/**
 * Il convertit un long long en un unbounded_int
 * 
 * @param i le nombre à convertir
 * 
 * @return Un unbounded_int
 */
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

/**
 * Il compare deux entiers unbounded_int et renvoi :
 * -1 : Si l'unbounded_int de gauche est inférieur à celui de droite
 *  0 : Si les deux unbounded_int ont la même valeur
 *  1 : Si l'unbounded_int de gauche est supérieur à celui de droite
 * 
 * @param a Le premier unbounded_int
 * @param b le unbounded_int à comparer
 * 
 * @return un nombre entier.
 */
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if(a.premier -> c == '0' && b.premier -> c == '0') return 0;
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

/**
 * Il compare unubunded_int et un long long et renvoi :
 * -1 : Si l'unbounded_int de gauche est inférieur à celui de droite
 *  0 : Si les deux unbounded_int ont la même valeur
 *  1 : Si l'unbounded_int de gauche est supérieur à celui de droite
 * 
 * @param a Le premier unbounded_int à comparer.
 * @param b le nombre à comparer
 * 
 * @return Le résultat de la comparaison.
 */
int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    return unbounded_int_cmp_unbounded_int(a,ll2unbounded_int(b));
}

/**
 * Il additionne deux unbounded_int
 * 
 * @param a le premier unbounded_int
 * @param b l'unbounded_int à ajouter
 * 
 * @return La somme des deux unbounded_int.
 */
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

/**
 * C'est une fonction qui calcule la différence de deux unbounded_int
 * 
 * @param a le premier unbounded_int
 * @param b le unbounded_int à soustraire d'un
 * 
 * @return La différence entre deux unbounded_int.
 */
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
        add_char_to_unbounded_int_at_start(&res, tmp>=0?(tmp+'0'):(abs(10+tmp) + '0'));
        tmpa = tmpa->precedent;
    }
    while (tmpb != NULL) {
        int tmp = tmpb->c - '0' - retenue;
        retenue = tmp>=0?0:1;
        add_char_to_unbounded_int_at_start(&res, tmp>=0?(tmp+'0'):(abs(10+tmp) + '0'));
        tmpb = tmpb->precedent;
    }
    return verify_0_unbounded_int(res);
}

/**
 * Il multiplie deux unbounded_int
 * 
 * @param a le premier unbounded_int
 * @param b le unbounded_int à multiplier
 * 
 * @return Le produit de deux unbounded_int.
 */
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

/**
 * Il divise deux entiers non bornés
 * 
 * @param a le dividende
 * @param b le diviseur
 * 
 * @return Le quotient de la division de deux entiers non bornés.
 */
unbounded_int unbounded_int_division( unbounded_int a, unbounded_int b) {
    if (a.signe == '*' || b.signe == '*' || unbounded_int_cmp_ll(b,0) == 0) return unbounded_int_error;
    char signe = (a.signe == b.signe) ? '+' : '-';
    long long q = 0;
    if (a.signe == '-'|| b.signe == '-') {
        a.signe = '+';
        b.signe = '+';
    }
    unbounded_int r = a;
    while  (unbounded_int_cmp_unbounded_int(b,r) <=  0) {
        r = unbounded_int_difference(r,b);
        q++;
    }
    r = ll2unbounded_int(q);
    r.signe = signe;
    return verify_0_unbounded_int(r);
}