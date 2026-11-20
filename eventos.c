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
            (*j).energia += BONUS_RECUPERACAO; // Recupera o bônus de energia
            (*j).mapa[(*j).linhaJ][(*j).colunaJ] = '.'; // Remove o item do mapa
            snprintf((*j).mensagem, TAM_MSG, "Fonte de energia encontrada! Energia atual: %d", (*j).energia);
            break;
        case 'A': // Armadilha
            if ((*j).energia - DANO_ARMADILHA < 0) { // Se a energia for menor que o dano, zera a energia, para evitar valores negativos
                (*j).energia = 0;
            } else {
                (*j).energia -= DANO_ARMADILHA;
            }
            (*j).mapa[(*j).linhaJ][(*j).colunaJ] = '.'; // Remove o item do mapa
            snprintf((*j).mensagem, TAM_MSG, "Armadilha! Energia perdida. Energia atual: %d", (*j).energia);
            break;
        case 'E': // Evento especial, faz a janela do jogo começar a se mover pela tela por 10 segundos
            iniciarEventoEspecial(j);
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

void iniciarEventoEspecial(Jogo *j) {
    SetWindowSize(larguraJanela(j), alturaJanela(j)); // Ajusta o tamanho da janela para o tamanho do jogo

    j->posicaoXJanela = GetWindowPosition().x; // Posição inicial da janela no eixo X
    j->posicaoYJanela = GetWindowPosition().y; // Posição inicial da janela no eixo Y
    j->velocidadeX = VELOCIDADE_JANELA; // Velocidade de movimento da janela no eixo X
    j->velocidadeY = VELOCIDADE_JANELA; // Velocidade de movimento da janela no eixo Y
    j->inicioEventoEspecial = GetTime(); // Marca o tempo de início do evento especial
    j->eventoEspecialAtivo = 1; // Ativa o evento especial
}

void atualizarEventoEspecial(Jogo *j) {
    if (GetTime() - j->inicioEventoEspecial >= DURACAO_EVENTO) {
        j->eventoEspecialAtivo = 0; // Para o evento especial após a duração definida
        return;
    }

    float monitorStartX = GetMonitorPosition(GetCurrentMonitor()).x;  // Posição inicial do monitor no eixo X
    float monitorStartY = GetMonitorPosition(GetCurrentMonitor()).y;  // Posição inicial do monitor no eixo Y

    float deltaTime = GetFrameTime(); // Tempo que o ultimo frame levou para ser processado
    j->posicaoXJanela += j->velocidadeX * deltaTime;
    j->posicaoYJanela += j->velocidadeY * deltaTime;

    // Verifica se a janela atingiu as bordas da tela e inverte a direção se necessário
    if (j->posicaoXJanela <= monitorStartX){
        j->posicaoXJanela = monitorStartX; // Garante que a posição não fique negativa
        j->velocidadeX *= -1; // Inverte a direção no eixo X
    } else if (j->posicaoXJanela + larguraJanela(j) >= GetMonitorWidth(GetCurrentMonitor()) + monitorStartX) {
        j->posicaoXJanela = GetMonitorWidth(GetCurrentMonitor()) - larguraJanela(j) + monitorStartX; // Garante que a posição não ultrapasse a largura da tela
        j->velocidadeX *= -1; // Inverte a direção no eixo X
    }
    if (j->posicaoYJanela <= monitorStartY) {
        j->posicaoYJanela = monitorStartY; // Garante que a posição não fique negativa
        j->velocidadeY *= -1; // Inverte a direção no eixo Y 
    } else if (j->posicaoYJanela + alturaJanela(j) >= GetMonitorHeight(GetCurrentMonitor()) + monitorStartY) {
        j->posicaoYJanela = GetMonitorHeight(GetCurrentMonitor()) - alturaJanela(j) + monitorStartY; // Garante que a posição não ultrapasse a altura da tela
        j->velocidadeY *= -1; // Inverte a direção no eixo Y
    }

    SetWindowPosition((int)j->posicaoXJanela, (int)j->posicaoYJanela); // Atualiza a posição da janela
}