#include "plutonio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define FILAS_STR 6
#define COLUMNA_STR 200

#define GANASTE 1
#define PERDISTE -1
#define SIGUES_JUGANDO 0 

#define intervalo_tiempo 5

#define RESET_COLOR "\033[0m"
#define COLOR_MAGENTA "\033[95m"
#define COLOR_TITULO "\033[1;36;4m"
#define COLOR_AZUL "\033[94m"

// PRE: La variable accion anteriormente declarada, pasada por referencia.
// POST: Devuelve la variable accion validada.

void validacion_accion(char *accion){
    printf("\n\tIngrese una accion: \n\t1. W si es arriba\n\t2. S si es abajo\n\t3. D si es derecha\n\t4. A izquierda\n\t5. L linterna\n");
    scanf(" %c", accion);
    while (*accion!='W' && *accion!='D' && *accion!='A' && *accion!='L' && *accion!='S'){
        printf("\n\tIngrese de nuevo una accion valida: \n\t1. W si es arriba\n\t2. S si es abajo\n\t3. D si es derecha\n\t4.A izquierda\n\t5. L linterna\n");
        scanf(" %c", accion);
    }
}

// PRE: El vector de caracteres lleno, pasados por valor.
// POST: Nos devuelve los caracteres editados con colores y tabulaciones en los extremos.

void introduccion_cartel(char texto[FILAS_STR][COLUMNA_STR]){
    for (int i=0; i<FILAS_STR; i++){
        if (i==0){
            printf("\t\t%s###################################\n", COLOR_MAGENTA);
            printf("\t\t###\t%s %s %s %s\t###\n", COLOR_TITULO, texto[i], RESET_COLOR, COLOR_MAGENTA);
            printf("\t\t###################################%s\n", RESET_COLOR);
        }
        else{
            printf("\t");
            printf("%s### %s %s %s###\n",COLOR_MAGENTA, COLOR_AZUL, texto[i], COLOR_MAGENTA);
            printf("\t");
            printf(RESET_COLOR);
        }
        printf("\n");
        printf(RESET_COLOR);
    }
    sleep(intervalo_tiempo);
}
    
// PRE: El struct juego ya inicializado en ´inicializar_juego´.
// POST: Si la funcion ´estado_juego´ retorna -1, perdiste el juego.
//       Si la funcion ´estado_juego´ retorna 1, ganaste el juego

void imprimir_final_juego(juego_t homero){
    if(estado_juego(homero)==GANASTE){
        printf("\n\n\tGANASTE!!!\n\n");
    }
    else if(estado_juego(homero)==PERDISTE){
        printf("\n\n\tHAS PERDIDO!!!\n\n");
    }
}
        
//PRE: El struct juego ya inicializado en ´inicializar_juego´ pasados por referencia, y la accion validada del usuario.
//POST: Homero se mueve dependiendo de la accion del usuario, y muestra el juego en la terminal.

void realizar_mostrar_juego(juego_t *homero,char accion){
    realizar_jugada(homero, accion);
    mostrar_juego(*homero);
}
    
// PRE: El struct juego ya inicializado en ´inicializar_juego´ pasados por referencia, y la accion validada del usuario.
// POST:  El juego se ejecuta hasta que retorne 1 o -1.
//       Muestra el desarrollo del juego por la terminal, hasta que se termine.

void flujo_del_juego(juego_t *homero, char *accion){
    while (estado_juego(*homero)==SIGUES_JUGANDO){  
        validacion_accion(accion);
        system("clear");
                
        realizar_mostrar_juego(homero, *accion);

        estado_juego(*homero);

        imprimir_final_juego(*homero);
    }
}
    

int main(){
    srand ((unsigned)time(NULL));
    
    char accion;
    char texto[FILAS_STR][COLUMNA_STR]={
        "DESASTRE NUCLEAR",
        "Las RATAS (R) te quitaran 1 linterna",
        "Los BARRILES (A) te quitaran 15 de energia",
        "Los PROPULSORES (E) o (C) te haran moverte 3 veces consumiendo 1 de energia.",
        "Las DONAS (D) te sumaran 10 de energia",
        "Las BARRAS (B) es lo que tienes como objetivo agarrar, se te restara uno a la cantidad de barras que te quedan."
    };
    
    juego_t H;

    system("clear");

    introduccion_cartel(texto);

    system("clear");

    inicializar_juego(&H);

    mostrar_juego(H);

    validacion_accion(&accion);

    system("clear");

    realizar_mostrar_juego(&H, accion);
    
    flujo_del_juego(&H, &accion);
    
    return 0;
}