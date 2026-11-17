#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include "jogo.h"

/* Le o arquivo do mapa, valida e preenche a struct Jogo.
 Retorna 1 se deu certo; 0 se houve erro (o motivo fica em j->mensagem).*/

int carregarMapa(const char *nomeArquivo, Jogo *j){
    FILE *arquivo;
    char linha[128];   // Maior que MAX_COLUNAS: linha comprida demais eh detectada
    int i, c, tamanho;
    int qtdP = 0, qtdS = 0, totalCristais = 0;
    char simbolo;

    // Zera a struct inteira: cristais, turnos e mensagem
    memset(j, 0, sizeof(*j));
    j->linhaJ = -1;
    j->colunaJ = -1;

    // Abrir o arquivo
    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        snprintf(j->mensagem, TAM_MSG, "arquivo nao encontrado");
        return 0;
    }

    // Linha 1: dimensoes (fgets + sscanf evita o problema do '\n' no buffer)
    if (fgets(linha, sizeof(linha), arquivo) == NULL ||
        sscanf(linha, "%d %d", &j->linhas, &j->colunas) != 2) {
        snprintf(j->mensagem, TAM_MSG, "linha 1 deve conter: linhas colunas");
        fclose(arquivo);
        return 0;
    }
    if (j->linhas < MIN_LINHAS || j->linhas > MAX_LINHAS ||
        j->colunas < MIN_COLUNAS || j->colunas > MAX_COLUNAS) {
        snprintf(j->mensagem, TAM_MSG, "dimensoes %dx%d fora do limite (%dx%d a %dx%d)",
                 j->linhas, j->colunas, MIN_LINHAS, MIN_COLUNAS, MAX_LINHAS, MAX_COLUNAS);
        fclose(arquivo);
        return 0;
    }

    // Linha 2: energia inicial e minimo de cristais
    if (fgets(linha, sizeof(linha), arquivo) == NULL ||
        sscanf(linha, "%d %d", &j->energia, &j->minimoCristais) != 2) {
        snprintf(j->mensagem, TAM_MSG, "linha 2 deve conter: energia minimoCristais");
        fclose(arquivo);
        return 0;
    }
    if (j->energia <= 0 || j->minimoCristais < 0) {
        snprintf(j->mensagem, TAM_MSG, "energia deve ser > 0 e minimo de cristais >= 0");
        fclose(arquivo);
        return 0;
    }

    // Linhas do mapa
    for (i = 0; i < j->linhas; i++) {
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            snprintf(j->mensagem, TAM_MSG, "faltam linhas no mapa: lidas %d de %d", i, j->linhas);
            fclose(arquivo);
            return 0;
        }

        linha[strcspn(linha, "\r\n")] = '\0';   // Remove \n e \r (arquivos do Windows)
        tamanho = (int)strlen(linha);
        if (tamanho != j->colunas) {
            snprintf(j->mensagem, TAM_MSG, "linha %d do mapa tem %d colunas, esperado %d",
                     i + 1, tamanho, j->colunas);
            fclose(arquivo);
            return 0;
        }

        for (c = 0; c < j->colunas; c++) {
            simbolo = linha[c];

            // Simbolo fora da tabela
            if (strchr(".#CRAESP", simbolo) == NULL) {
                snprintf(j->mensagem, TAM_MSG, "simbolo invalido '%c' na linha %d do mapa",
                         simbolo, i + 1);
                fclose(arquivo);
                return 0;
            }

            if (simbolo == 'P') {
                qtdP++;
                j->linhaJ = i; // Guarda a posicao inicial
                j->colunaJ = c;
                simbolo = '.'; // A casa do P vira caminho livre
            } else if (simbolo == 'S') {
                qtdS++;
            } else if (simbolo == 'C') {
                totalCristais++;
            }
            j->mapa[i][c] = simbolo;
        }
    }
    fclose(arquivo);

    // Validacoes que dependem do mapa inteiro
    if (qtdP != 1) {
        snprintf(j->mensagem, TAM_MSG, "o mapa precisa de exatamente um P (encontrados: %d)", qtdP);
        return 0;
    }
    if (qtdS != 1) {
        snprintf(j->mensagem, TAM_MSG, "o mapa precisa de exatamente um S (encontrados: %d)", qtdS);
        return 0;
    }
    if (j->minimoCristais > totalCristais) {
        snprintf(j->mensagem, TAM_MSG, "minimo de cristais (%d) maior que o total do mapa (%d)",
                 j->minimoCristais, totalCristais);
        return 0;
    }
    return 1;
}

int movimentoValido(Jogo *j, int comando){
    int novaLinha = (*j).linhaJ;
    int novaColuna = (*j).colunaJ;

    switch (comando) {
        case KEY_W:
        case KEY_UP:
            novaLinha--;
            break;
        case KEY_S:
        case KEY_DOWN:
            novaLinha++;
            break;
        case KEY_A:
        case KEY_LEFT:
            novaColuna--;
            break;
        case KEY_D:
        case KEY_RIGHT:
            novaColuna++;
            break;
        default:
            return 0; // Comando invalido
    }

    // Verifica se a nova posicao esta dentro dos limites do mapa
    if (novaLinha >= 0 && novaLinha < (*j).linhas && novaColuna >= 0 && novaColuna < (*j).colunas) {
        // Verifica se a nova posicao nao eh uma parede
        if ((*j).mapa[novaLinha][novaColuna] != '#') {
            return 1; // Movimento valido
        }
    }

    return 0; // Movimento invalido
}

// Largura da janela calculada a partir do mapa.
int larguraJanela(Jogo *j){
    int largura = MARGEM + j->colunas * TAM_CASA + MARGEM;
    if (largura < LARGURA_MINIMA) {   // mapa estreito: garante espaco para os textos
        largura = LARGURA_MINIMA;
    }
    return largura;
}

// Altura da janela: mapa + painel de estado.
int alturaJanela(Jogo *j){
    int altura = MARGEM + j->linhas * TAM_CASA + ALTURA_PAINEL + MARGEM;
    if (altura < ALTURA_MINIMA) {
        altura = ALTURA_MINIMA;
    }
    return altura;
}

// Escreve um texto centralizado dentro de uma casa.
static void escreverNaCasa(const char *texto, int x, int y, Color cor){
    int largura = MeasureText(texto, 20);
    DrawText(texto, x + (TAM_CASA - largura) / 2, y + (TAM_CASA - 20) / 2, 20, cor);
}

// Converte um simbolo da matriz em forma + cor.
static void desenharCasa(char simbolo, int x, int y){
    int lado = TAM_CASA - 2;   // deixa uma linha entre as casas

    switch (simbolo) {
        case '#':
            DrawRectangle(x, y, lado, lado, DARKGRAY);
            break;
        case 'C':
            DrawRectangle(x, y, lado, lado, LIGHTGRAY);   // piso
            DrawCircle(x + TAM_CASA / 2, y + TAM_CASA / 2, TAM_CASA / 3, GOLD);
            break;
        case 'R':
            DrawRectangle(x, y, lado, lado, DARKGREEN);
            escreverNaCasa("+", x, y, WHITE);
            break;
        case 'A':
            DrawRectangle(x, y, lado, lado, RED);
            escreverNaCasa("!", x, y, WHITE);
            break;
        case 'E':
            DrawRectangle(x, y, lado, lado, PURPLE);
            escreverNaCasa("?", x, y, WHITE);
            break;
        case 'S':
            DrawRectangle(x, y, lado, lado, BLUE);
            escreverNaCasa("S", x, y, WHITE);
            break;
        default:   // '.' caminho livre
            DrawRectangle(x, y, lado, lado, LIGHTGRAY);
            break;
    }
}

/* Desenha o mapa, o jogador por cima e o painel de estado.
 Nao chama BeginDrawing/EndDrawing: quem faz isso e a main.*/

void desenharJogo(Jogo *j){
    int i, c, x, y, yPainel;

    // O mapa, casa por casa
    for (i = 0; i < j->linhas; i++) {
        for (c = 0; c < j->colunas; c++) {
            x = MARGEM + c * TAM_CASA;
            y = MARGEM + i * TAM_CASA;
            desenharCasa(j->mapa[i][c], x, y);
        }
    }

    // O jogador POR CIMA, sem alterar a matriz
    x = MARGEM + j->colunaJ * TAM_CASA;
    y = MARGEM + j->linhaJ * TAM_CASA;
    DrawRectangle(x, y, TAM_CASA - 2, TAM_CASA - 2, MAGENTA);
    escreverNaCasa("P", x, y, WHITE);

    // Painel de estado (R7) abaixo do mapa
    yPainel = MARGEM + j->linhas * TAM_CASA + 10;
    DrawText(TextFormat("Folego: %d   Cristais: %d/%d   Turnos: %d",
                        j->energia, j->cristais, j->minimoCristais, j->turnos),
             MARGEM, yPainel, 20, BLACK);
    DrawText(j->mensagem, MARGEM, yPainel + 26, 20, MAROON);
    DrawText("Setas ou WASD: mover   Q: desistir", MARGEM, yPainel + 56, 16, DARKGRAY);
    DrawText("Legenda: P jogador  # parede  C cristal  S saida", MARGEM, yPainel + 80, 16, DARKGRAY);
    DrawText("R +energia  A armadilha  E evento", MARGEM, yPainel + 100, 16, DARKGRAY);
}
