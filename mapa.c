#include <raylib.h>
#include "jogo.h"

/*
* Verifica se o movimento do jogador é válido (não colide com paredes e está dentro dos limites do mapa).
*/
int movimentoValido(Jogo *j, int comando){
    int novaLinha = (*j).linhaJ;
    int novaColuna = (*j).colunaJ;

    switch (comando) {
        case KEY_UP:
            novaLinha--;
            break;
        case KEY_DOWN:
            novaLinha++;
            break;
        case KEY_LEFT:
            novaColuna--;
            break;
        case KEY_RIGHT:
            novaColuna++;
            break;
        default:
            return 0; // Comando inválido
    }

    // Verifica se a nova posição está dentro dos limites do mapa
    if (novaLinha >= 0 && novaLinha < (*j).linhas && novaColuna >= 0 && novaColuna < (*j).colunas) {
        // Verifica se a nova posição não é uma parede
        if ((*j).mapa[novaLinha][novaColuna] != '#') {
            return 1; // Movimento válido
        }
    }

    return 0; // Movimento inválido
}