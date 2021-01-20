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
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include "initConfig.h"
#include "../Cliente/cliente.h"
#include "comunicacao.h"
#include "arbitro.h"

init setup;
info* jogadores;
int nJogadores;
int decorreCampeonato;
pthread_t temporizador;
pthread_t mainThread;

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
void obtemJogoCliente(comCliente* coms, info* jogadores) {

    info* aux = jogadores;

    while(aux != NULL && aux->pidCliente != coms->pid)
        aux = aux->next;
    // Envia mensagem ao Cliente
    if(aux != NULL) {
        strcpy(coms->mensagem, aux->nomeJogo);
        coms->cdgErro = 2;
    }
}

char* sorteiaJogos(char** jogos, const int* nJogos) {

    int valor;

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

void terminaTodasThreads(info* jogadores) {

    info* aux = jogadores;
    while(aux != NULL) {
        aux->abort = 1;
        pthread_kill(*(aux->thread), SIGUSR2);
        pthread_join(*(aux->thread), NULL);
        aux = aux->next;
    }

}

void gestorComandos(char* comando, info* jogadores, int* nJogadores, const char** jogos,
                    const int* nJogos) {

    if(strcmp(comando, "players") == 0) {
        listaJogadores(jogadores);
        return ;
    }

    if(strcmp(comando, "games") == 0) {
        listaJogos(jogos, nJogos);
        return ;
    }

    if(comando[0] == 'k') {
        int tam = (int) strlen(comando);
        char *jogadorRemover = (char*) malloc(sizeof(char) * (tam - 1));
        if(jogadorRemover != NULL) {
            for (int j = 1; j < tam; ++j)
                jogadorRemover[j-1] = (char) tolower(comando[j]);
            jogadorRemover[tam-1] = '\0';

            // Guardar Named Pipe do Cliente e o seu nome
            char pipeCli[200];
            strcpy(pipeCli, obtemNamedPipeCliente(jogadores, jogadorRemover));
            if(verificaNomeCliente(jogadores, jogadorRemover) == 0) {
                int res;
                removeJogador(nJogadores, jogadorRemover, &res);
                if ( res == 1 ) {
                    /* TERMINAR O CLIENTE E JOGO */
                    printf("[AVISO] O jogador %s foi removido com sucesso!", jogadorRemover);

                    // Mandar mensagem ao cliente para terminar a execucao
                    comCliente coms;
                    strcpy(coms.nomeJogador, jogadorRemover);
                    strcpy(coms.pipeCliente, pipeCli);
                    coms.cdgErro = 4;
                    enviaMensagemCliente(&coms);

                    // Verifica condicao de fim de campeonato
                    if(*nJogadores == 1 && decorreCampeonato != 0){
                        terminaCampeonato();
                    }

                    free(jogadorRemover);
                    return ;
                }
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

    if(comando[0] == 's') {
        int tam = (int) strlen(comando);
        char *nomeJogador = (char *) malloc(sizeof(char) * (tam - 1));
        if (nomeJogador != NULL) {
            for (int j = 1; j < tam; ++j)
                nomeJogador[j - 1] = (char) tolower(comando[j]);
            nomeJogador[tam - 1] = '\0';

            if (verificaNomeCliente(jogadores, nomeJogador) == 0) {
                if (intComsCliente(jogadores, nomeJogador) == 1) {
                    printf("As comunicacoes do Jogador %s foram suspensas\n",
                           nomeJogador);

                    // Informa suspensao das comunicacoes ao cliente
                    comCliente coms;
                    coms.pid = obtemPidCliente(jogadores, nomeJogador);
                    strcpy(coms.nomeJogador, nomeJogador);
                    strcpy(coms.pipeCliente, obtemNamedPipeCliente(jogadores, nomeJogador));
                    strcpy(coms.pipeArbitro, obtemNamedPipeThread(jogadores, nomeJogador));
                    coms.cdgErro = 7;
                    enviaMensagemCliente(&coms);

                    free(nomeJogador);
                    return ;
                }
                else {
                    printf("[AVISO] As comunicacoes do jogador %s ja se encontravam suspensas\n",
                           nomeJogador);
                    free(nomeJogador);
                    return ;
                }
            }
            else {
                printf("[ERRO] O jogador %s nao se encontra a disputar o Campeonato\n",
                       nomeJogador);
                free(nomeJogador);
                return ;
            }
        }
        free(nomeJogador);
        return ;
    }

    if(comando[0] == 'r') {
        int tam = (int) strlen(comando);
        char *nomeJogador = (char *) malloc(sizeof(char) * (tam - 1));
        if (nomeJogador != NULL) {
            for (int j = 1; j < tam; ++j)
                nomeJogador[j - 1] = (char) tolower(comando[j]);
            nomeJogador[tam - 1] = '\0';

            if (verificaNomeCliente(jogadores, nomeJogador) == 0) {
                if(retComsCliente(jogadores, nomeJogador) == 1) {
                    printf("As comunicacoes do Jogador %s foram retomadas\n",
                       nomeJogador);

                    // Mandar mensagem ao cliente para terminar a execucao
                    comCliente coms;
                    coms.pid = obtemPidCliente(jogadores, nomeJogador);
                    strcpy(coms.nomeJogador, nomeJogador);
                    strcpy(coms.pipeCliente, obtemNamedPipeCliente(jogadores, nomeJogador));
                    strcpy(coms.pipeArbitro, obtemNamedPipeThread(jogadores, nomeJogador));
                    coms.cdgErro = 8;
                    enviaMensagemCliente(&coms);

                    free(nomeJogador);
                    return ;
                }
                else {
                    printf("[AVISO] As comunicacoes do jogador %s ja se encontravam establecidas\n",
                           nomeJogador);
                    free(nomeJogador);
                    return ;
                }
            }
            else {
                printf("[ERRO] O jogador %s nao se encontra a disputar o Campeonato\n",
                       nomeJogador);
                free(nomeJogador);
                return ;
            }
        }
        free(nomeJogador);
        return ;
    }

    if(strcmp(comando, "end") == 0) {
        if(decorreCampeonato != 0) {
            // Fechar a thread do temporizador
            pthread_kill(temporizador, SIGUSR2);
            pthread_join(temporizador, NULL);
            decorreCampeonato = 2;
            strcpy(comando, " ");
            return;
        }
        else
            return ;
    }

    if(strcmp(comando, "exit") == 0) {
        if(decorreCampeonato != 0) {
            // Fechar a thread do temporizador
            pthread_kill(temporizador, SIGUSR2);
            pthread_join(temporizador, NULL);
            decorreCampeonato = 3;
            strcpy(comando, " ");
            return;
        }
        else
            return ;
    }

    puts("[AVISO] O comando pretendido nao se encontra definido\n");
}

void trataComandosCliente(comCliente* coms, int* nJogadores, info* jogadores) {

    if(strcmp(coms->resposta, "#mygame") == 0) {
        obtemJogoCliente(coms, jogadores);
    }

    if(strcmp(coms->resposta, "#quit") == 0) {
        char nomeJogador[200];
        strcpy(nomeJogador, coms->nomeJogador);
        int res;
        removeJogador(nJogadores, nomeJogador, &res);
        if(res == 1) {
            printf("[AVISO] O jogador %s desistiu do campeonato\n", nomeJogador);

            if(*nJogadores == 1 && decorreCampeonato != 0)
                terminaCampeonato();

        }
    }

}

void iniciaCampeonato() {

    // Marca inicio do campeonato
    decorreCampeonato = 1;

    // Verifica nr de jogadores
    if(nJogadores == 1)
        terminaCampeonato();

    // Inicia a comunicacao entre o Cliente e Jogo
    trocaIntComs(jogadores);

}

void terminaCampeonato() {

    // Marca o final do campeonato
    decorreCampeonato = 2;

    // Fechar todas as threads, fechar todos os jogos e guardar o exit status
    printf("O Campeonato chegou ao Fim!\n");
    terminaTodasThreads(jogadores);

    // Anunciar o vencedor
    acao action = fim;
    enviaMensagemTodosClientes(jogadores, action);

    info* vencedor = obtemVencedor(jogadores);
    printf("[FIM DO CAMPEONATO] O vencedor do Campeonato foi o %s com a "
           "pontuacao total de %d pontos\n", vencedor->nomeJogador, vencedor->pontuacao);

    // Envia sinal para desbloquar SELECT
    pthread_kill(mainThread, SIGUSR2);
}

int main(int argc, char* argv[]) {

    // Inicializacao do gerador de random
    srand(time(NULL));

    // Sinal para fechar a thread --> Salta SELECT
    struct sigaction actionUnlockSelect;
    actionUnlockSelect.sa_sigaction = signalThread;
    actionUnlockSelect.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &actionUnlockSelect, NULL);

    mainThread = pthread_self();

    int fd, res, n;
    char cmd[200], **jogos = NULL;
    comCliente coms;
    int nJogos = 0;
    jogadores = NULL;
    pthread_mutex_t trincoComunicacao;
    pthread_mutex_t trincoAddJogadores;
    int count = 0;
    unsigned int* clientesEspera;
    int nCliEspera = 0;

    // Definicao do Ambiente
    setup = initialization(argc, argv);

    // Carregar a lista de jogos
    jogos = obtemJogos(jogos, &nJogos, &setup);

    // Criacao do NamedPipe do Arbitro
    verificaLocalPipes();
    fd_set fds;
    fd = criaPipeArbitro(&fds, FIFO_ARB);

    // Iniciazacao dos Mutexes
    pthread_mutex_init(&trincoComunicacao, NULL);
    pthread_mutex_init(&trincoAddJogadores, NULL);

    do{
        decorreCampeonato = 0;
        nJogadores = 0;
        clientesEspera = NULL;
        // system("clear");
        printInit(setup);
        printf("\n\n\tBem-Vindo Administrador!\nArbitro do Campeonato nr. %d do Sistema Champion!\n\n", ++count);

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
                gestorComandos(cmd, jogadores, &nJogadores, (const char **) jogos, &nJogos);
            } else {
                if (res > 0 && FD_ISSET(fd, &fds)) {
                    // Ler informacao passada pelo Pipe
                    n = read(fd, &coms, sizeof(comCliente));
                    // Verifica se o cliente já está registado no arbitro (Nomes iguais)
                    if (verificaNomeCliente(jogadores, coms.nomeJogador) == 1) {
                        // Verifica se excede o numero maximo de jogadores
                        if (nJogadores < setup.MAXPLAYERS) {
                            // Se nao houver nenhum campeonato a decorrer
                            if (decorreCampeonato == 0) {

                                pthread_mutex_lock(&trincoAddJogadores);

                                // Adicionar o jogador a lista de jogadores & cria uma nova thread na lista de threads
                                jogadores = adicionaCliente(jogadores, &nJogadores, &coms, jogos, &nJogos);

                                // Mandar o Cliente trocar o canal de comunicacao
                                coms.cdgErro = 6;

                                jogadores->trinco = &trincoComunicacao;

                                // Aloca espaco para a thread
                                pthread_t *temp = (pthread_t *) malloc(sizeof(pthread_t));
                                if (temp == NULL) {
                                    fprintf(stderr, "[ERRO] Nao foi possivel acrescentar thread à lista de threads!\n");
                                    exit(9);
                                }
                                jogadores->thread = temp;

                                // Criar a thread
                                pthread_create(jogadores->thread, NULL, threadsClientes,
                                               (void *) jogadores);

                                // Verificar temporizacao
                                if (nJogadores == 2 && decorreCampeonato == 0)
                                    pthread_create(&temporizador, NULL, threadTemporizacao, NULL);


                                pthread_mutex_unlock(&trincoAddJogadores);
                            }
                                // Caso o jogador tenha as caracteristicas necessarias
                                // mas um campeonato encontra-se a decorrer
                            else {
                                printf("CLIENTE %s TENTOU ENTRAR!\n", coms.nomeJogador);

                                // ARRAY PID
                                unsigned int* temp = (unsigned int*) realloc(clientesEspera, sizeof(int) * (nCliEspera + 1));
                                if(temp == NULL) {
                                    printf("[ERRO] Nao foi possivel guardar jogadores a espera\n");
                                    exit(6);
                                }
                                clientesEspera = temp;

                                clientesEspera[nCliEspera++] = coms.pid;

                                // ENVIAR MSG P ESPERAR
                                coms.cdgErro = 11;
                                coms.pontuacao = setup.DURATION * 60;
                            }
                        } else {
                            coms.cdgErro = 5;
                        }
                    }
                    // Caso o nome do Cliente seja igual a um nome ja registado
                    else {
                        // PID do Cliente ainda nao existe na lista?
                        if (verificaPidCliente(jogadores, &coms) == 0)
                            coms.cdgErro = 1;
                            // Caso o PID do Cliente já exista na lista de jogadores
                        else
                            coms.cdgErro = 6;
                    }

                    // Envia mensagem ao Cliente
                    enviaMensagemCliente(&coms);

                }
            }

        } while (decorreCampeonato != 2 && (strcmp(cmd, "exit") != 0 && strcmp(cmd, "end") != 0));

        if(strcmp(cmd, "exit") == 0) {
            puts("\n\nEntrei no comando exit\n\n");
            decorreCampeonato = 3;
        }
        else {
            decorreCampeonato = 0;
            puts("\n\nEntrei no else do comando exit\n\n");
            // Apaga dados dos jogadores em memoria dinamica
            if (jogadores != NULL) {
                puts("\n\nLibertar jogadores\n\n");
                jogadores = libertaJogadores(jogadores, &nJogadores);
                nJogadores = 0;
            }
        }
        puts("\n\nPassei aqui 1\n\n");
        // Enviar sinal para acordar clientes a espera
        if (nCliEspera != 0 && clientesEspera != NULL) {
            puts("\n\nLibertar lista de espera\n\n");
            avisaClientesEspera(clientesEspera, nCliEspera);
            free(clientesEspera);
            nCliEspera = 0;
        }
        puts("\n\nPassei aqui 2\n\n");
        sleep(5);

        printf("\n\n decorreCampeonato = %d \n\n", decorreCampeonato);
    } while(decorreCampeonato != 3);

    // Avisar os Clientes e Jogos que o Arbitro encerrou
    printf("O Arbitro encerrou a sua execucao!\n");

    // Encerra os Clientes em jogo
    if(jogadores != NULL)
        terminaTodosClientes(jogadores, 3);

    // Apaga dados dos jogadores em memoria dinamica
    if (jogadores != NULL)
        jogadores = libertaJogadores(jogadores, &nJogadores);

    // Fechar o NamedPipe do Arbitro
    close(fd);
    unlink(FIFO_ARB);
    remove(FIFO_ARB);

    // Apaga dados constantes em memoria
    free(setup.GAMEDIR);
    libertarJogos(jogos, &nJogos);

    exit(EXIT_SUCCESS);
}

