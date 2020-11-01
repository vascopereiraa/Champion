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

#include "arbitro.h"

int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);

    /*  GET ENVIRONMENT VARIABLES */
    char* GAMEDIR;
    if (getenv("GAMEDIR") == NULL) {  //NOT DEFINED
        GAMEDIR = GAMES;
        fprintf(stderr, "[ERROR] GAMEDIR NOT DEFINED => GAMEDIR = %s\n", GAMES);
    }
    else
        GAMEDIR = getenv("GAMEDIR");

    int MAXPLAYERS;
    if (getenv("MAXPLAYERS") == NULL) {  //NOT DEFINED
        MAXPLAYERS = PLAYER_LIMIT;
        fprintf(stderr, "[ERROR] MAXPLAYERS NOT DEFINED => MAXPLAYERS = %d\n", MAXPLAYERS);
    }
    else {
        if (atoi(getenv("MAXPLAYERS")) == 0) {  //WRONG TYPE
            MAXPLAYERS = PLAYER_LIMIT;
            fprintf(stderr, "[ERROR] MAXPLAYERS NOT DEFINED => MAXPLAYERS = %d\n", MAXPLAYERS);
        }
        else
            MAXPLAYERS = atoi(getenv("MAXPLAYERS"));
    }

    /* GET COMMAND LINE ARGUMENTS
     * FORMAT: ./arbitro DURACAO=123 ESPERA=12
     */
    if (argc != 3) {
        fprintf(stderr, "[FATAL] NUMBER OF COMMAND LINE ARGUMENTS INVALID\n");
        exit(2);
    }

    int DURACAO = 0, ESPERA = 0;
    char helper[50];
    for (int i = 8; i < strlen(argv[1]); i++) {
        helper[i-8] = argv[1][i];
    }
    helper[strlen(helper)] = '\0';
    DURACAO = atoi(helper);

    // Clear helper
    for(int i = 0; i < 50; i++)
        helper[i] = ' ';

    for (int i = 7; i < strlen(argv[2]); i++) {
        helper[i-7] = argv[2][i];
    }
    helper[strlen(helper)] = '\0';
    ESPERA = atoi(helper);

    // Wrong Arguments
    if (DURACAO == 0) {
        DURACAO = TIME;
        fprintf(stderr, "[ERROR] DURACAO NOT DEFINED => DURACAO = %d\n", TIME);
    }
    if(ESPERA == 0) {
        ESPERA = WAIT;
        fprintf(stderr, "[ERROR] ESPERA NOT DEFINED => ESPERA = %d\n", WAIT);
    }

    // PRINT INITIALIZATION
    printf("GAMEDIR: %s\tMAXPLAYERS: %d\n", GAMEDIR, MAXPLAYERS);
    printf("DURACAO: %d\tESPERA:%d\n", DURACAO, ESPERA);

    exit(EXIT_SUCCESS);
}
