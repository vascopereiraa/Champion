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
#include <dirent.h>
#include <sys/wait.h>

#include "arbitro.h"

/*
void initialization(char GAMEDIR[], int* MAXPLAYERS, int *DURATION, int* WAIT, int argc, char *argv) {

    // todo: corrigir erros de ponteiros
    */
/*  GET ENVIRONMENT VARIABLES *//*

    if (getenv("GAMEDIR") == NULL) {  //NOT DEFINED
        GAMEDIR = GAMES;
        fprintf(stderr, "[ERRO] GAMEDIR nao definido -> GAMEDIR = \"%s\"\n", GAMES);
    }
    else
        GAMEDIR = getenv("GAMEDIR");

    if (getenv("MAXPLAYERS") == NULL) {  //NOT DEFINED
        *MAXPLAYERS = PLAYER_LIMIT;
        fprintf(stderr, "[ERRO] MAXPLAYERS nao definido -> MAXPLAYERS = %d\n", *MAXPLAYERS);
    }
    else {
        if (strtol(getenv("MAXPLAYERS"), NULL, 10) == 0) {  //WRONG TYPE
            *MAXPLAYERS = PLAYER_LIMIT;
            fprintf(stderr, "[ERRO] MAXPLAYERS nao definido -> MAXPLAYERS = %d\n", *MAXPLAYERS);
        }
        else
            *MAXPLAYERS = (int) strtol(getenv("MAXPLAYERS"), NULL, 10);
    }

    */
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
     *//*


    int opt, checkD = 0, checkW = 0;
    long int aux;
    while ((opt = getopt(argc, &argv, "d:w:D:W:")) != -1) {
        switch (opt) {
            case 'd':
            case 'D':
                aux = strtol(optarg, NULL, 10);
                if (aux >= 1 && checkD == 0) {
                    *DURATION = (int) aux;
                    checkD++;
                }
                break;
            case 'w':
            case 'W':
                aux = strtol(optarg, NULL, 10);
                if (aux >= 1 && checkW == 0) {
                    *WAIT = (int) aux;
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
    if (*DURATION == -1) {
        *DURATION = TIME;
        fprintf(stderr, "[ERRO] DURATION nao definida -> DURATION = %d\n", *DURATION);
    }
    if(*WAIT == -1) {
        *WAIT = WAITING_TIME;
        fprintf(stderr, "[ERRO] WAIT nao definida -> WAIT = %d\n", *WAIT);
    }
}
*/

void criaJogo() {

    int i = 0, res, pid, canalLeitura[2], canalEscrita[2], estado;
    DIR* dir;
    struct dirent* conteudo;
    char listaJogos[10][100];

    // Abrir pasta
    dir = opendir(GAMES);
    if(dir == NULL)
        exit(2);

    // Listar todos os jogos na pasta
    while((conteudo = readdir(dir)) != NULL) {
        if(conteudo->d_name[0] == 'g' && conteudo->d_name[1] == '_')
            strcpy(listaJogos[i++], conteudo->d_name);
    }

    closedir(dir);

    for(int k = 0; k < i; k++)
        printf("%s\n", listaJogos[k]);
    printf("\n");

    // todo: separar em diversas funcoes

    // Criar o pipe anonimo de comunicaocao com o filho
    pipe(canalLeitura);
    pipe(canalEscrita);
    
    // Criar o filho
    res = fork();
    if(res == 0) {
        pid = getpid();
        printf("Sou o filho");
        
        // Atribuir saida e entrada de dados para o pipe
        close(canalLeitura[0]);
        close(1);
        dup(canalLeitura[1]);
        close(canalLeitura[1]);
        
        close(canalEscrita[1]);
        close(0);
        dup(canalEscrita[0]);
        close(canalEscrita[0]);

        // Mudar diretoria corrente
        // todo: acrescentar verificacao da diretoria
        chdir(GAMES);
        execl(listaJogos[0], listaJogos[0], NULL);

        fprintf(stderr, "[%4d] FILHO: Erro a executar o jogo", pid);
        exit(10);
    }
    close(canalLeitura[1]);
    close(canalEscrita[0]);

    // Troca de dados com o cliente

    wait(&estado);
    if(WIFEXITED(estado))
        printf("%d\n\n", WEXITSTATUS(estado));
}

int main(int argc, char* argv[]) {

/*    setbuf(stdout, NULL);

    int MAXPLAYERS, DURATION = -1, WAIT = -1;
    char GAMEDIR[50];

    initialization(GAMEDIR, &MAXPLAYERS, &DURATION, &WAIT, argc, *argv);

    // PRINT INITIALIZATION
    printf("Configuracoes do Arbitro: \n");
    printf("GAMEDIR: %s\tMAXPLAYERS: %d\n", GAMEDIR, MAXPLAYERS);
    printf("DURATION: %d\tWAIT:%d\n", DURATION, WAIT);*/

    // msg_jogo jogo1 = criaJogo();
    criaJogo();
    
    exit(EXIT_SUCCESS);
}
