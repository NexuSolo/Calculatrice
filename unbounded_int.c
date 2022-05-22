#include "unbounded_int.h"

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#define LEN 4096
// output 0 = print dans le stdout,
// output 1 = print dans le fichier
static FILE *output = NULL;
static FILE *input = NULL;
static FILE *save = NULL;

variable *get_table_value(table *t, const char *name) {
    if(t -> premier == NULL) {
        return NULL;
    }
    variable *v = t -> premier;
    while (v != NULL) {
        if (strcmp(v->name, name) == 0) {
            return v;
        }
        v = v->suivant;
    }
    return NULL;
}

void set_table_value(table *t, const char *name, unbounded_int value) {
    variable *v = get_table_value(t, name);
    if (v == NULL) {
        variable *tmp = malloc(sizeof(variable));
        assert(tmp != NULL);
        tmp->name = name;
        tmp->value = value;
        tmp->suivant = t->premier;
        t->premier = tmp;
    } else {
        v->value = value;
    }
}

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
    assert(res != NULL);
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

char *getline(FILE *f1) {
    if (f1 != stdin) {
        if(getc(f1) == EOF) {
                return NULL;
            }
            fseek(f1, -1, SEEK_CUR);
    }
    char *res = malloc(sizeof(char) * LEN);
    assert(res != NULL);
    if (f1 == stdin) {
        scanf("%s",res);
    }
    else {
        fscanf(f1, "%[^\n]", res);
    }
    //Déplacement du curseur après le saut de ligne (\n)
    fseek(f1, 1, SEEK_CUR);
    if (strlen(res) == 0) return NULL;
    return res;
}

bool is_a_var(char *c, size_t taille) {
    if(isalpha(c[0])) {
        for (size_t i = 1; i < taille; i++) {
            if(!isalnum(c[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool is_a_number(char *c, size_t taille) {
    if(c[0] == '-' || c[0] == '+' || isdigit(c[0])) {
        for (size_t i = 1; i < taille; i++) {
            if(!isdigit(c[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

int verif_ligne(char **c, size_t taille) {
    if(taille < 2) return -1;
    bool variable = false;
    bool print = false;
    bool op = false;
    int res = -1;
    for (size_t i = 0; i < taille; i++) {
        if(strcmp(c[i],"print") == 0) {
            if(i != 0) return -1;
            print = true;
        }
        else if(strcmp(c[i], "=") == 0) {
            if(print == true || i != 1) return -1;
            variable = false;
        }
        else if( strcmp(c[i], "+")  == 0  ||  strcmp(c[i], "-")   == 0   || strcmp(c[i], "*")  == 0  || strcmp(c[i], "/")  == 0 ) {
            if(i == 1 || i%2==0 || print == true)  return -1;
            variable = false;
            op = true;
        }
        else {
            if(variable == true || i == 3) return -1;
            if(i == 0 && !is_a_var(c[i],strlen(c[i])))  return -1;
            if(!is_a_var(c[i],strlen(c[i])) && !is_a_number(c[i],strlen(c[i]))) return -1;
            res++;
            variable = true;
        }
    }
    if(print && taille > 2) return -1;
    if(op && (taille < 3 || taille%2==0)) return -1;
    return res;
}

void maj_variable(table *t, char **c, size_t taille) {
    unbounded_int res[taille/2];
    int i = 0;
    int j = 2;
    bool prio = false;
    bool pos = true;
    while(j < taille) {
        if(strcmp(c[j], "-") == 0) {
            pos = false;
        }
        else if(strcmp(c[j], "*") == 0  || strcmp(c[j], "/")  == 0) {
            prio = true;
        }
        else if(strcmp(c[j], "+") != 0) {
            if(!prio) {
                if(is_a_var(c[j], strlen(c[j]))) {
                    res[i] = get_table_value(t, c[j])->value;
                    if(!pos) {
                        if(res[i].signe == '+') {
                            res[i].signe = '-';
                        }
                        else {
                            res[i].signe = '+';
                        }
                    }
                }
                else {
                    res[i] = string2unbounded_int(c[j]);
                    if(!pos) {
                        if(res[i].signe == '+') {
                            res[i].signe = '-';
                        }
                        else {
                            res[i].signe = '+';
                        }
                    }
                }
            i++;
            }
            else {
                if(is_a_var(c[j], strlen(c[j]))) {
                    if ( strcmp(c[j-1], "*") == 0  ) {
                        res[i-1] = unbounded_int_produit(res[i-1], get_table_value(t, c[j])->value);
                    }
                    else  {
                        res[i-1] = unbounded_int_division(res[i-1], get_table_value(t, c[j])->value);
                    }
                }
                else {
                    if ( strcmp(c[j-1], "*") == 0  ) {
                        res[i-1] = unbounded_int_produit(res[i-1], string2unbounded_int(c[j]));
                    }
                    else {
                        res[i-1] = unbounded_int_division(res[i-1], string2unbounded_int(c[j]));
                    }
                }
            }
            prio = false;
            pos = true;
        }
        j++;
    }
    unbounded_int res2 = init_unbounded_int();
    chiffre *f = char_to_chiffre('0');
    res2.premier = f;
    res2.dernier = f;
    for(int a = 0; a < i; a++) {
        res2 = unbounded_int_somme(res2, res[a]);
    }
    set_table_value(t, c[0], res2);
}

void arg_print(char *c, table *t) {
    if (output != NULL) {
        if (!is_a_var(c,strlen(c)) ) {
            fprintf(output,"%s\n", unbounded_int2string(string2unbounded_int(c)));
        } 
        else {
            variable *v = get_table_value(t,c);
            char *valeur = "0";
            if (v != NULL) {
                valeur = unbounded_int2string(v->value);
            }
            fprintf(output,"%s = %s\n",c,valeur);
        }
    }
}

void traitement_ligne(table *t, char *l) {
    printf("%s\n",l);
    char *res = strtok(l, " ");
    int taille = 0;
    char *buffer[1024];
    while(res != NULL) {
        buffer[taille++] = res;
        res = strtok(NULL, " ");
    }
    int tmp = verif_ligne(buffer, taille);
    if(tmp == -1) {
        // printf("%s",buffer);
        printf("erreur\n");
    }
    else if(tmp == 0) {
        // printf("Print\n");
        arg_print(buffer[1], t);
    }
    else {
        // printf("maj variable\n");
        maj_variable(t, buffer, taille);
    }
}

void lecture_fichier(FILE *f1,table *t) {
    char *c = getline(f1);
    while(c != NULL) {
            traitement_ligne(t, c);
            c = getline(f1);
    }
}

void save_table(table *t, const char *name) {
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", name);
        return;
    }
    variable *v = t->premier;
    while (v != NULL) {
        fprintf(f, "%s = %s\n", v->name, unbounded_int2string(v->value));
        v = v->suivant;
    }
    fclose(f);
}

table *load_table(const char *name) {
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        table *t = malloc(sizeof(table));
        assert(t != NULL);
        return t;
    }
    table *t = malloc(sizeof(table));
    assert(t != NULL);
    lecture_fichier(f, t);
    return t;
}

int main(int argc, char const *argv[]) {
    table *t;
    FILE *fInput,*fOutput,*fSave;
    bool in = false;
    bool o = false;
    int s = 0;
    input = stdin;
    output = stdout;
    assert(argc%2 == 1);
    for (int i=1; i<argc; i+=2) {
        const char *option = argv[i];
        if (strcmp(option,"-i") == 0 ) {
            if (in) {
                printf("Erreur : Plusieurs arguments -i\n");
                exit(1);
            }
            if (argv[i+1][0] == '-') {
                printf("Erreur : Instruction %s mal placee\n",argv[i+1]);
                exit(1);
            }
            fInput = fopen(argv[i+1],"r");
            if (fInput == NULL) {
                fprintf(stderr,"Fichier %s inexistant\n",argv[i+1]);
                return 0;
                exit(1);

            }
            input = fInput;
            in = true;
        }
        else if (strcmp(option,"-o") == 0 ) {
            if (o) {
                printf("Erreur : Plusieurs arguments -o\n");
                exit(1);
            }
            if (argv[i+1][0] == '-') {
                printf("Erreur : Instruction %s mal placee\n",argv[i+1]);
                exit(1);
            }
            fOutput = fopen(argv[i+1],"w");
            if (fOutput == NULL) {
                fprintf(stderr," Echec ouverture fichier %s\n",argv[i+1]);
                exit(1);
            }
            output = fOutput;
            o = true;
        }
        else if (strcmp(option,"-s") == 0 ) {            if (s != 0) {
                printf("Erreur : Plusieurs arguments -s\n");
                exit(1);
            }
             if (argv[i+1][0] == '-') {
                printf("Erreur : Instruction %s mal placee\n",argv[i+1]);
                exit(1);
            }
            fSave = fopen(argv[i+1],"r+");
            // if (fSave == NULL) {
            //     fprintf(stderr," Echec ouverture fichier %s\n",argv[i+1]);
            //     exit(1);
            // }
            save = fSave;
            s = i+1;
        }
    }
    if(s != 0) {
        t = load_table(argv[s]);
    }
    else {
        t = load_table(NULL);
    }
    lecture_fichier(input,t);
    if(s != 0 ) {
        save_table(t,argv[s]);
    }
    return 0;
}
