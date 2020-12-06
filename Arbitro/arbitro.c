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
#include <sys/select.h>
#include <fcntl.h>
#include <ctype.h>

#include "initConfig.h"
#include "../Cliente/cliente.h"
#include "comunicacao.h"
#include "arbitro.h"

char** obtemJogos(char** jogos, int* nJogos, const init* setup) {

    DIR* dir;
    struct dirent* conteudo;
    char** array = NULL;

    // Abrir pasta
    dir = opendir(setup->GAMEDIR);
    if(dir == NULL) {
        fprintf(stderr,"\n[ERRO] Nao foi possivel aceder 'a diretoria %s\n", setup->GAMEDIR);
        exit(1);
    }

    // Listar todos os jogos na pasta
    while((conteudo = readdir(dir)) != NULL) {
        if(conteudo->d_name[0] == 'g' && conteudo->d_name[1] == '_') {

            /* MEMORIA DINAMICA - ARRAY BIDIMENSIONAL */
            array = (char**) realloc(jogos, sizeof(char*) * ((*nJogos)+1));
            if(array == NULL) {
                fprintf(stderr, "[ERRO] Erro ao adicionar um jogo na lista de Jogos!\n");
                exit(1);
            }

            jogos = array;

            int len = (int) strlen(conteudo->d_name);
            char* nomeJogo = NULL;
            nomeJogo = (char*) malloc(len);
            if(nomeJogo == NULL) {
                fprintf(stderr, "[ERRO] Erro ao adicionar um jogo na lista de Jogos!\n");
                exit(1);
            }

            jogos[(*nJogos)] = nomeJogo;
            strcpy(jogos[(*nJogos)], conteudo->d_name);
            ++(*nJogos);
        }
    }

    closedir(dir);
    return jogos;
}

void listaJogos(const char** jogos, const int* nJogos) {

    int tam = 0;
    for(int i = 0; i < (*nJogos); ++i) {
        tam = (int) strlen(jogos[i]);
        printf("%c", toupper(jogos[i][2]));
        for(int j = 3; j < tam; ++j)
            printf("%c", jogos[i][j]);
        printf("\n");
    }
}

void gestorComandos(char* comando, info* jogadores, int* nJogadores, const char** jogos,
                    const int* nJogos) {

    if(strcmp(comando, "players") == 0) {
        listaJogadores(jogadores, nJogadores);
        return ;
    }

    if(strcmp(comando, "games") == 0) {
        listaJogos(jogos, nJogos);
        return;
    }

    if(comando[0] == 'k') {
        int posicao, tam = strlen(comando);
        char *jogadorRemover = malloc(sizeof(char) * (strlen(comando) -1 ));
        if(jogadorRemover != NULL) {
            for (int j = 1; j < tam; ++j)
                jogadorRemover[j-1] = (char) tolower(comando[j]);

            jogadorRemover[tam-1] = '\0';
            posicao = existeJogador(jogadores, nJogadores, jogadorRemover);
            if (posicao == -1) {
                printf("[ERRO] O jogador %s nao se encontra no campeonato\n", jogadorRemover);
                return ;
            }

            printf("\nPosicao: %d\n", posicao);
            removeJogador((info **) &jogadores, nJogadores, &posicao);
            return;
        }
    }

    puts("[AVISO] O comando pretendido nao se encontra definido\n");
}

void libertarJogos(char** jogos, int* nJogos) {

    for(int i = 0; i < (*nJogos); ++i)
        free(jogos[i]);

}

int main(int argc, char* argv[]) {

    int fd, fdr, res, n;
    char cmd[200], **jogos = NULL;
    comCliente coms;
    info* jogadores = NULL;
    int nJogadores = 0, nJogos = 0;


    // Definicao do Ambiente
    const init setup = initialization(argc, argv);
    printInit(setup);

    // Carregar a lista de jogos
    jogos = obtemJogos(jogos, &nJogos, &setup);

    // Criacao do NamedPipe do Arbitro
    fd_set fds;
    fd = criaPipeArbitro(&fds);

    do {
        printf("\nComando: ");
        fflush(stdout);

        // Definicao dos descritores abertos
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(fd, &fds);

        res = select(fd + 1, &fds, NULL, NULL, NULL);
        if (res > 0 && FD_ISSET(0, &fds)) {
            scanf("%s", cmd);
            gestorComandos(cmd, jogadores, &nJogadores, (const char**) jogos, &nJogos);
        }
        else
            if (res > 0 && FD_ISSET(fd, &fds)) {
               n = read(fd, &coms, sizeof(comCliente));
               /* printf("Recebi: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",coms.pid, coms.nomeJogador,
                       coms.mensagem, coms.resposta, coms.cdgErro, coms.pipeCliente, coms.pontuacao); */

               if(verificaCliente(jogadores, &nJogadores, &coms) == 0 && nJogadores < setup.MAXPLAYERS)
                   jogadores = adicionaCliente(jogadores, &nJogadores, &coms);

               strcpy(coms.mensagem, "ok!");
               coms.cdgErro = 0;
               fdr = open(coms.pipeCliente, O_WRONLY);
               n = write(fdr, &coms, sizeof(comCliente));
               close(fdr);
               puts(" ");
               printf("Enviei: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",coms.pid, coms.nomeJogador,
                       coms.mensagem, coms.resposta, coms.cdgErro, coms.pipeCliente, coms.pontuacao);
            }
    } while(strcmp(cmd, "exit") != 0);

    // Avisar os Clientes que o Arbitro encerrou
    printf("O Arbitro encerrou a sua execucao!\n");
    /* AVISAR TODOS DO TERMINO DO ARBITRO */

    // Fechar o NamedPipe do Arbitro
    close(fd);
    unlink(FIFO_ARB);

    // Apaga dados em memoria dinamica
    free(setup.GAMEDIR);
    free(jogadores);
    libertarJogos(jogos, &nJogos);

    exit(EXIT_SUCCESS);
}
