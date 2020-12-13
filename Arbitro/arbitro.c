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
#include <sys/select.h>
#include <ctype.h>
#include <time.h>

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

    int tam;
    for(int i = 0; i < (*nJogos); ++i) {
        tam = (int) strlen(jogos[i]);
        printf("%c", toupper(jogos[i][2]));
        for(int j = 3; j < tam; ++j)
            printf("%c", jogos[i][j]);
        printf("\n");
    }
}

void libertarJogos(char** jogos, int* nJogos) {

    for(int i = 0; i < (*nJogos); ++i)
        free(jogos[i]);
    (*nJogos) = 0;

}

// Envia ao Cliente o nome do jogo que tem associado
void obtemJogoCliente(comCliente* coms, info* jogadores, const int* nJogadores) {

    for(int i = 0; i < (*nJogadores); ++i)
        if(jogadores[i].pidCliente == coms->pid) {
            strcpy(coms->mensagem, jogadores[i].nomeJogo);
            coms->cdgErro = 2;
        }
}

char* sorteiaJogos(char** jogos, const int* nJogos) {

    int valor;
    srand(time(NULL));

    if((*nJogos) == 0) {
        printf("[ERRO] Nao exitem jogos disponiveis para sortear!");
        return "  ERRO NA PASTA GAMEDIR";
    }

    do {
        valor = rand() % (*nJogos);
        if (valor >= 0 && valor <= (*nJogos))
            return jogos[valor];
    } while (valor < 0 || valor > (*nJogos));

    return " ";
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
        int posicao, tam = (int) strlen(comando);
        char *jogadorRemover = (char*) malloc(sizeof(char) * (tam - 1));
        if(jogadorRemover != NULL) {
            for (int j = 1; j < tam; ++j)
                jogadorRemover[j-1] = (char) tolower(comando[j]);

            jogadorRemover[tam-1] = '\0';
            // printf("Nome a remover: %s\n", jogadorRemover);
            posicao = existeJogador(jogadores, nJogadores, jogadorRemover);
            if (posicao == -1) {
                printf("[ERRO] O jogador %s nao se encontra no campeonato\n", jogadorRemover);
                 free(jogadorRemover);
                return ;
            }

            // Guardar Named Pipe do Cliente e o seu nome
            char pipeCli[200];
            strcpy(pipeCli, jogadores[posicao].pipeCliente);
            /* unsigned int pidJogo = jogadores[posicao].pidJogo; */
            if(removeJogador((info **) &jogadores, nJogadores, &posicao) == 1) {
                /* TERMINAR O CLIENTE E JOGO */
                printf("[AVISO] O jogador %s foi removido com sucesso!", jogadorRemover);

                // Mandar mensagem ao cliente para terminar a execucao
                comCliente coms;
                strcpy(coms.nomeJogador, jogadorRemover);
                strcpy(coms.pipeCliente, pipeCli);
                coms.cdgErro = 4;
                enviaMensagemCliente(&coms);

                /* signalTerminaExecucao(&pidJogo, valor); */

                free(jogadorRemover);
                return ;
            }
            printf("[ERRO] Nao foi possivel remover o jogador %s\n", jogadorRemover);
            free(jogadorRemover);
            return;
        }
    }

    if(strcmp(comando, "clear") == 0) {
        system("clear");
        return ;
    }

    puts("[AVISO] O comando pretendido nao se encontra definido\n");
}

void trataComandosCliente(comCliente* coms, info* jogadores, int* nJogadores) {

    if(strcmp(coms->resposta, "#mygame") == 0) {
        obtemJogoCliente(coms, jogadores, nJogadores);
    }

    if(strcmp(coms->resposta, "#quit") == 0) {
        char nomeJogador[200];
        int posicao = existeJogador(jogadores, nJogadores, coms->nomeJogador);
        // unsigned int pidJogo = jogadores[posicao].pidJogo;
        strcpy(nomeJogador, jogadores[posicao].nomeJogador);
        if(removeJogador((info**) &jogadores, nJogadores, &posicao) == 1) {
            printf("[AVISO] O jogador %s desistiu do campeonato\n", nomeJogador);
            // Enviar sinal ao jogo para terminar
        }
    }

}

int main(int argc, char* argv[]) {

    int fd, res, n;
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
    verificaLocalPipes();
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
                // Ler informacao passada pelo Pipe
                n = read(fd, &coms, sizeof(comCliente));

                // Verifica se o cliente já está registado no arbitro (Nomes iguais)
                if(verificaNomeCliente(jogadores, &nJogadores, &coms) == 0) {
                   // Verifica se excede o numero maximo de jogadores
                   if (nJogadores < setup.MAXPLAYERS)
                       jogadores = adicionaCliente(jogadores, &nJogadores, &coms, jogos, &nJogos);
                   else {
                       coms.cdgErro = 5;
                       enviaMensagemCliente(&coms);
                   }
                }

                // Caso o nome do Cliente seja igual a um nome ja registado
                else {
                   // PID do Cliente ainda nao existe na lista
                   if(verificaPidCliente(jogadores, &nJogadores, &coms) == 0)
                       coms.cdgErro = 1;
                   // Caso o PID do Cliente já exista na lista de jogadores
                   else
                       coms.cdgErro = 0;
                }

                // Tratamento da resposta do Cliente
                if(coms.resposta[0] == '#') {
                    // Mensagem destinada ao Arbitro
                    trataComandosCliente(&coms, jogadores, &nJogadores);
                }
                else {
                    // Mensagem destinada ao Jogo
                    printf("\n%s", coms.resposta);
                }

                // Envia mensagem ao Cliente
                enviaMensagemCliente(&coms);

            }
    } while(strcmp(cmd, "exit") != 0);

    // Avisar os Clientes e Jogos que o Arbitro encerrou
    printf("O Arbitro encerrou a sua execucao!\n");
    /* AVISAR TODOS DO TERMINO DO ARBITRO -->> Enviar sinal aos clientes e jogos */
    terminaTodosClientes(jogadores, &nJogadores, 3);

    // Fechar o NamedPipe do Arbitro
    close(fd);
    unlink(FIFO_ARB);
    remove(FIFO_ARB);

    // Apaga dados em memoria dinamica
    free(setup.GAMEDIR);
    libertaJogadores(jogadores, &nJogadores);
    libertarJogos(jogos, &nJogos);

    exit(EXIT_SUCCESS);
}

/* printf("Enviei: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",coms.pid, coms.nomeJogador,
                       coms.mensagem, coms.resposta, coms.cdgErro, coms.pipeCliente, coms.pontuacao); */