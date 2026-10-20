#include <stdio.h>
#include <Jogo.h>


void limparTela(void):

int esperarTecla(Jogo *j, int telaFinal, int resultado);

void showMainMenu(void);

int main(){
    
    

    while (1){
        limparTela();
        showMainMenu();
        scanf("%d", &option);




    }


    return 0;
}

int esperarTecla(Jogo *j, int telaFinal, int resultado) {
    
    int tecla;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        if (telaFinal) {
            desenharFimDeJogo(j, resultado);
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
    printf("Escolha uma opcao: ");
}





