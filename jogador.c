#include <raylib.h>
#include "jogo.h"

/*
* Move o jogador de acordo com o comando (tecla) pressionado.
*/
void moverJogador(int *linha, int *coluna, int comando){
    switch (comando) {
        case KEY_W:
        case KEY_UP:
            (*linha)--;
            break;
        case KEY_S:
        case KEY_DOWN:
            (*linha)++;
            break;
        case KEY_A:
        case KEY_LEFT:
            (*coluna)--;
            break;
        case KEY_D:
        case KEY_RIGHT:
            (*coluna)++;
            break;

    }
};
