/*
 * FILE: initConfig.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/27/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "initConfig.h"

char* getGamedir() {

    int size;
    char* dir;

    if (getenv("GAMEDIR") == NULL) {  //NOT DEFINED
        size = strlen(GAMES);
        dir = (char*) malloc(size * sizeof(char));
        if(dir == NULL)
            exit(-1);
        strcpy(dir, GAMES);
        fprintf(stderr, "[ERRO] GAMEDIR nao definido -> GAMEDIR = \"%s\"\n", GAMES);
    }
    else {
        size = strlen(getenv("GAMEDIR"));
        dir = (char*) malloc(size * sizeof(char));
        if(dir == NULL)
            exit(-1);
        strcpy(dir, getenv("GAMEDIR"));
    }

    return dir;
}

int getMaxPlayers() {
    
    int maxplayers;
    
    if (getenv("MAXPLAYERS") == NULL) {  //NOT DEFINED
        maxplayers = PLAYER_LIMIT;
        fprintf(stderr, "[ERRO] maxplayers nao definido -> maxplayers = %d\n", maxplayers);
    }
    else {
        if (strtol(getenv("MAXPLAYERS"), NULL, 10) == 0) {  //WRONG TYPE
            maxplayers = PLAYER_LIMIT;
            fprintf(stderr, "[ERRO] maxplayers nao definido -> maxplayers = %d\n", maxplayers);
        }
        else
            maxplayers = (int) strtol(getenv("MAXPLAYERS"), NULL, 10);
    }

    return maxplayers;
}

void getCmdLnArgs(int argc, char **argv, int* D, int* W) {

    /*      GET COMMAND LINE ARGUMENTS
     * FORMAT: ./arbitro -d duration -w wait
     * Time passed in minutes (m);
     * Minimum duration and waiting time of 1 minute (m);
     * When both DURATION and WAIT are filled with user info -> exit loop;
     * The first argument passed is the one saved
     *
     * OPT:
     *   -d / -D -> Duration;
     *   -w / -W -> Wait;
     */


    int opt, checkD = 0, checkW = 0;
    long int aux;
    while ((opt = getopt(argc, argv, "d:w:D:W:")) != -1) {
        switch (opt) {
            case 'd':
            case 'D':
                aux = strtol(optarg, NULL, 10);
                if (aux >= 1 && checkD == 0) {
                    *D = (int) aux;
                    checkD++;
                }
                break;
            case 'w':
            case 'W':
                aux = strtol(optarg, NULL, 10);
                if (aux >= 1 && checkW == 0) {
                    *W = (int) aux;
                    checkW++;
                }
                break;
            default:
                if (optopt == 'd' || optopt == 'D' || optopt == 'w' || optopt == 'W')
                    fprintf(stderr, "[ERRO] Opcao -%c requer um argumento -> Valor padrao assumido\n", optopt);
                else
                    fprintf(stderr, "[ERRO] Opcao '-%c' nao existe\n", optopt);
        }

        if (checkD == 1 && checkW == 1)
            return;
    }

    // WRONG ARGUMENT VALUES
    if (*D == -1) {
        *D = TIME;
        fprintf(stderr, "[ERRO] DURATION nao definida -> DURATION = %d\n", *D);
    }
    if(*W == -1) {
        *W = WAITING_TIME;
        fprintf(stderr, "[ERRO] WAIT nao definida -> WAIT = %d\n", *W);
    }
}

init initialization(int argc, char **argv) {

    init setup;

    setup.GAMEDIR = getGamedir();
    setup.MAXPLAYERS = getMaxPlayers();
    setup.WAIT = -1;
    setup.DURATION = -1;
    getCmdLnArgs(argc, argv, &setup.DURATION, &setup.WAIT);

    return setup;
}

void printInit(init a) {
     printf("Configuracoes do Arbitro: \n");
     printf("GAMEDIR: %s\n", a.GAMEDIR);
     printf("MAXPLAYERS: %d\n", a.MAXPLAYERS);
     printf("DURATION: %d\n", a.DURATION);
     printf("WAIT: %d\n", a.WAIT);
}

