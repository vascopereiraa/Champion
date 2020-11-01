all: jogo.o arbitro.o cliente.o
	gcc ./Games/perguntas.o -o ./Games/g_perguntas
	gcc ./Arbitro/arbitro.o -o arbitro
	gcc ./Cliente/cliente.o -o cliente

jogo.o: ./Games/Perguntas/perguntas.c ./Games/Perguntas/perguntas.h
	gcc -c ./Games/Perguntas/perguntas.c -o ./Games/perguntas.o

arbitro.o: ./Arbitro/arbitro.c ./Arbitro/arbitro.h
	gcc -c ./Arbitro/arbitro.c -o ./Arbitro/arbitro.o

cliente.o: ./Cliente/cliente.c
	gcc -c ./Cliente/cliente.c -o ./Cliente/cliente.o

clean:
	find . -name '*.o' -delete
