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
        if (atoi(getenv("MAXPLAYERS")) == 0) {  //WRONG TYPE
            MAXPLAYERS = PLAYER_LIMIT;
            fprintf(stderr, "[ERRO] MAXPLAYERS nao definido -> MAXPLAYERS = %d\n", MAXPLAYERS);
        }
        else
            MAXPLAYERS = atoi(getenv("MAXPLAYERS"));
    }

    /* GET COMMAND LINE ARGUMENTS
     * FORMAT: ./arbitro -d duration -w wait
     *    Time passed in minutes (m);
     *    Minimum duration and waiting time of 1 minute (m);
     *    OPT:
     *      -d / -D -> Duration;
     *      -w / -W -> Wait;
     */

    int DURATION = -1, WAIT = -1;
    int opt, aux;

    // Check max args
    if (argc > 5) {
        fprintf(stderr, "[FATAL] Numero maximo de argumentos excedido!\n Limite: 5 -> Recebeu: %d\n", argc);
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "d:w:D:W:")) != -1) {
        switch (opt) {
            case 'd':
            case 'D':
                aux = atoi(optarg);
                if (aux >= 1)
                    DURATION = aux;
                break;
            case 'w':
            case 'W':
                aux = atoi(optarg);
                if (aux >= 1)
                    WAIT = aux;
                break;
            default:
                fprintf(stderr, "[ERRO] Opcao nao existe\n");
        }
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
    printf("GAMEDIR: %s\tMAXPLAYERS: %d\n", GAMEDIR, MAXPLAYERS);
    printf("DURATION: %d\tWAIT:%d\n", DURATION, WAIT);
    exit(EXIT_SUCCESS);
}
