/*
 * FILE: jogadores.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 12/6/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Cliente/cliente.h"
#include "arbitro.h"

int verificaCliente(info* jogadores, const int* nJogadores, comCliente* coms) {

    // Se a lista de jogadores tiver vazia
    if(jogadores == NULL)
        return 0;

    // Verifica se o jogador existe na lista
    for (int i = 0; i < *nJogadores; ++i) {
        if(jogadores[i].pidCliente == coms->pid)
            return 1;
    }

    // Caso o jogador nao exista na lista
    return 0;
}

info* adicionaCliente(info* jogadores, int* nJogadores, comCliente* coms) {

    info* array = NULL;

    // Realocar espaco para um novo jogador
    array = realloc(jogadores, sizeof(info) * ((*nJogadores) + 1));
    if(array == NULL) {
        fprintf(stderr, "[ERRO] Erro ao adicionar novo cliente!\n");
        coms->cdgErro = 1;
        return jogadores;
    }

    // Preencher os dados do jogador
    jogadores = array;
    jogadores[(*nJogadores)++] = preencheDadosCliente(coms);

    return jogadores;
}

info preencheDadosCliente(comCliente* coms) {

    info novo;

    novo.pidCliente = coms->pid;
    strcpy(novo.pipeCliente, coms->pipeCliente);
    strcpy(novo.nomeJogador, coms->nomeJogador);

    return novo;
}

void listaJogadores(const info* jogadores, const int* nJogadores) {

    if((*nJogadores) == 0) {
        printf("[AVISO] Ainda nenhum jogador entrou no campeonato!\n");
        return ;
    }

    for(int i = 0; i < (*nJogadores); ++i) {
        printf("Jogador %d:\nNome: %s\nJogo: %s\n\n", i+1, jogadores[i].nomeJogador, jogadores[i].nomeJogo);
    }

}

int existeJogador(info* jogadores, const int* nJogadores, char* jogadorRemover) {

    for(int i = 0; i < (*nJogadores); ++i)
        if(strcmp(jogadores[i].nomeJogador, jogadorRemover) == 0)
            return i;

    return -1;
}

void removeJogador(info** jogadores, int* nJogadores, int* posicao) {

    info* aux = NULL;
    info remove;

    // Caso so exista 1 jogador
    if((*nJogadores) == 1 && (*posicao) == 0) {
        free(jogadores[0]);
        --(*nJogadores);
        printf("[RES] Jogador removido com sucesso!(nJogadores = %d)\n", *nJogadores);
    }
    else {
        remove = *jogadores[(*posicao)];
        *jogadores[(*posicao)] = *jogadores[(*nJogadores) - 1];
        aux = (info*) realloc((*jogadores), sizeof(info) * ((*nJogadores) - 1));
        if(aux != NULL) {
            *jogadores = aux;
            --(*nJogadores);
            printf("[RES] Jogador removido com sucesso!\n");
            return ;
        }

        *jogadores[(*nJogadores)] = remove;
        printf("[RES] Impossivel remover o jogador pretendido!\n");
        return ;

    }
}

info* libertaJogadores(info* jogadores, int* nJogadores) {

    if((*nJogadores) == 0)
        return jogadores;
    else {
        free(jogadores);
        jogadores = NULL;
        return jogadores;
    }

}
