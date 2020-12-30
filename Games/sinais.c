/*
 * FILE: signals.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/19/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "sinais.h"

void endGame(int s, siginfo_t *info, void *context) {
    if(s == SIGUSR1) {
    	if(pontuacao < 0)
    		pontuacao = 0;
    		
        exit(pontuacao);
    }
}