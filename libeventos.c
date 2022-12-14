#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libeventos.h"

/* Funcoes internas */

/* funcao que gera um numero aleatorio entre dois limites */
int gerar_numeros_aleatorios (int min, int max) {
    return min + (rand() % max); 
}

/* cria o array de rumores e o retorna em rumores */
/* Cria uma seguencia ordenada de rumores com id = count (1 a 30) */
void cria_rumores_mundo (conjunto_t *rumores) {
    int count;
    for (count = 1; count <= RUMORES_MUNDO; count++) 
        insere_conjunto (rumores, count);
}

/* Cria todos os locais que vão ser usados na simulacao do mundo */
/* Gera os locaois do mundo */
void cria_locais_mundo (local_m locais[]) {
    local_m local;
    int count;
    for (count = 1; count <= LOCAIS_MUNDO; count++) {
        local.id = count;
        local.lot_max = gerar_numeros_aleatorios (5, 30); 
        local.localizacao.x = gerar_numeros_aleatorios (0, TAM_MUNDO-1); 
        local.localizacao.y = gerar_numeros_aleatorios (0, TAM_MUNDO-1); 
        local.pessoas = cria_conjunto (local.lot_max); 
        local.fila = cria_fila (); 
        
        locais[count-1] = local;
    }
}

/* cria todas as pessoas que participaram da simulacao do mundo */
/* cria seus respectivos eventos de chegada iniciais */
void cria_pessoas_mundo (pessoa_m pessoas[], local_m locais[], conjunto_t rumores[], lef_t *eventos) {
    pessoa_m pessoa;
    int count;
    for (count = 1; count <= PESSOAS_MUNDO; count++) {
        pessoa.id = count;
        pessoa.extroversao = gerar_numeros_aleatorios (0, 100);
        pessoa.paciencia = gerar_numeros_aleatorios (0, 100);
        pessoa.idade = gerar_numeros_aleatorios (18, 101);
        pessoa.rumores = cria_subconjunto (rumores, gerar_numeros_aleatorios (1, 5));
        
        pessoas[count-1] = pessoa;

        cria_evento_de_chegada (pessoa, locais[gerar_numeros_aleatorios (1, LOCAIS_MUNDO)-1], eventos, gerar_numeros_aleatorios (0, 96*7));
    }
}

/* trata a saida do evento de chegada */
void tratamento_saida_ev_chegada (mundo_m mundo, pessoa_m pessoa, local_m local, int caso) {
    switch (caso) {
        case 1: { /* caso de entrada */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), ENTRA\n", mundo.tempo_atual, pessoa.id, local.id, cardinalidade (local.pessoas), local.lot_max);
            break;
        }
        case 2: { /* caso de ida para fila */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), FILA %2d\n", mundo.tempo_atual, pessoa.id, local.id, cardinalidade (local.pessoas), local.lot_max, tamanho_fila(local.fila));
            break;
        }
        case 3: { /* caso de desistencia */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), DESISTE\n", mundo.tempo_atual, pessoa.id, local.id, cardinalidade (local.pessoas), local.lot_max);
            break;
        }
    }
}

/* trata a saida do evento de saida */
void tratamento_saida_ev_saida (mundo_m mundo, pessoa_m pessoa_saida, int pessoa_fila, local_m local, int caso) {
    switch (caso) {
        case 1: { /* caso de saida sem liberar espaço na fila */
            printf ("%6d:SAIDA Pessoa %4d Local %2d (%2d/%2d)\n", mundo.tempo_atual, pessoa_saida.id, local.id, cardinalidade (local.pessoas), local.lot_max);
            break;
        }
        case 2: { /* caso de saida libearando espaço na fila */
            printf ("%6d:SAIDA Pessoa %4d Local %2d (%2d/%2d), REMOVE FILA %2d\n", mundo.tempo_atual, pessoa_saida.id, local.id, cardinalidade (local.pessoas), local.lot_max, pessoa_fila);
            break;
        }
    }
}

/* Funcoes de criacao */

mundo_m cria_mundo (lef_t *eventos) {
    mundo_m mundo;
    mundo.tempo_atual = 0; 
    mundo.n_pessoas = PESSOAS_MUNDO; 
    mundo.n_locais = LOCAIS_MUNDO; 
    mundo.tam_mundo.x = TAM_MUNDO; 
    mundo.tam_mundo.y = TAM_MUNDO; 

    mundo.rumores = cria_conjunto (RUMORES_MUNDO);
    cria_rumores_mundo (mundo.rumores); 
    cria_locais_mundo (mundo.locais); 
    cria_pessoas_mundo (mundo.pessoas, mundo.locais, mundo.rumores, eventos); 

    cria_evento_de_fim (eventos);
    
    return mundo;
}


/* funcao para criar eventos de chegada */
void cria_evento_de_chegada (pessoa_m pessoa, local_m local, lef_t *eventos, int tempo) {
    evento_t evento; 
    dados_chegada_m chegada_pessoa; 

    chegada_pessoa.id_pessoa = pessoa.id; 
    chegada_pessoa.id_local = local.id; 
   
    evento.tempo = tempo;
    evento.tipo = 1;
    evento.tamanho = sizeof(dados_chegada_m);
    evento.dados = &chegada_pessoa;
    
    adiciona_ordem_lef (eventos, &evento);
}

/* funcao para criar eventos de saida */
void cria_evento_de_saida (pessoa_m pessoa, local_m local, lef_t *eventos, int tempo) {
    evento_t evento; 
    dados_saida_m saida_pessoa; 

    saida_pessoa.id_pessoa = pessoa.id; 
    saida_pessoa.id_local = local.id; 
    
    evento.tempo = tempo;
    evento.tipo = 2;
    evento.tamanho = sizeof(dados_saida_m);
    evento.dados = &saida_pessoa;
    
    adiciona_ordem_lef (eventos, &evento);
}

/* funcao para criar eventos de disceminacao */
void cria_evento_de_disceminacao (pessoa_m pessoa, local_m local, lef_t *eventos, int tempo) {
    evento_t evento; 
    dados_disceminacao_m disceminacao_pessoa; 
    int nrd = pessoa.extroversao/10;

    disceminacao_pessoa.id_pessoa_origem = pessoa.id; 
    disceminacao_pessoa.id_local = local.id; 
    disceminacao_pessoa.cj_rumores = cria_subconjunto (pessoa.rumores, nrd); 
    
    evento.tempo = tempo;
    evento.tipo = 3;
    evento.tamanho = sizeof(dados_disceminacao_m);
    evento.dados = &disceminacao_pessoa;
    
    adiciona_ordem_lef (eventos, &evento);
} 

/* funcao para criar eventos de fim */
void cria_evento_de_fim (lef_t *eventos) {
    evento_t evento; 

    evento.tempo = 34944;
    evento.tipo = 4;
    evento.tamanho = 0;
    evento.dados = NULL;
    adiciona_ordem_lef (eventos, &evento);
}

/* Funcoes de execucao */
/* ------------------------------------------------------------------------------- */
void evento_chegada (mundo_m mundo, lef_t *eventos, int id_pessoa, int id_local) {
    local_m local = mundo.locais[id_local-1]; /* variavel para o local */
    pessoa_m pessoa = mundo.pessoas[id_pessoa-1]; /* variavel para a pessoa */
    int tpl; 

    /* se o local estiver lotado, decide se vai para a fila ou se vai para outro lugar */
    if (cardinalidade (local.pessoas) == local.lot_max) {
        /* se a paciencia for suficiente, entra na fila */
        if ((pessoa.paciencia / 4 - tamanho_fila (local.fila)) > 0) {
            insere_fila (local.fila, id_pessoa);
            tratamento_saida_ev_chegada (mundo, pessoa, local, 2); 
        }
        else { /* caso não, cria um evento de saida da pessoa */
            cria_evento_de_saida (pessoa, local, eventos, mundo.tempo_atual);
            tratamento_saida_ev_chegada (mundo, pessoa, local, 3);
        }
    } else { 
        insere_conjunto (local.pessoas, id_pessoa); 
        tratamento_saida_ev_chegada (mundo, pessoa, local, 1);
        tpl = 1 + (pessoa.paciencia/10+gerar_numeros_aleatorios (2, 6)); 
        /* cria o evento de disceminacao para a pessoa */
        cria_evento_de_disceminacao (pessoa, local, eventos, mundo.tempo_atual+gerar_numeros_aleatorios (0, tpl));
        /* cria o evento de saida da pessoa */
        cria_evento_de_saida (pessoa, local, eventos, mundo.tempo_atual+tpl);
    }
}

/* calcula o proximo local, distancia entre eles e a velocidade de locomoçcao da pessoa */
void evento_saida (mundo_m mundo, lef_t *eventos, int id_pessoa, int id_local) {
    local_m local = mundo.locais[id_local-1]; /* variavel para o local */
    pessoa_m pessoa = mundo.pessoas[id_pessoa-1]; /* variavel para a pessoa */
    int pessoa_fila; /* possivel pessoa que pode ser retirada da fila */
    local_m local_dest; /* variavel que recebe o local de destino da pessoa */
    int tdl, dist_cart, velocidade; /* variaveis para calculo do tempo de deslocamento */

    local_dest = mundo.locais[gerar_numeros_aleatorios (1, LOCAIS_MUNDO)-1];
    dist_cart = sqrt (pow (local_dest.localizacao.x - local.localizacao.x, 2) + pow (local_dest.localizacao.y - local.localizacao.y, 2));
    velocidade = 100 - (pessoa.idade-40);
    tdl = dist_cart/velocidade;

    retira_conjunto (local.pessoas, pessoa.id);

    /* cria evento de chegada da pessoa no proximo lugar */
    cria_evento_de_chegada (pessoa, local_dest, eventos, mundo.tempo_atual+(tdl/15));
    if (!fila_vazia (local.fila)) {
        retira_fila (local.fila, &pessoa_fila);
        cria_evento_de_chegada (mundo.pessoas[pessoa_fila-1], local, eventos, mundo.tempo_atual);
        tratamento_saida_ev_saida (mundo, pessoa, pessoa_fila, local, 2);
        return;
    }
    tratamento_saida_ev_saida (mundo, pessoa, -1, local, 1);
}

/* possui o tratamento de retorno verbose na propia funcao, devido a maneira que este evento funciona */
void evento_disseminacao (mundo_m mundo, int id_pessoa, int id_local, conjunto_t *cj_rumores) {
    local_m local = mundo.locais[id_local-1]; /* variavel para o local */
    pessoa_m pessoa_origem = mundo.pessoas[id_pessoa-1]; /* variavel para a pessoa de origem dos rumores */
    int id_pessoa_destino;
    int iterador_rumor, iterador_pessoa, rumor;

    printf ("%6d:RUMOR Pessoa %4d Local %2d ", mundo.tempo_atual, pessoa_origem.id, local.id);

    iniciar_iterador (cj_rumores);
    for (iterador_rumor = 0; iterador_rumor < cardinalidade (cj_rumores); iterador_rumor++) {
        incrementar_iterador (cj_rumores, &rumor); 
        iniciar_iterador (local.pessoas); 
        for (iterador_pessoa = 0; iterador_pessoa < cardinalidade (local.pessoas); iterador_pessoa++) {
            incrementar_iterador (local.pessoas, &id_pessoa_destino);
            if (gerar_numeros_aleatorios (0, 100) < mundo.pessoas[id_pessoa_destino-1].extroversao)  
                if (insere_conjunto (mundo.pessoas[id_pessoa_destino-1].rumores, rumor))
                    printf ("(P%d/R%d) ", mundo.pessoas[id_pessoa_destino-1].id, rumor);
        }
    }
    printf ("\n");
}

void evento_fim (mundo_m mundo) {
    int count;

    /* libera todos os rumores das pessoas */
    for (count = 0; count < mundo.n_pessoas; count++) 
        destroi_conjunto (mundo.pessoas[count].rumores);
    /* libera as pessoas dos locais e as filas */        
    for (count = 0; count < mundo.n_locais; count++) {
        destroi_conjunto (mundo.locais[count].pessoas);
        destroi_fila (mundo.locais[count].fila);
    }
    /* libera os rumores do mundo */
    destroi_conjunto (mundo.rumores);
}
