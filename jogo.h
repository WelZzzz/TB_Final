#ifndef JOGO_H
#define JOGO_H

// Constantes da interface grafica
#define MAX_LINHAS 20
#define MAX_COLUNAS 40
#define MIN_LINHAS 6
#define MIN_COLUNAS 8
#define TAM_MSG 80
#define TAM_CASA 28
#define MARGEM 20
#define ALTURA_PAINEL 130
#define LARGURA_MINIMA 640
#define ALTURA_MINIMA 440

// Resultado da partida
#define EM_ANDAMENTO 0
#define VITORIA 1
#define DERROTA 2
#define DESISTENCIA 3

// Efeito dos eventos
#define BONUS_RECUPERACAO 5
#define DANO_ARMADILHA 4

// Struct que guarda o estado do jogo
typedef struct {
    int linhas, colunas; // Dimensoes lidas do arquivo
    int linhaJ, colunaJ; // Posicao atual do jogador
    int energia; // Energia inicial vem do arquivo
    int minimoCristais; // Lido do arquivo
    int cristais, turnos;
    char mensagem[TAM_MSG]; // Ultima mensagem mostrada na tela 
    char mapa[MAX_LINHAS][MAX_COLUNAS]; 
} Jogo;

/* mapa.c */
void desenharJogo(Jogo *j);
int carregarMapa(const char *nomeArquivo, Jogo *j); // 1 = ok, 0 = erro 
int movimentoValido(Jogo *j, int comando);
int larguraJanela(Jogo *j);
int alturaJanela(Jogo *j);

/* jogador.c */
void moverJogador(int *linha, int *coluna, int comando);

/* eventos.c */
void processarCasa(Jogo *j); // Cristal/evento; atualiza mensagem
void desenharFimDeJogo(Jogo *j, int resultado);
int chegouNaSaida(Jogo *j);

#endif