#include "unbounded_int.c"

int main(int argc, char const *argv[]) {
    unbounded_int u = init_unbounded_int();
    unbounded_int u2 = ll2unbounded_int(10);
    unbounded_int u3 = string2unbounded_int("-5");
    unbounded_int u4 = string2unbounded_int("27500");
    unbounded_int u5 = ll2unbounded_int(-34000);
    unbounded_int u6 = string2unbounded_int("27500");
    u = ll2unbounded_int(0);
    printf("somme de 0 et 10 : %s\n",unbounded_int2string(unbounded_int_somme(u,u2)));
    printf("somme de 0 et -5 : %s\n",unbounded_int2string(unbounded_int_somme(u,u3)));
    printf("somme de 27500 et -34000 : %s\n",unbounded_int2string(unbounded_int_somme(u4,u5)));
    printf("somme de -34000 et -5 : %s\n",unbounded_int2string(unbounded_int_somme(u3,u5)));
    printf("\n");

    printf("difference entre 0 et 10 : %s\n",unbounded_int2string(unbounded_int_difference(u,u2)));
    printf("difference entre 0 et -5 : %s\n",unbounded_int2string(unbounded_int_difference(u,u3)));
    printf("difference entre 27500 et -34000 : %s\n",unbounded_int2string(unbounded_int_difference(u4,u5)));
    printf("difference entre -34000 et -5 : %s\n",unbounded_int2string(unbounded_int_difference(u3,u5)));
    printf("\n");

    printf("produit entre -5 et 10 : %s\n",unbounded_int2string(unbounded_int_produit(u3,u2)));
    printf("produit entre 0 et 10 : %s\n",unbounded_int2string(unbounded_int_produit(u,u2)));
    printf("produit entre 27500 et -34000 : %s\n",unbounded_int2string(unbounded_int_produit(u4,u5)));
    printf("produit entre -34000 et -5 : %s\n",unbounded_int2string(unbounded_int_produit(u3,u5)));
    printf("\n");

    printf("division de 10 par -5 : %s\n",unbounded_int2string(unbounded_int_division(u2,u3)));
    printf("division de 0 par 10 : %s\n",unbounded_int2string(unbounded_int_division(u,u2)));
    printf("division de 27500 par 10 : %s\n",unbounded_int2string(unbounded_int_division(u4,u2)));
    printf("division de 27500 par -34000 : %s\n",unbounded_int2string(unbounded_int_division(u4,u5)));
    printf("division de -34000 par -5 : %s\n",unbounded_int2string(unbounded_int_division(u5,u3)));
    printf("\n");
    
    printf("comparaison entre 10 et 27500 : %d\n", unbounded_int_cmp_unbounded_int(u, u6));
    printf("comparaison entre 27500 et 10 : %d\n", unbounded_int_cmp_unbounded_int(u6, u));
    printf("comparaison entre 10 et -5 : %d\n", unbounded_int_cmp_unbounded_int(u, u2));
    printf("comparaison entre -5 et 10 : %d\n", unbounded_int_cmp_unbounded_int(u2, u));
    printf("comparaison entre -34000 et -5 : %d\n", unbounded_int_cmp_unbounded_int(u5, u3));
    printf("comparaison entre -5 et -34000 : %d\n", unbounded_int_cmp_unbounded_int(u3, u5));
    printf("comparaison entre 27500 et 27500 : %d\n", unbounded_int_cmp_unbounded_int(u4, u6));

    printf("comparaison entre 10 et 27500 en ll : %d\n",unbounded_int_cmp_ll(u2,27500));
    
    return 0;
}
