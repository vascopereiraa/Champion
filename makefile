all: jogo.o arbitro.o cliente.o
<<<<<<< HEAD
	gcc ./Games/perguntas.o ./Games/sinais.o -o ./Games/g_perguntas
	gcc ./Arbitro/arbitro.o -o arbitro
=======
	gcc ./Games/perguntas.o -o ./Games/g_perguntas
	gcc ./Arbitro/arbitro.o ./Arbitro/initConfig.o ./Arbitro/comunicacao.o  -o arbitro
>>>>>>> origin/Arbitro&Cliente
	gcc ./Cliente/cliente.o -o cliente

jogo.o: ./Games/Perguntas/perguntas.c ./Games/Perguntas/perguntas.h ./Games/Perguntas/sinais.h ./Games/Perguntas/sinais.c
	gcc -c ./Games/Perguntas/perguntas.c -o ./Games/perguntas.o
	gcc -c ./Games/Perguntas/sinais.c -o ./Games/sinais.o

arbitro.o: ./Arbitro/arbitro.c ./Arbitro/initConfig.c  ./Arbitro/initConfig.h ./Arbitro/comunicacao.c ./Arbitro/comunicacao.h
	gcc -c ./Arbitro/arbitro.c -o ./Arbitro/arbitro.o
	gcc -c ./Arbitro/initConfig.c -o ./Arbitro/initConfig.o
	gcc -c ./Arbitro/comunicacao.c -o ./Arbitro/comunicacao.o

cliente.o: ./Cliente/cliente.c ./Cliente/cliente.h
	gcc -c ./Cliente/cliente.c -o ./Cliente/cliente.o

jogo: jogo.o
	gcc ./Games/perguntas.o ./Games/sinais.o -o ./Games/g_perguntas

arbitro: arbitro.o
	gcc ./Arbitro/arbitro.o ./Arbitro/initConfig.o ./Arbitro/comunicacao.o  -o arbitro

cliente: cliente.o
	gcc ./Cliente/cliente.o -o cliente

clean:
	find . -name '*.o' -delete
