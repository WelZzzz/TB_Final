#include <stdio.h>
#include "Jogo.h"

//#define MAPA1 "mapa1.txt"
#define KEY_Q 'q'

int esperarTecla(Jogo *j, int telaFinal, int resultado);

void showMainMenu(void);

void showFinalCredits(void);

void showInstructions(void);

void showGameOverScreen(void);

void limparTela(void);

int main(){
    
    char option;

    while (1){
        limparTela();
        showMainMenu();
        
        option = _getche();
        
        if (option == '1') {
            // Iniciar o jogo
            printf("iniciarJogo()");
            
        } else if (option == '2') {
            // Mostrar instruções
            showInstructions();
        } else if (option == '3') {
            // Sair do jogo
            break;
        } else {
            printf("Opcao invalida. Pressione qualquer tecla para tentar novamente...\n");
            _getch();
        }
    }


    return 0;
}

/*int esperarTecla(Jogo *j, int telaFinal, int resultado) {
    
    int tecla;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        if (telaFinal) {
            desenharFimDeJogo(j, resultado);
        } else {
            desenharJogo(j);
        }
        EndDrawing();
        
        tecla = GetKeyPressed(); // letras chegam sempre em maiuscula
        if (tecla != 0) {
            return tecla;
        }
    }
    return KEY_Q;
}*/

void limparTela(void) {
    system("cls");
}   

void showMainMenu(void) {
    printf("========================================\n");
    printf("    Expedicao na Ilha dos Algoritmos\n");
    printf("========================================\n\n");
    printf("1. Iniciar Jogo\n");
    printf("2. Como Jogar\n");
    printf("3. Sair\n\n");
    printf("========================================\n");
}


void showFinalCredits(void) {
    limparTela();
    printf("========================================\n");
    printf("        PARABENS VOCE GANHOU!\n");
    printf("========================================\n\n");
    printf("Voce completou todos os niveis!\n\n");
    printf("Credits:\n");
    printf("Este jogo foi desenvolvido para fins educacionais.\n");
    printf("Programado por WelZzz.\n\n");
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    _getch();
}

void showInstructions(void) {
    limparTela();
    printf("========================================\n");
    printf("               Como Jogar\n");
    printf("========================================\n\n");
    printf("Use W, A, S, D ou as setas ↑, ←, ↓, → para se mover pelo mapa.\n");
    printf("W = cima        Seta = ↑\n");
    printf("A = esquerda    Seta = ←\n");
    printf("S = baixo       Seta = ↓\n");
    printf("D = direita     Seta = →\n\n");
    printf("Colete os cristais necessarios para avancar.\n\n");
    printf("Pressione qualquer tecla para retornar ao menu...\n");
    _getch();
}

void showGameOverScreen(void) {
    limparTela();
    printf("========================================\n");
    printf("               GAME OVER\n");
    printf("========================================\n\n");
    printf("Voce perdeu todas as vidas.\n");
    printf("Reiniciando no nivel 1...\n");
    printf("Pressione qualquer tecla para continuar...\n");
    _getch();
}
