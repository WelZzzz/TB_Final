#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include "jogo.h"

/*
- Leitura do mapa a partir de um arquivo
- Retorna 1 se o mapa foi carregado com sucesso, 0 caso contrário
*/

int carregarMapa(const char *nomeArquivo, Jogo *j){
    FILE *arquivo;
    char linha[MAX_COLUNAS + 4];
    int i, c;

    //abrir o arquivo para leitura
    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        return 0; // Erro ao abrir o arquivo
    }

    // Ler as dimensões do mapa
    if (fscanf(arquivo, "%d %d", &j->linhas, &j->colunas) != 2) {
        fclose(arquivo);
        return 0; // Erro ao ler as dimensões
    }

    //Ler a energia inicial e o mínimo de cristais
    if (fscanf(arquivo, "%d %d", &j->energia, &j->minimoCristais) != 2) {
        fclose(arquivo);
        return 0; // Erro ao ler energia e mínimo de cristais
    }
    fgets(linha, sizeof(linha), arquivo); // Limpar o buffer após a leitura

    //Linhas do mapa
    for (i = 0; i < j->linhas; i++){
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            return 0; // Erro ao ler uma linha do mapa
        }
        
        for (c = 0; c <j->colunas; c++){
            j->mapa[i][c] = linha[c];
            if (linha[c] == 'P'){
                j->linhaJ = i;
                j->colunaJ = c;
                j->mapa[i][c] = ' '; // Substituir a posição inicial do jogador por espaço
            }
        }
    }
    fclose(arquivo);
    return j->linhaJ != -1; // Retorna 1 se a posição do jogador foi encontrada, 0 caso contrário
}