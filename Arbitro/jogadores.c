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

#include "../Cliente/cliente.h"
#include "arbitro.h"

int verificaCliente(info* jogadores, const int* tamJogadores, const comCliente* coms) {

    // Se a lista de jogadores tiver vazia
    if(jogadores == NULL)
        return 0;

    // Verifica se o jogador existe na lista
    for (int i = 0; i < *tamJogadores; ++i) {
        if(jogadores->pidCliente == coms->pid)
            return 1;
    }

    // Caso o jogador nao exista na lista
    return 0;
}
 
