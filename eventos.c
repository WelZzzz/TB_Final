#include <raylib.h>
#include <stdio.h>
#include "jogo.h"

/* Processa a casa atual do jogador, verificando se há um cristal, armadilha ou evento especial.*/

void processarCasa(Jogo *j){
    char casa = (*j).mapa[(*j).linhaJ][(*j).colunaJ];

    switch (casa) {
        case 'C': // Cristal
            (*j).cristais++;
            (*j).mapa[(*j).linhaJ][(*j).colunaJ] = '.'; // Remove o item do mapa
            snprintf((*j).mensagem, TAM_MSG, "Cristal coletado! (%d/%d)", (*j).cristais, (*j).minimoCristais);
            break;
        case 'R': // Recuperação
            (*j).energia += BONUS_RECUPERACAO; // Recupera 1 de energia
            (*j).mapa[(*j).linhaJ][(*j).colunaJ] = '.'; // Remove o item do mapa
            snprintf((*j).mensagem, TAM_MSG, "Fonte de energia encontrada! Energia atual: %d", (*j).energia);
            break;
        case 'A': // Armadilha
            if ((*j).energia > 0) { // Se energia > 0, perde 1 de energia. Evita com que energia fique negativa.
                (*j).energia -= DANO_ARMADILHA;
            }
            (*j).mapa[(*j).linhaJ][(*j).colunaJ] = '.'; // Remove o item do mapa
            snprintf((*j).mensagem, TAM_MSG, "Armadilha! Energia perdida. Energia atual: %d", (*j).energia);
            break;
        case 'E': // Evento especial
            // Criar evento
            (*j).mapa[(*j).linhaJ][(*j).colunaJ] = '.'; // Remove o item do mapa
            snprintf((*j).mensagem, TAM_MSG, "Evento especial encontrado!");
            break;
    }
}

int chegouNaSaida(Jogo *j){
    if ((*j).mapa[(*j).linhaJ][(*j).colunaJ] == 'S') {  // Verifica se o jogador chegou na saída
        if ((*j).cristais >= (*j).minimoCristais) {     // Verifica se o jogador coletou cristais suficientes
            snprintf((*j).mensagem, TAM_MSG, "Parabens! Voce coletou %d cristais e chegou na saida!", (*j).cristais);
            return VITORIA; // Vitória
        } else {
            snprintf((*j).mensagem, TAM_MSG, "Cristais insuficientes! (%d/%d)", (*j).cristais, (*j).minimoCristais);
        }
    }
    return EM_ANDAMENTO; // Jogo continua
}

void desenharFimDeJogo(Jogo *j, int resultado){
    int y = MARGEM + 20;

    // Resultado da partida
    if (resultado == VITORIA) {
        DrawText("VITORIA! Voce escapou da ilha.", MARGEM, y, 28, DARKGREEN);
    } else if (resultado == DERROTA) {
        DrawText("DERROTA - seu folego acabou.", MARGEM, y, 28, MAROON);
    } else {
        DrawText("DESISTENCIA - voce abandonou a expedicao.", MARGEM, y, 24, DARKBLUE);
    }

    // Resumo
    DrawText(TextFormat("Cristais: %d (minimo exigido: %d)", (*j).cristais, (*j).minimoCristais),
             MARGEM, y + 60, 20, BLACK);
    DrawText(TextFormat("Turnos: %d", (*j).turnos), MARGEM, y + 90, 20, BLACK);
    DrawText(TextFormat("Folego restante: %d", (*j).energia), MARGEM, y + 120, 20, BLACK);

    // Menu (as teclas R/Q sao tratadas no case TELA_FIM da main)
    DrawText("[R] Jogar novamente     [Q] Sair", MARGEM, y + 180, 20, RED);
}