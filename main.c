//Expedicao na Ilha dos Algoritmos
/*Este é a main do jogo.*/

#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "jogo.h"

#define MAPA1 "mapa1.txt"

typedef enum { TELA_MENU, TELA_INSTRUCOES, TELA_JOGO, TELA_FIM, TELA_SAIR} Tela;

//Prototipos das funcoes
void showMainMenu(void);
void showFinalCredits(void);
void showInstructions(void);
void showGameOverScreen(void);
void processarTurno(Jogo *j, int tecla, int *resultado);
int novaPartida(Jogo *j);

/*void iniciarJogo(void);*/

int main(void){

    Tela telaAtual = TELA_MENU;
    int tecla;
    int resultado = EM_ANDAMENTO;
    int erroMapa = 0;
    Jogo jogo;


    if (carregarMapa(MAPA1, &jogo) == 0) {
        //Se o mapa não foi carregado corretamente, exibe uma mensagem de erro.
        printf("Erro ao carregar %s: %s\n", MAPA1, jogo.mensagem);

        return 1;
    }
    InitWindow(larguraJanela(&jogo), alturaJanela(&jogo), "Expedicao na Ilha dos Algoritmos");
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
                    if (novaPartida(&jogo)) {
                        resultado = EM_ANDAMENTO;
                        telaAtual = TELA_JOGO;
                    }else{
                        erroMapa = 1; // Sinaliza que houve um erro ao carregar o mapa
                        telaAtual = TELA_SAIR; // Sai do jogo se não conseguir carregar o mapa
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
                    resultado = DESISTENCIA;
                } 
                else if (tecla !=0){
                    processarTurno(&jogo, tecla, &resultado);
                }
                if (resultado != EM_ANDAMENTO) {
                    telaAtual = TELA_FIM; // Vai para a tela de fim de jogo
                }
                /*else if(telaAtual == TELA_JOGO){
                    DrawText(TextFormat("Linhas: %d | Colunas: %d", jogo.linhas, jogo.colunas), 40, 80, 20, BLACK);
                    DrawText(TextFormat("Energia: %d | Cristais minimos: %d", jogo.energia, jogo.minimoCristais), 40, 110, 20, BLACK);
                    DrawText(TextFormat("Jogador em: linha %d, coluna %d", jogo.linhaJ, jogo.colunaJ), 40, 140, 20, BLACK);
                    DrawText("Q = voltar ao menu", 40, 200, 20, DARKGRAY);
                }*/
                break;
            case TELA_FIM:
                if (tecla == KEY_R){
                    if (novaPartida(&jogo)) {
                        resultado = EM_ANDAMENTO;
                        telaAtual = TELA_JOGO;
                    }else{
                        erroMapa = 1; // Sinaliza que houve um erro ao carregar o mapa
                        telaAtual = TELA_SAIR; // Sai do jogo se não conseguir carregar o mapa
                    }    
                } else if (tecla == KEY_Q) { // Tecla 'Q' pressionada
                    telaAtual = TELA_SAIR; // Sai do jogo
                }
                break;
            default:
                break;
        }

        // Atualiza o evento especial se estiver ativo
        if (jogo.eventoEspecialAtivo)
            atualizarEventoEspecial(&jogo);

        // Desenha a tela principal do jogo
        BeginDrawing();
        ClearBackground(WHITE);
        if (telaAtual == TELA_MENU) {
            showMainMenu();
        } else if (telaAtual == TELA_INSTRUCOES) {
            showInstructions();
        } else if (telaAtual == TELA_JOGO) {
            desenharJogo(&jogo);
        } else if (telaAtual == TELA_FIM) {
            desenharFimDeJogo(&jogo, resultado);
        }
        EndDrawing();
    }

    CloseWindow(); // Fecha a janela e limpa recursos
    
    //Mensagem de erro caso o mapa não tenha sido carregado corretamente
    if (erroMapa) {
        printf("Erro ao carregar %s: %s\n", MAPA1, jogo.mensagem);

        return 1;
    }
    return 0;
}

void showMainMenu(void) {
    DrawText("Expedicao na Ilha dos Algoritmos", 40, 80, 32, DARKBLUE);
    DrawText("1. Iniciar Jogo",  40, 180, 24, BLACK);
    DrawText("2. Como Jogar",    40, 220, 24, BLACK);
    DrawText("3. Sair",   40, 260, 24, BLACK);
}

void showFinalCredits(void) {
    DrawText("OBRIGADO POR JOGAR!", 40, 80, 32, DARKGREEN);
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

void processarTurno(Jogo *j, int tecla, int *resultado){
    j->mensagem[0] = '\0'; // Limpa a mensagem
    // Verifica se o movimento é válido diminui energia e processa o turno
    if (movimentoValido(j, tecla)){
        moverJogador(&j->linhaJ, &j->colunaJ, tecla);
        j->energia--; // Reduz a energia do jogador a cada movimento
        j->turnos++; // Incrementa o número de turnos
        processarCasa(j);
        //Define vitoria ou derrota
        if(chegouNaSaida(j) == VITORIA){
            *resultado = VITORIA;
        }
        if ((*resultado) == EM_ANDAMENTO && j->energia<=0){
            *resultado = DERROTA;
        }
    }
    // Se o movimento não for válido, atualiza a mensagem de erro
    else{
        strcpy(j->mensagem, "Movimento invalido!"); 
    }
}

int novaPartida(Jogo *j){

    // Erro ao carregar o mapa
    if (!carregarMapa(MAPA1, j)){
        return 0; 
    }

    j->cristais = 0; // Reinicia a contagem de cristais
    j->turnos = 0; // Reinicia a contagem de turnos
    j->mensagem[0] = '\0'; // Limpa a mensagem
    return 1; // Mapa carregado com sucesso
}