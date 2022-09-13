#include "liblef.h"
#include <stdlib.h>
#include <string.h>

/* define o Primeiro como NULL (lef vazia) */
lef_t * cria_lef (){
    lef_t *lef;
    if ((lef = malloc(sizeof(lef_t)))){
        lef->Primeiro = NULL;
        return lef;
    }else return NULL;
}

lef_t * destroi_lef (lef_t *l){
    nodo_lef_t *nodo;

    /* Se a lef estiver vazia, apenas da free em sua estrutura */
    if (l->Primeiro == NULL){
        free(l);
        return NULL;
    }
    /* Equanto a lista não ficar vazia, vai liberando os nodos */
    while(l->Primeiro != NULL){
        nodo = l->Primeiro;
        l->Primeiro = l->Primeiro->prox;
        free(nodo->evento->dados);
        free(nodo->evento);
        free(nodo);
    }
    /* Libera o espaço da estrutura da lista ao final */
    free(l);
    return NULL;
}

/* Se estiver vazia, altera a sequencia dos ponteiros */ 
int adiciona_inicio_lef (lef_t *l, evento_t *evento){
    nodo_lef_t *nodo;
    evento_t *evento_copia;
    
    /* Cria uma copia do evento */
    if((evento_copia = malloc(sizeof(evento_t)))){
        evento_copia->tempo = evento->tempo;
        evento_copia->tipo = evento->tipo;
        evento_copia->tamanho = evento->tamanho;
        evento_copia->dados = malloc(evento->tamanho);
        memcpy(evento_copia->dados,evento->dados,evento->tamanho);
    }else return 1;

    if((nodo = malloc(sizeof(nodo_lef_t)))){
        nodo->evento = evento_copia;
        nodo->prox = NULL;
    }else return 0;

    /* Se estiver vazia, altera a sequencia dos ponteiros */ 
    if (l->Primeiro == NULL)
        l->Primeiro = nodo;
    else{
        nodo->prox = l->Primeiro;
        l->Primeiro = nodo;
    }
    return 1;
}

int adiciona_ordem_lef  (lef_t *l, evento_t *evento){
    nodo_lef_t *nodo, *temp_ant, *temp_atual;
    evento_t *evento_copia;

    /* Cria uma copia do evento */
    /* antes de chamar a funcao de insercao no inicio, libera as estruturas alocadas */
    if ((evento_copia = (evento_t*)malloc(sizeof(evento_t)))){
        evento_copia->tempo = evento->tempo;
        evento_copia->tipo = evento->tipo;
        evento_copia->tamanho = evento->tamanho;
        evento_copia->dados = malloc (evento->tamanho);
        memcpy (evento_copia->dados, evento->dados, evento->tamanho);
    } else return 1;

    if (!(nodo = malloc(sizeof(nodo_lef_t)))) return 1;
    temp_atual =  l->Primeiro;
    /* se a minha lista estiver vazia, adiciona no inicio */
    if (temp_atual == NULL){
        free(evento_copia->dados);
        free(evento_copia);
        free(nodo);
        adiciona_inicio_lef(l,evento);
        return 1;
    }
    while(temp_atual->prox != NULL && temp_atual->evento->tempo < evento->tempo){
        temp_ant = temp_atual;
        temp_atual = temp_atual->prox;
    }
    /* se a minha lista estiver vazia, adiciona no inicio */
    if (temp_atual == l->Primeiro && temp_atual->evento->tempo >= evento->tempo){
        free(evento_copia->dados);
        free(evento_copia);
        free(nodo);
        adiciona_inicio_lef(l,evento);
        return 0;
    }else if (temp_atual->prox == NULL){
        nodo->evento = evento_copia;
        temp_atual->prox = nodo;
        nodo->prox = NULL;
    }else{  
        nodo->evento = evento_copia;
        temp_ant->prox = nodo;
        nodo->prox = temp_atual;
    }
    return 0;
}

evento_t * obtem_primeiro_lef (lef_t *l){
    nodo_lef_t *nodo;
    evento_t *evento;
    
    /* Se a lista ficou vazia retorna null */
    if (!l->Primeiro) return NULL;

    evento = l->Primeiro->evento;
    nodo = l->Primeiro;
    
    /* aponta o ponteiro do primeiro para o prox da lista */
    l->Primeiro = l->Primeiro->prox;
    free(nodo);
    return evento;
}

