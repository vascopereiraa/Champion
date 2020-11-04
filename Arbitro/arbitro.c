/*
 * FILE: arbitro.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/1/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arbitro.h"

int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);

    /*  GET ENVIRONMENT VARIABLES */
    char* GAMEDIR;
    if (getenv("GAMEDIR") == NULL) {  //NOT DEFINED
        GAMEDIR = GAMES;
        fprintf(stderr, "[ERRO] GAMEDIR nao definido -> GAMEDIR = \"%s\"\n", GAMES);
    }
    else
        GAMEDIR = getenv("GAMEDIR");

    int MAXPLAYERS;
    if (getenv("MAXPLAYERS") == NULL) {  //NOT DEFINED
        MAXPLAYERS = PLAYER_LIMIT;
        fprintf(stderr, "[ERRO] MAXPLAYERS nao definido -> MAXPLAYERS = %d\n", MAXPLAYERS);
    }
    else {
        if (strtol(getenv("MAXPLAYERS"), NULL, 10) == 0) {  //WRONG TYPE
            MAXPLAYERS = PLAYER_LIMIT;
            fprintf(stderr, "[ERRO] MAXPLAYERS nao definido -> MAXPLAYERS = %d\n", MAXPLAYERS);
        }
        else
            MAXPLAYERS = (int) strtol(getenv("MAXPLAYERS"), NULL, 10);
    }

    /* GET COMMAND LINE ARGUMENTS
     * FORMAT: ./arbitro -d duration -w wait
     *    Time passed in minutes (m);
     *    Minimum duration and waiting time of 1 minute (m);
     *    When both DURATION and WAIT are filled with user info -> exit loop;
     *    The first argument passed is the one saved
     *
     *    OPT:
     *      -d / -D -> Duration;
     *      -w / -W -> Wait;
     */

    int DURATION = -1, WAIT = -1;
    int opt, checkD = 0, checkW = 0;
    long int aux;
    while ((opt = getopt(argc, argv, "d:w:D:W:")) != -1) {
        switch (opt) {
            case 'd':
            case 'D':
                aux = strtol(optarg, NULL, 10);
                if (aux >= 1 && checkD == 0) {
                    DURATION = (int) aux;
                    checkD++;
                }
                break;
            case 'w':
            case 'W':
                aux = strtol(optarg, NULL, 10);
                if (aux >= 1 && checkW == 0) {
                    WAIT = (int) aux;
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
            break;
    }

    // WRONG ARGUMENT VALUES
    if (DURATION == -1) {
        DURATION = TIME;
        fprintf(stderr, "[ERRO] DURATION nao definida -> DURATION = %d\n", DURATION);
    }
    if(WAIT == -1) {
        WAIT = WAITING_TIME;
        fprintf(stderr, "[ERRO] WAIT nao definida -> WAIT = %d\n", WAIT);
    }

    // PRINT INITIALIZATION
    printf("Configuracoes do Arbitro: \n");
    printf("GAMEDIR: %s\tMAXPLAYERS: %d\n", GAMEDIR, MAXPLAYERS);
    printf("DURATION: %d\tWAIT:%d\n", DURATION, WAIT);
    exit(EXIT_SUCCESS);
}
