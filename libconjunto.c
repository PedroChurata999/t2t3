#include <stdio.h>
#include "libconjunto.h"

conjunto_t * cria_conjunto (int max);

conjunto_t * destroi_conjunto (conjunto_t *c);

int conjunto_vazio (conjunto_t *c);

int cardinalidade (conjunto_t *c);

int insere_conjunto (conjunto_t *c, int elemento);

int retira_conjunto (conjunto_t *c, int elemento);
