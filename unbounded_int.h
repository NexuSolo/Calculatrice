#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

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

typedef struct variable{
    const char *name;
    unbounded_int value;
    struct variable *suivant;
} variable;

typedef struct table{
    variable *premier;
} table;

static unbounded_int init_unbounded_int();

static void add_char_to_unbounded_int_at_end(unbounded_int *n, char c);

static void add_char_to_unbounded_int_at_start(unbounded_int *n, char c);

static chiffre *get_chiffre_at_unit(unbounded_int *n,int p);

static chiffre *char_to_chiffre(char c);

static unbounded_int string2unbounded_int(const char *e);

static char *unbounded_int2string(unbounded_int n);

static unbounded_int ll2unbounded_int(long long i);

static int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

static int unbounded_int_cmp_ll(unbounded_int a, long long b);

static unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

static unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b);

static unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b);

static unbounded_int unbounded_int_division( unbounded_int a, unbounded_int b);

static unbounded_int verify_0_unbounded_int(unbounded_int a);

static chiffre *get_chiffre_at_unit(unbounded_int *n,int p);

static unbounded_int unbounded_int_error = {.signe = '*', .len = 0, .premier = NULL, .dernier = NULL};




char *getLigne(FILE *f1);

void set_table_value(table *t, const char *name, unbounded_int value);

bool is_a_var(char *c, size_t taille);

bool is_a_number(char *c, size_t taille);

int verif_ligne(char **c, size_t taille);

void maj_variable(table *t, char **c, size_t taille);

void arg_print(char *c, table *t);

void traitement_ligne(table *t, char *l);

void lecture_fichier(FILE *f1,table *t);

variable *get_table_value(table *t, const char *name);

void save_table(table *t, const char *name);

table *load_table(const char *name);