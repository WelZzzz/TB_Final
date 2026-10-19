#include <stdio.h>
#include Jogo.h


int main(){
    
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
    

    return 0;
}