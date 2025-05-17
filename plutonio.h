#ifndef __PLUTONIO_H__
#define __PLUTONIO_H__

#include <stdbool.h>

#define MAX_HERRAMIENTAS 30
#define MAX_OBSTACULOS 30
#define MAX_BARRAS 10
#define MAX_PROPULSORES 10

typedef struct coordenada {
    int fil;
    int col;
} coordenada_t;

typedef struct objeto {
    coordenada_t posicion;
    char tipo;
    bool visible;
} objeto_t;

typedef struct personaje {
    coordenada_t posicion;
    int cantidad_linternas;
    int cantidad_barras;
    int energia;
    bool linterna_activada;
    int mov_linterna;
} personaje_t;

typedef struct juego {
    personaje_t homero;
    objeto_t herramientas[MAX_HERRAMIENTAS];
    int cantidad_herramientas;
    objeto_t obstaculos[MAX_OBSTACULOS];
    int cantidad_obstaculos;
    objeto_t barras[MAX_BARRAS];
    int cantidad_barras;
    objeto_t propulsores[MAX_PROPULSORES];
    int cantidad_propulsores;
} juego_t;

/*
 * Pre condiciones: -
 * Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero, las herramientas y los obstáculos.
 */
void inicializar_juego(juego_t *juego);

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción
 * debe ser válida.
 * Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
 */
void realizar_jugada(juego_t *juego, char accion);

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego `.
 * Post condiciones: Imprime el juego por pantalla.
 */
void mostrar_juego(juego_t juego);

/*
 * Pre condiciones: El juego deberá estar inicializado previamente con `inicializar_juego `
 * Post condiciones: Devuelve:
 * --> 1 si es ganado
 * --> -1 si es perdido
 * --> 0 si se sigue jugando
 * El juego se dará por ganado cuando Homero junta todas las barras de plutonio.
 * Se dará por perdido si se le termina la energía a Homero y no juntó todas las barras de plutonio.
 */
int estado_juego(juego_t juego);


#endif /* __PLUTONIO_H__ */
