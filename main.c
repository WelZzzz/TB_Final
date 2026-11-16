//Expedicao na Ilha dos Algoritmos
/*Este é a main do jogo.*/

#include "raylib.h"
#include "jogo.h"

#define MAPA1 "mapa1.txt"

typedef enum { TELA_MENU, TELA_INSTRUCOES, TELA_JOGO, TELA_SAIR} Tela;

//Prototipos das funcoes
void showMainMenu(void);

void showFinalCredits(void);

void showInstructions(void);

void showGameOverScreen(void);

/*void iniciarJogo(void);*/

int main(void){

    Tela telaAtual = TELA_MENU;
    int tecla;
    Jogo jogo;

    InitWindow(800, 600, "Expedicao na Ilha dos Algoritmos");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // Desabilita a tecla de saída padrão (ESC)

    // Loop principal do jogo
    while (!WindowShouldClose() && telaAtual != TELA_SAIR){

        //Lê a tecla pressionada
        tecla = GetKeyPressed();

        //Verifica a tela atual e realiza as ações correspondentes
        switch (telaAtual) {
            case TELA_MENU:
                
                if (tecla == KEY_ONE) { 
                    if (carregarMapa(MAPA1, &jogo)){
                        telaAtual = TELA_JOGO;
                    }
                } else if (tecla == KEY_TWO) {
                    telaAtual = TELA_INSTRUCOES; // Vai para a tela de instruções
                } else if (tecla == KEY_THREE || tecla == KEY_Q) { // Tecla '3' ou 'Q' pressionada
                    telaAtual = TELA_SAIR; // Sai do jogo
                }
                break;

            case TELA_INSTRUCOES:
                if (tecla == KEY_Q) { // Tecla 'Q' pressionada
                    telaAtual = TELA_MENU; // Volta para o menu
                }
                break;

            case TELA_JOGO:
                if (tecla == KEY_Q ){
                    telaAtual = TELA_MENU; // Volta para o menu
                }
                else if(telaAtual == TELA_JOGO){
                    DrawText(TextFormat("Linhas: %d | Colunas: %d", jogo.linhas, jogo.colunas), 40, 80, 20, BLACK);
                    DrawText(TextFormat("Energia: %d | Cristais minimos: %d", jogo.energia, jogo.minimoCristais), 40, 110, 20, BLACK);
                    DrawText(TextFormat("Jogador em: linha %d, coluna %d", jogo.linhaJ, jogo.colunaJ), 40, 140, 20, BLACK);
                    DrawText("Q = voltar ao menu", 40, 200, 20, DARKGRAY);
                }
                break;

            default:
                break;
        }

        // Desenha a tela principal do jogo
        BeginDrawing();
        ClearBackground(WHITE);
        if (telaAtual == TELA_MENU) {
            showMainMenu();
        } else if (telaAtual == TELA_INSTRUCOES) {
            showInstructions();
        }
        EndDrawing();
    }

    CloseWindow(); // Fecha a janela e limpa recursos
    return 0;
}

void showMainMenu(void) {
    DrawText("Expedicao na Ilha dos Algoritmos", 40, 80, 32, DARKBLUE);
    DrawText("1. Iniciar Jogo",  40, 180, 24, BLACK);
    DrawText("2. Como Jogar",    40, 220, 24, BLACK);
    DrawText("3. Sair",   40, 260, 24, BLACK);
}

void showFinalCredits(void) {
    DrawText("PARABENS VOCE GANHOU!", 40, 80, 32, DARKGREEN);
    DrawText("Voce completou todos os niveis!", 40, 150, 22, BLACK);
    DrawText("Este jogo foi desenvolvido para fins educacionais.", 40, 210, 20, BLACK);
    DrawText("Programado por WelZzz e FabioRaupp323.", 40, 240, 20, BLACK);
    DrawText("Pressione qualquer tecla para voltar ao menu...", 40, 320, 20, DARKGRAY);
}

void showInstructions(void) {
    DrawText("Como Jogar", 40, 60, 32, DARKBLUE);
    DrawText("Use W, A, S, D ou as setas para se mover pelo mapa.", 40, 130, 20, BLACK);
    DrawText("W = cima        Seta para cima",     40, 180, 20, BLACK);
    DrawText("A = esquerda    Seta para esquerda", 40, 210, 20, BLACK);
    DrawText("S = baixo       Seta para baixo",    40, 240, 20, BLACK);
    DrawText("D = direita     Seta para direita",  40, 270, 20, BLACK);
    DrawText("Colete os cristais necessarios para avancar.", 40, 320, 20, BLACK);
    DrawText("Pressione Q para voltar ao menu...", 40, 380, 20, DARKGRAY);
}

void showGameOverScreen(void) {
    DrawText("GAME OVER", 40, 80, 32, MAROON);
    DrawText("Voce gastou todas as energias.", 40, 150, 22, BLACK);
    DrawText("Pressione qualquer tecla para continuar...", 40, 220, 20, DARKGRAY);
}
 
/*void iniciarJogo(void){
    Jogo jogo;

    if (!carregarMapa(MAPA1, &jogo)) {
        printf("========================================\n");
        printf("Erro ao carregar o mapa do jogo.\n");
        printf("Certifique-se de que o arquivo '%s' existe e esta no diretorio correto.\n\n", MAPA1);
        printf("========================================\n");
        printf("Pressione 'Q' para voltar ao menu...\n");
        _getch();
        return;
    }
    
    printf("========================================\n");
    printf("     Mapa carregado com sucesso!\n");
    printf("========================================\n");
    _getch();
        
    printf("Linhas: %d | Colunas: %d\n", jogo.linhas, jogo.colunas);
    printf("Energia %d | Cristais Minimos: %d\n", jogo.energia, jogo.minimoCristais);
        jogo.energia = 35; //energia inicial do jogador
        jogo.minimoCristais = 3; //minimo de cristais para vencer
    
    _getch();

}*/

