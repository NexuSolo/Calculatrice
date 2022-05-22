#include "unbounded_int.c"

#define LEN 4096
static FILE *output = NULL;
static FILE *input = NULL;
static FILE *save = NULL;

/**
 * Il renvoie la variable avec le nom donné dans la table donnée, ou NULL si elle n'existe pas
 * 
 * @param t la table dans laquelle chercher
 * @param name le nom de la variable que vous voulez obtenir
 * 
 * @return Un pointeur vers une variable.
 */
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

/**
 * Il ajoute une nouvelle variable à la table si elle n'existe pas, ou met à jour sa valeur si elle
 * existe
 * 
 * @param t le tableau à modifier
 * @param name le nom de la variable à définir
 * @param value la valeur à définir
 */
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

/**
 * Il écrit le contenu d'une table dans un fichier
 * 
 * @param t le tableau à sauvegarder
 * @param name le nom du fichier dans lequel enregistrer la table
 * 
 * @return Un pointeur vers une table.
 */
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

/**
 * Il lit un fichier et et stocke ses informations dans la table
 * 
 * @param name le nom du fichier à charger
 * 
 * @return Un pointeur vers une table.
 */
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

/**
 * Il prend une table, une ligne et le nombre d'argument dans la ligne comme paramètres. Il
 * analyse ensuite la liste des chaînes pour trouver les variables et les opérations à effectuer sur
 * celles-ci. Il effectue ensuite les opérations et stocke le résultat dans la table
 * 
 * @param t la table
 * @param c le tableau de chaînes
 * @param taille le nombre d'arguments
 */
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

/**
 * Il prend une table et une ligne en entrée, et il imprimera la valeur d'une variable ou mettra à
 * jour la valeur d'une variable
 * 
 * @param t le tableau des variables
 * @param l la ligne à traiter
 */
void traitement_ligne(table *t, char *l) {
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

/**
 * Il lit un fichier ligne par ligne, et pour chaque ligne, il appelle la fonction `traitement_ligne`
 * qui traitera la ligne
 * 
 * @param f1 le fichier à lire
 * @param t la table
 */
void lecture_fichier(FILE *f1,table *t) {
    char *c = getLigne(f1);
    while(c != NULL) {
            traitement_ligne(t, c);
            c = getLigne(f1);
    }
}

/**
 * Il lit une ligne d'un fichier et la renvoie sous forme de chaîne
 * Cette fonction peux etre appelé plusieurs fois jusqu'à être au bout du fichier
 * 
 * @param f1 le fichier à lire
 * 
 * @return Un string
 */
char *getLigne(FILE *f1) {
    char *res;
    if (f1 != stdin) {
        res = malloc(sizeof(char) * LEN);
        assert(res != NULL);
        if(fgetc(f1) == EOF) {
                return NULL;
        }
        fseek(f1, -1, SEEK_CUR);
        fscanf(f1,"%[^\n]",res);
    }
    else {
        res = malloc(sizeof(char) * LEN);
        assert(res != NULL);
        fgets(res, LEN, f1);
        res[strlen(res)-1] = '\0';

    }
    //Déplacement du curseur après le saut de ligne (\n)
    fseek(f1, 1, SEEK_CUR);
    if (strlen(res) == 0) return NULL;
    return res;
}

/**
 * Il vérifie si la ligne est valide et renvoi :
 * -1 : Si la ligne doit etre un print
 *  0 : Si il y a une érreur
 *  x : Le nombres de variable/nombres dans la ligne sinon
 * 
 * @param c le tableau de chaînes
 * @param taille le nombre d'arguments dans la ligne de commande
 * 
 * @return Le nombre de variables dans la ligne.
 */
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

/**
 * Il imprime la valeur d'une variable ou d'une constante
 * 
 * @param c la chaîne à imprimer
 * @param t le tableau des variables
 */
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

/**
 * Il vérifie si une chaîne est une variable
 * 
 * @param c la chaîne à vérifier
 * @param taille la longueur de la ficelle
 * 
 * @return Une valeur booléenne.
 */
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

/**
 * Il vérifie si une chaîne est un nombre
 * 
 * @param c la chaîne à vérifier
 * @param taille la longueur de la ficelle
 * 
 * @return Une valeur booléenne.
 */
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
