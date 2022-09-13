#include <stdlib.h>
#include "libfila.h"

fila_t* cria_fila ()
{
        fila_t *new;

        /* cria fila vazia e a retorna */
        if ( new = (fila_t*)malloc(sizeof(fila_t))){
                new->ini = NULL;        /* inicializa ponteiro ini */
                new->fim = NULL;        /* inicializa ponteiro fim */
                new->tamanho = 0;       /* inicializa tamanho */       
                return new;
        }
        /* se falhar criacao da fila retornar NULL*/
        return NULL;
}

int insere_fila (fila_t* f, int elemento)
{
        nodo_f_t * novo_nodo;   /* declara novo elemento */
        novo_nodo = (nodo_f_t *)malloc(sizeof(nodo_f_t));
        if (fila_vazia(f)){    /* se fila for vazia */
                f->fim = novo_nodo;     /* novo nodo eh inicio */
                f->ini = novo_nodo;     /* novo nodo eh fim */           
        }
        else{
                if (f->ini->prox == NULL){      /* prox elemento do ini ah vazio*/
                        f->fim = novo_nodo;     /* novo nodo inserido no fim */     
                        novo_nodo->prox = f->ini->prox; /* prox do ini recebe prox do novo nodo*/
                        f->ini->prox = novo_nodo;       /* prox do ini recebe novo nodo*/
                }
        }
        f->tamanho++;   /* incrementa tamanho */
        return 1;
}

int retira_fila (fila_t* f, int* elemento)
{
        int aux = 0;
        /* se fila for vazia retorna 0 */
        if (fila_vazia(f)) 
                return 0;
        /* se fila tiver apenas 1 elemento retorna 1*/
        if (f->ini->prox == NULL){
                aux = f->ini->chave;
                free(f->ini);
                return 1;
        }

        /* enquanto o atual nao estiver apontando para o NULL */
        /* faz ele apontar para o proximo */
        nodo_f_t * atual = f->ini;
        while(atual->prox->prox != NULL){
                atual = atual->prox;
        }

        aux = atual->prox->chave;
        free(atual->prox);
        atual->prox = NULL;
        return aux;
}

int tamanho_fila (fila_t* f)
{
    return f->tamanho;
}

int fila_vazia (fila_t* f)
{
    if (f->tamanho == 0)
                return 1;
        else
                return 0;
}

fila_t* destroi_fila (fila_t* f)
{
    nodo_f_t *p;

    if (f->tamanho == 0){
            return f;
    }

    while (f->ini->prox != NULL){
            p = f->ini;
            f->ini = f->ini->prox;
            f->tamanho--;
            free(p);
    }
    return NULL;
}
