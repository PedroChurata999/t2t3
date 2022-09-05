#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"

/* cria uma copia do conjunto enviado como parametro */
/* insere cada elemento do conjunto enviado como parametro na copia */
conjunto_t * cria_copia (conjunto_t *c) {
    conjunto_t *conj;
    int count;

    if ((conj = cria_conjunto (c->max)) != NULL) {
        for (count = 0; count < c->card; count++) {
            insere_conjunto (conj, *(c->v+count));
        }
        return conj;
    } else return 0;
}

/* Incicializa o iterador em 0 */
void iniciar_iterador (conjunto_t *c) {
    c->ptr = 0;
}

/* Verifica se o ponteiro vai passar do fim do conjunto */
/* Necessário retornar o valor do ponteiro -1 pois o array inicia na casa 0 e não 1 */
int incrementar_iterador (conjunto_t *c, int *elemento) {
    if (c->ptr == c->card) return 0;
    c->ptr++;
    *elemento = *(c->v+(c->ptr-1));
    return 1;
}

/* Se o n for maior que a cardinalidade retorna uma copia do conjunto */
/* consideramos o max do subconjunto sendo o max do conjunto enviado */
/* Se a inserção ocorrer ok, ou seja, o elemento nao existir no conjunto, soma + 1 no count */
conjunto_t * cria_subconjunto (conjunto_t *c, int n) {
    conjunto_t *conj;
    int count = 0;
    if (n >= c->card) return cria_copia (c);
    if ((conj = cria_conjunto (c->max)) != NULL) {
        if (c->card == 0) return conj;
        while (count < n) {
            if (insere_conjunto (conj, *(c->v+(rand() % (cardinalidade (c))))))
                count++;
        }
        return conj;
    } else return NULL;
}

/* Busca iterativa o elemento x dentro do conjunto. */
/* Retorna a posição do elemento se ele foi encontrado e -1 caso ele não esteja no conjunto */
int busca_binaria (conjunto_t *c, int x) {
    int begin, end, iterator;
    begin = 0;
    end = c->card;
    while (end > begin) {
        iterator = (begin + end) / 2;
        if (*(c->v+iterator) == x) return iterator;
        if (*(c->v+iterator) < x) begin = iterator + 1;
        else end = iterator;
    }
    return -1;
}

/* Caso o array esteja vazio, insere no primeiro espaço */
/* Enquanto o valor da posição foi maior que o elemento empurra as valores uma 'casa' para frente */
/* Insere o elemento na poisção correta, mantendo ordenado */
void sorted_insertion (int x, conjunto_t *c) {
    int count = c->card;

    if (c->card == 0) 
        *(c->v+c->card) = x;
    else {
        while (count != 0 && x <= *(c->v+count-1)) {
            *(c->v+count) = *(c->v+count-1);
            count--;
        }
        *(c->v+count) = x;
    }
}

conjunto_t * cria_conjunto (int max){
    conjunto_t *c;

    if ((c = malloc (sizeof(conjunto_t)))) {
        c->max = 0;     /* inicializa maximo do vetor*/ 
        c->card = 0;    /* inicializa cardinalidade */
        if ((c->v = (int*)malloc(sizeof(int)*(max+1))))
            return c;
        else{
            free(c);
            return NULL;
        }
    }else return NULL;
}

conjunto_t * destroi_conjunto (conjunto_t *c){
    int count;
    for (count = 0; count< c->card; count++){
        retira_conjunto(c,*(c->v+count));
    }
    free(c->v);
    free(c);
    return NULL;

}

int conjunto_vazio (conjunto_t *c){
    if (c->card == 0)
        return 1;
    else
        return 0;

}

int cardinalidade (conjunto_t *c){
    return c->card;
}

int insere_conjunto (conjunto_t *c, int elemento){
    if (!pertence(c,elemento)){
        if (c->card + 1 > c->max)
            return -1;
        else{
            sorted_insertion(elemento,c);
            c->card++;
            return 1;
        }
    }else return 0;
}

int retira_conjunto (conjunto_t *c, int elemento){
    int pos, count;
    if ((pos = busca_binaria(c,elemento)) != -1){
        pos = pos + 1;
        for (count = pos; count <= c->card; count++)
            *(c->v+count-1) = *(c->v+count);
        c->card--;
        return 1;
    } else return 0;
}

/* Retorna se o elemento pertence ao conjunto */
int pertence (conjunto_t *c, int elemento) {
   if (busca_binaria (c, elemento) == -1) return 0;
   return 1;
}
