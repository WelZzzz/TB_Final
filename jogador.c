#include <raylib.h>
#include "jogo.h"

/*
* Move o jogador de acordo com o comando (tecla) pressionado.
*/
void moverJogador(int *linha, int *coluna, int comando){
    switch (comando) {
        case KEY_UP:
            (*linha)--;
            break;
        case KEY_DOWN:
            (*linha)++;
            break;
        case KEY_LEFT:
            (*coluna)--;
            break;
        case KEY_RIGHT:
            (*coluna)++;
            break;
    }
};

/*
* Redesenha a tela a cada quadro ate uma tecla ser pressionada e a retorna.
* telaFinal = 0 desenha a exploracao; 1 desenha a tela de fim de jogo.
* Se a janela for fechada pelo X, retorna KEY_Q (sair).
*/
int esperarTecla(Jogo *j, int telaFinal, int resultado) {
    int tecla;
    while (!WindowShouldClose()) {
        BeginDrawing();
        if (telaFinal) {
            mostrarFimDeJogo(j, resultado);
        } else {
            desenharJogo(j);
        }
        EndDrawing();
        tecla = GetKeyPressed(); /* letras chegam sempre em maiuscula */
        if (tecla != 0) {
            return tecla;
        }
    }
    return KEY_Q;
}
