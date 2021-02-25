/*
 * FILE: perguntas.c
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
#include <signal.h>
#include <unistd.h>

#include "perguntas.h"
#include "../sinais.h"

#define MAXPERGUNTAS 50

int pontuacao = 0;

int main() {

    // setbuf(stdout, NULL);

    // Inicializacao da lista de perguntas
    perguntas lista[MAXPERGUNTAS] = {
            {"Quem foi o primero Rei de Portugal?",
                    "D. Manuel I",
                    "D. Filipe II",
                    "D. Sancho",
                    "D. Afonso Henriques",
                    4},

            {"Que matematico e geografo grego calculou o perimetro da Terra em 240 a.C.?",
                    "Descartes",
                    "Erastotenes",
                    "Sofocles",
                    "Socrates",
                    2},

            {"Quanto tempo demora a Lua a dar a volta à Terra (período orbital)?",
                    "27 dias e 8 horas",
                    "30 dias",
                    "30 dias",
                    "29 dias e 12 horas",
                    1},

            {"Que eletrodoméstico foi criado por Murray Spangler em 1907 e que ainda hoje é vendido praticamente inalterado?",
                    "Esquentador",
                    "Torradeira",
                    "Micro-ondas",
                    "Aspirador",
                    4 },

            {"Qual é o objeto de estudo do puericultor?",
                    "Crianças",
                    "Coelhos",
                    "Abelhas",
                    "Galinhas",
                    1 },

            {"Que país sul-americano é o maior produtor de vinho?",
                    "Brasil",
                    "Venezuela",
                    "Canadá",
                    "Argentina",
                    4 },

            {"Qual foi a primeira moeda a ser usada em toda a Europa Ocidental?",
                    "ECU",
                    "Euro",
                    "Sestércio",
                    "Dracma",
                    3 },

            {"Que palavra de origem inuíte significa casa?",
                    "Quiosque",
                    "Caiaque",
                    "Anoraque",
                    "Iglu",
                    4 },

            {"Qual é o símbolo Químico Da Prata?",
                    "Au",
                    "Pt",
                    "Ag" ,
                    "Zn",
                    3 },

            { "Quem foi o primeiro vencedor do Prêmio Nobel de Literatura?",
                    "Um ensaísta",
                    "Um dramaturgo",
                    "Um novelista",
                    "Um poeta",
                    4 },

            {"Como se chamou o Sporting Club de Portugal entre 1904 e 1906?",
                    "Sporting de Portugal",
                    "Não teve nome",
                    "Leões da Estrela",
                    "Campo Grande Football Club",
                    4},

            {"Em Portugal, a que cidadãos foi retirado o direito de voto em 1913?",
                    "Mulheres",
                    "Analfabetos",
                    "Emigrantes",
                    "Idosos",
                    2 },

            {"Que parque natural na região de Bragança foi criado em 1979?",
                    "Peneda-Gerês",
                    "Montesinho",
                    "Alto Douro",
                    "Arrábida",
                    2 },

            {"Que modalidade desportiva foi introduzida em Portugal Continental em 1888 por Guilherme Pinto Basto?",
                    "Futebol",
                    "Corfebol",
                    "Andebol",
                    "Ténis",
                    1},

            { "De acordo com a normal-padrão da língua portuguesa , qual é o plural de troféu?",
                    "Troféus",
                    "Troféis",
                    "Trofés",
                    "Troféls",
                    1},

            { "A resolução 4K, também conhecida como Ultra HD, proporciona telas com a definição de quantos pixels?",
                    " 1920 x 1080",
                    " 7860 x 4320",
                    "3840 x 2160",
                    "2560 x 1440",
                    3},

            {"Nephelococcygia se refere ao hábito de:",
                    "Nadar em água congelada",
                    "Identificar formatos em nuvens",
                    "Dormir com os olhos abertos",
                    "Quebrar vidros com o som da voz",
                    2},

            {"Qual é a classificação dada ao medicamento obtido exclusivamente da matéria-prima vegetal?",
                    "Fitoterápico",
                    "Alopático",
                    "Genérico",
                    "Homeopático",
                    1},

            {"O estado do Alasca pertence a qual destes países deste 1867?",
                    "Estados Unidos",
                    "Canadá",
                    "África do Sul",
                    "Rússia",
                    1},

            {"O programa desenvolvido por empresa de segurança, com o objetivo de eliminar e detectar vírus no computador, e chamado de:",
                    "Firefox",
                    "Antivirus",
                    "Firemall",
                    "Antiwall",
                    2},

            { "Em média quantas pesquisas totalmente novas são feitas no Google por dia?",
                    "450 milhoes",
                    "1 bilhão",
                    "280 milhões",
                    "10 milhões",
                    1},

            {"Qual foi a primeira rede social da história da intenet?",
                    "Classmate",
                    "Myspace",
                    "Orkut",
                    "Facebook",
                    1},

            {"Quando foi criado o primeiro smarthphone da história?",
                    "1994" ,
                    "2000",
                    "2001",
                    "1998",
                    1},

            {"Quantos bits cabem em um byte?",
                    "8 bits",
                    "1 bits",
                    "2 bits",
                    "12 bits",
                    1},

            {"Qual foi a duração do primeiro vídeo do Youtube?",
                    "1 minuto",
                    "10 segundos",
                    "3 minutos",
                    "18 segundos",
                    4},

            {"Não  é  considerado  um   elemento  de  rede : ",
                    "Memória",
                    "Dispositivos",
                    "Meio físico",
                    "Regras e protocolo",
                    1 },

            {"Qual destes jogadores foi formado no Sporting?",
                    "Nuno Gomes",
                    "Petit",
                    "Simão",
                    "Maniche",
                    3 },

            {"Normalmente, quantos litros de sangue uma pessoa tem? Em média, quantos são retirados numa doação de sangue?",
                    "Tem entre 2 a 4 litros. São retirados 450 mililitros",
                    "Tem entre 4 a 6 litros. São retirados 450 mililitros",
                    "Tem 0,5 litros. São retirados 0,5 litros",
                    "Tem 7 litros. São retirados 1,5 litros",
                    2 },

            {" De quem é a famosa frase “Penso, logo existo”? ",
                    "Platão",
                    "Galileu Galilei",
                    "Descartes",
                    "Sócrates",
                    3},

            {"Qual o menor e o maior país do mundo?",
                    "Vaticano e Rússia",
                    "Nauru e China",
                    "Mônaco e Canadá",
                    "Malta e Estados Unidos",
                    1},

            {"Quantas casas decimais tem o número pi?",
                    "Duas",
                    "Centenas",
                    "Infinitas",
                    "Milhares",
                    3 },

            {"Atualmente, quantos elementos químicos a tabela periódica possui?",
                    "113",
                    "109",
                    "108",
                    "118",
                    4},

            {"Qual o maior animal terrestre?",
                    "Baleia Azul",
                    "Dinossauro",
                    "Elefante africano",
                    "Tubarão Branco",
                    3},

            {"Quais os nomes dos três Reis Magos?",
                    "Gaspar, Nicolau e Natanael",
                    "Belchior, Gaspar e Baltazar",
                    "Belchior, Gaspar e Nataniel",
                    "Gabriel, Benjamim e Melchior",
                    2},

            { "Quais os principais heterônimos de Fernando Pessoa?",
                    "Alberto Caeiro, Ricardo Reis e Álvaro de Campos",
                    "Ariano Suassuna, Raul Bopp e Quincas Borba",
                    "Bento Teixeira, Ricardo Reis e Haroldo de Campos",
                    "Alberto Caeiro, Ricardo Leite e Augusto de Campos",
                    1 },

            {"As pessoas de qual tipo sanguíneo são consideradas doadores universais?",
                    " Tipo A",
                    "Tipo B",
                    "Tipo O ",
                    "Tipo AB",
                    3},

            {"Qual o metal cujo símbolo químico é o Au?",
                    "Cobre",
                    "Prata",
                    "Mercúrio",
                    "Ouro",
                    4 },

            { "Em que oceano fica Madagascar?",
                    "Oceano Índico",
                    "Oceano Antártico",
                    "Oceano Atlântico",
                    "Oceano Pacífico",
                    1 },

            { "Quais são os cromossomos que determinam o sexo masculino?",
                    "Os V",
                    "Os X",
                    "Os Y",
                    "Os W",
                    3 } ,

            {"Quem foi o primeiro homem a pisar na Lua? Em que ano aconteceu?",
                    "Yuri Gagarin, em 1961",
                    "Buzz Aldrin, em 1969",
                    "Charles Conrad, em 1969",
                    "Neil Armstrong, em 1969",
                    4},

            {"Qual a religião monoteísta que conta com o maior número de adeptos no mundo",
                    " Judaísmo",
                    "Zoroastrismo",
                    " Islamismo",
                    "Cristianismo",
                    4 },
            {"De onde é a invenção do chuveiro elétrico?",
                    "França",
                    "Inglaterra",
                    "Brasil",
                    "Austrália",
                    3 },

            { "Quantos braços tem um polvo?",
                    "Seis",
                    "Oito",
                    "Dez",
                    "Sete",
                    2},

            {"De que são constituídos os diamantes?",
                    "Grafite",
                    "Rênio",
                    "Carbono",
                    "Ósmio",
                    1 },

            { "As cantigas de Escárnio e Maldizer pertencem a qual escola literária?",
                    "Classicismo",
                    "Barroco",
                    "Quinhentismo",
                    "Trovadorismo",
                    4 },

            { "Qual a unidade que mede a intensidade do som?",
                    "Decibel",
                    "Frequência",
                    "Hertz",
                    "Compasso",
                    1},

            {"Que personagem da mitologia grega era metade homem metade cavalo?",
                    "Centauro",
                    "Sátiro",
                    "Górgona",
                    "Gigante",
                    1},

            {"Em que lugar vivem mais cangurus do que pessoas?",
                    "Indonésia",
                    "Nova Zelândia",
                    "Austrália",
                    "Papua-Nova Guiné",
                    3 },

            {"Qual destas, apesar do seu nome, não é considerada um tipo de força?",
                    "Força de atrito",
                    "Força peso",
                    "Força centrípeta",
                    "Força eletromotriz",
                    4},

            {"O que mede a Escala Mercalli?",
                    "A intensidade dos ventos",
                    "A intensidade sísmica de acordo com os danos provocados",
                    "A resistência dos minerais",
                    "A magnitude de um terremoto",
                    2},


    };

    // Manipulacao de Sinais
    struct sigaction actionEndGame;
    actionEndGame.sa_sigaction = endGame;
    actionEndGame.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &actionEndGame, NULL);

    // DESCRICAO
    printf("                      JOGO DAS PERGUNTAS\n"
    	   "O jogo consiste em acertar o maior numero de perguntas possivel.\n"
           "Sera apresentada a questao, seguida de 4 hipoteses de escolha. \n"
           "Para responder basta indicar o numero da solucao pretendida.\n");

    // Base do Jogo
    int count = 0;
    while(1) {
        for (int i = 0; i < MAXPERGUNTAS; ++count, ++i) {
            puts("-------------------------------------------------------------");
            printf("Pergunta %d\tPontuacao: %d\n", count + 1, pontuacao);
            printf("QUESTAO: %s\n", lista[i].pergunta);
            printf("1: %s\t2: %s\n3: %s\t4: %s\n\n",
                   lista[i].respostas[0], lista[i].respostas[1],
                   lista[i].respostas[2], lista[i].respostas[3]);

            fflush(stdout);

            int resposta, n;
            do {
                n = 0;
                n = scanf("%d", &resposta);
            } while (n == 0 || (resposta > 4 || resposta < 1));

            if (n == 1) {
                if (resposta == lista[i].solucao) {
                    printf("Resposta Correta!\n");
                    fflush(stdout);
                    pontuacao++;
                } else {
                    printf("Resposta Errada!\n");
                    fflush(stdout);
                    if (pontuacao > 0)
                        pontuacao--;
                }
            }
        }
    }
}
