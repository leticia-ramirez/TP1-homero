#include "plutonio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_TERRENO 20
#define MAX_OBJETOS 55

#define ENERGIA_DONA 10
#define ENERGIA_BARRIL 15

#define GANASTE 1
#define PERDISTE -1
#define SIGUES_JUGANDO 0 

#define DISTANCIA_MANHATTAN 2
#define MOV_PROPULSORES 3

#define DONAS 5
#define INTERRUPTOR 4
#define PROPULSOR_E 3
#define PROPULSOR_C 3
#define RATAS 5
#define BARRIL 15

#define RANGO 20

#define RESET_COLOR "\033[0m"
#define COLOR_VERDE "\033[1;32m"
#define COLOR_ROSA "\033[1;35m"
#define COLOR_TITULO "\033[1;36;4m"
#define COLOR_MAGENTA "\033[95m"


// PRE: Tener dos coordenadas que se necesiten comparar.
// POST: Retorna true si la coordenada generada es igual a la de algun objeto dentro del juego. En caso contrario, devuelve false.
//       Se usa para que no haya superposicion con los objetos dentro del juego.

bool coordenadas_iguales(coordenada_t coordenada_objeto_uno, coordenada_t coordenada_objeto_dos){
    bool coordenadas_repetidas=false;
    if(coordenada_objeto_uno.fil==coordenada_objeto_dos.fil && coordenada_objeto_uno.col==coordenada_objeto_dos.col){
        coordenadas_repetidas=true;
    }
    return coordenadas_repetidas;
}

// PRE: Tener una nueva coordenada generada aleatoriamente, saber las coordenadas generadas hasta el momento y la cantidad total.
// POST: Nos devuelve un tipo de dato bool. True si la nueva coordenada ya fue generada, False en caso contrario.

bool evitar_repeticion_numeros_inicial(coordenada_t nueva_coordenada, coordenada_t coordenadas_usadas[MAX_OBJETOS], int tope_usadas){
    bool coordenadas_repetidas=false;
    for(int i=0; i<tope_usadas;i++){
        if(coordenadas_iguales(coordenadas_usadas[i], nueva_coordenada)){
            coordenadas_repetidas= true;
        }
    }
    return coordenadas_repetidas;
}

// PRE: Una coordenada inicializada anteriormente con su tope (pasada por referencia) y la nueva coordenada que se quiere guardar en las coordenadas usadas.
// POST: Agrega la nueva coordenada al vector coordenadas.

void guardar_coordenadas(coordenada_t coordenadas_usadas[MAX_OBJETOS], int *tope_usadas, coordenada_t nueva_coordenada){
    coordenadas_usadas[*tope_usadas]=nueva_coordenada;
    (*tope_usadas)++;
}

// PRE: Una coordenada_t inicializada anteriormente con su tope pasada por referencia. Se utiliza para inicializar las posiciones de los objetos. 
// POST: Nos devuelve una coordenada nueva y distintas a las generadas anteriormente.

coordenada_t coordenadas_aleatorias_inicial(coordenada_t coordenadas_usadas[MAX_OBJETOS], int *tope_usadas){
    coordenada_t nueva_coordenada;

    do{
        nueva_coordenada.fil= rand()% RANGO;
        nueva_coordenada.col= rand()% RANGO;
    } while(evitar_repeticion_numeros_inicial(nueva_coordenada, coordenadas_usadas, *tope_usadas));
    
    guardar_coordenadas(coordenadas_usadas, tope_usadas, nueva_coordenada);

    return nueva_coordenada;
}

// PRE: Tener una nueva coordenada generada aleatoriamente, y el struct juego anteriormente inicializado.
// POST: Nos devuelve un tipo de dato bool. True si la nueva coordenada ya fue generada, False en caso contrario.

bool evitar_repeticion_numeros_aleatorios(coordenada_t nueva_coordenada, juego_t juego){
    bool coordenada_repetida = false;

    if (coordenadas_iguales(juego.homero.posicion, nueva_coordenada)) {
        coordenada_repetida = true;
    }

    for (int i = 0; i < juego.cantidad_herramientas; i++) {
        if (coordenadas_iguales(juego.herramientas[i].posicion, nueva_coordenada)) {
            coordenada_repetida = true;
        }
    }

    for (int i = 0; i < juego.cantidad_obstaculos; i++) {
        if (coordenadas_iguales(juego.obstaculos[i].posicion, nueva_coordenada)){
            coordenada_repetida=true;
        }
        
    }

    for (int i = 0; i < juego.cantidad_propulsores; i++) {
        if (coordenadas_iguales(juego.propulsores[i].posicion, nueva_coordenada)) {
            coordenada_repetida = true;
        }
    }

    for (int i = 0; i < juego.cantidad_barras; i++) {
        if (coordenadas_iguales(juego.barras[i].posicion, nueva_coordenada)) {
            coordenada_repetida = true;
        }
    }

    return coordenada_repetida;
}


// PRE: el struct juego anteriormente inicializado.
// POST: Nos devuelve una coordenada nueva y distintas a las generadas anteriormente.

coordenada_t coordenadas_aleatorias_no_repetidas(juego_t juego){
    coordenada_t nueva_coordenada;

    do{
        nueva_coordenada.fil= rand()% RANGO;
        nueva_coordenada.col= rand()% RANGO;
    } while(evitar_repeticion_numeros_aleatorios(nueva_coordenada, juego));
    
    return nueva_coordenada;
}
    
// PRE: Obtener los valores por referencia del juego, y su indice 
// POST: Nos devuelve un vector corrido hacia adelante, empezando desde su indice y a su tope se le resta uno. Esto quiere decir que, eliminamos el valor que se tenia en el indice.
//          El objetivo de esto es eliminar el elemento de la matriz cuando el usuario tenga las mismas coordenadas que esta.  

void eliminar_vectores_unitarios_barras(juego_t *juego, int indice){
    if ((indice<0) || (indice>=(juego->cantidad_barras))){
        return;
    }

    for (int i=indice; i<(juego->cantidad_barras-1); i++){
        juego->barras[i]=juego->barras[i+1];
    }

    (juego->cantidad_barras)--;
}

// PRE: Obtener los valores por referencia del juego, y su indice 
// POST: Nos devuelve un vector corrido hacia adelante, empezando desde su indice y a su tope se le resta uno. Esto quiere decir que, eliminamos el valor que se tenia en el indice.
//          El objetivo de esto es eliminar el elemento de la matriz cuando el usuario tenga las mismas coordenadas que esta.  

void eliminar_vectores_unitarios_herramientas(juego_t *juego, int indice){
    if ((indice<0) || (indice>=(juego->cantidad_herramientas))){
        return;
    }
    for (int i=indice; i<(juego->cantidad_herramientas-1); i++){
        juego->herramientas[i]=juego->herramientas[i+1];
    }

    (juego->cantidad_herramientas)--;
}
     
// PRE: Se crea una variable vector objeto_t vacio con la cantidad por referencia de la misma que estara inicializada en 0 y los valores del juego.
// POST: Se anaden todas las herramientas, obstaculos, barras y propulsores al vector objeto_t.

void objetos_en_juego(objeto_t objetos[MAX_OBJETOS], int *tope_objetos, juego_t juego){
    for (int i=0; i<juego.cantidad_herramientas; i++){
        objetos[(*tope_objetos)]=juego.herramientas[i];
        (*tope_objetos)++;
    }
        
    for (int i=0; i< juego.cantidad_barras; i++){
        objetos[(*tope_objetos)]=juego.barras[i];
        (*tope_objetos)++;
    }
                        
    for (int i=0; i< juego.cantidad_propulsores; i++){
        objetos[(*tope_objetos)]=juego.propulsores[i];
        (*tope_objetos)++;
    }
    
    for (int i=0; i<juego.cantidad_obstaculos;i++){
        objetos[(*tope_objetos)]=juego.obstaculos[i];
        (*tope_objetos)++;
    }
}

// PRE: Terreno con su tamano expecifico y que este admita solo los tipos de datos char.
// POST: Inicializa el terreno con '.', para evitar que la matriz se inicialice con basura dentro.

void inicializar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO]){
    for (int i=0;i<MAX_TERRENO;i++){
        for (int j=0;j<MAX_TERRENO;j++){
            terreno[i][j]= '.';     
        }
    }
}

// PRE: Terreno con su tamano expecifico y que este admita solo los tipos de datos char.
// POST: Imprime el terreno con los datos del tipo char.

void imprimir_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], juego_t juego){
    coordenada_t posicion_homero=juego.homero.posicion;
    
    for (int i=0; i<MAX_TERRENO;i++){
        printf("\t");
        for (int j=0; j<MAX_TERRENO; j++){
            terreno[posicion_homero.fil][posicion_homero.col]='H';
            printf ("[%c]", terreno[i][j]);        
        }
        printf("\n");
    }
}

// PRE: Valor por referencia del struct juego anteriormente inicializados.
// POST: Suma 10 de energia, cuando homero y la dona tengan la misma coordenada. Ademas elimina el objeto cuando se le suma esa cantidad de energia.
    
void reaccion_donas_homero(juego_t *juego){
    coordenada_t posicion_homero= juego->homero.posicion;
    
    for (int i=0; i< (juego->cantidad_herramientas); i++){
        if(juego->herramientas[i].tipo=='D'){
            if (coordenadas_iguales(juego->herramientas[i].posicion, posicion_homero)){
                juego->homero.energia += ENERGIA_DONA;
                eliminar_vectores_unitarios_herramientas(juego, i);
            }
        }
    }
}

// PRE: Valor por referencia del struct juego anteriormente inicializados.
// POST: Quita una linterna, cuando homero y la rata tengan la misma coordenada.

void reaccion_ratas_homero(juego_t *juego){
    coordenada_t posicion_homero= juego->homero.posicion;

    for (int i=0; i<(juego->cantidad_obstaculos); i++){
        if(juego->obstaculos[i].tipo=='R'){
            if(coordenadas_iguales(juego->obstaculos[i].posicion, posicion_homero)){
                (juego->homero.cantidad_linternas)--;
            }
        }
    }
}
 
// PRE: Valor por referencia del struct juego anteriormente inicializados.
// POST: Le resta 15 de energia, cuando homero y el barril tengan la misma coordenada.

void reaccion_barril_homero(juego_t *juego){
    coordenada_t posicion_homero= juego->homero.posicion;

    for (int i=0; i< juego->cantidad_obstaculos; i++){ 
        if(juego->obstaculos[i].tipo=='A'){
            if(coordenadas_iguales(juego->obstaculos[i].posicion, posicion_homero)){
                (juego->homero.energia) -= ENERGIA_BARRIL;     
            }  
        }
    }
}

// PRE: Valor por referencia del struct juego anteriormente inicializados.
// POST: Cuando homero y la barra tengan la misma coordenada, se elimina del vector y le suma 1 por cada barra que agarre.

void reaccion_barras_homero(juego_t *juego){
    coordenada_t posicion_homero= juego->homero.posicion;

    for (int i=0;i<juego->cantidad_barras;i++){
        if(coordenadas_iguales(juego->barras[i].posicion, posicion_homero)){
            (juego->homero.cantidad_barras)++;
            eliminar_vectores_unitarios_barras(juego, i);
        }
    }
}

// PRE: Obtener los valores por referencia del juego, y el estado booleano si homero se encuentra en el interruptor. 
// POST: Los objetos cambian los valores de tipo bool a false, excepto los interruptores, si y solo si no se encuentra activada la linterna ni homero se encuentra en un interruptor.

void objetos_no_visibles(juego_t *juego, bool esta_sobre_interruptor){
    if(!(juego->homero.linterna_activada)&& !(esta_sobre_interruptor)){
        for (int i=0; i<(juego->cantidad_herramientas); i++){
            if (juego->herramientas[i].tipo=='I'){
                juego->herramientas[i].visible=true;
            }
            else{
                juego->herramientas[i].visible=false;
            }
        }
    
        for (int i=0; i<juego->cantidad_obstaculos; i++){
            juego->obstaculos[i].visible=false;
        }
        for (int i=0; i<juego->cantidad_propulsores; i++){
            juego->propulsores[i].visible=false;
        }
        for (int i=0; i<juego->cantidad_barras; i++){
            juego->barras[i].visible=false;
        }     
    }
}

// PRE: Los valores de struct juego anteriormente inicializados pasados por referencia.
// POST: Cambian las coordenadas de las ratas (R).

void coordenadas_ratas_interruptor_linterna(juego_t *juego){
    for (int i = 0; i < juego->cantidad_obstaculos; i++) {
        if (juego->obstaculos[i].tipo == 'R') {
            juego->obstaculos[i].posicion = coordenadas_aleatorias_no_repetidas(*juego);
        }
    }
}
            
// PRE: Datos del struct juego anteriormente inicializados, y el booleano que indica si homero se encuentra sobre un interruptor pasado por referencia.
// POST: Las coordenadas de las ratas cambian de posicion cuando homero se coloca sobre el interruptor y la variable bool cambia a true. En caso que no se encuentre sobre el interruptor, no se moveran las ratas y la variable bool no cambiara.

void reaccion_interruptor_ratas_homero (juego_t *juego, bool *esta_sobre_interruptor){
    coordenada_t posicion_homero=juego->homero.posicion;

    for (int j = 0; j < juego->cantidad_herramientas; j++) {
        if ((juego->herramientas[j].tipo == 'I') && coordenadas_iguales(juego->herramientas[j].posicion, posicion_homero)) {
            coordenadas_ratas_interruptor_linterna(juego);
            (*esta_sobre_interruptor) = true;
        }
    }
}

// PRE: El struct juego anteiormente inicializado pasado por referencia, y el booleano que indica si homero esta sobre el interruptor.
// POST: Se modifica la visibilidad de los objetos a true si homero se encuentra sobre el interruptor. 

void interruptor_homero(juego_t *juego, bool esta_sobre_interruptor){
    if (esta_sobre_interruptor) {
        for (int i=0; i<juego->cantidad_herramientas; i++){
            juego->herramientas[i].visible=true;
        }
                
        for (int i=0; i<juego->cantidad_obstaculos;i++){
            juego->obstaculos[i].visible=true;
        }
                    
        for (int i=0; i< juego->cantidad_barras; i++){
            juego->barras[i].visible=true;
        }
                                    
        for (int i=0; i< juego->cantidad_propulsores; i++){
            juego->propulsores[i].visible=true;
        }
    } 
}
    

// PRE: Valor por referencia del struct juego anteriormente inicializados.
// POST: Esta funcion permite que cuando Homero se encuentre con donas o barril.. reaccione con estos.

void reaccion_escalera_objetos(juego_t *juego){
    reaccion_donas_homero(juego);
    reaccion_barril_homero(juego);
}

// PRE: Valor por referencia del struct juego anteriormente inicializados.
// POST: Esta funcion permite que cuando Homero se encuentre con barras o ratas.. reaccione con estos.

void reaccion_charco_objetos(juego_t *juego){
    reaccion_barras_homero(juego);
    reaccion_ratas_homero(juego);
}
 
// PRE: Obtencion de los valores por referencia del juego anteriormente inicializados y las coordenadas nuevas de homero.
// POST: Como objetivo de la funcion es que homero este dentro de los limites del terreno siempre.

void validacion_col_fil(juego_t *juego, int columna_homero, int fila_homero){
    if ((columna_homero>=0 && columna_homero<MAX_TERRENO)&&(fila_homero>=0 && fila_homero<MAX_TERRENO)){
        juego->homero.posicion.col=columna_homero;
        juego->homero.posicion.fil=fila_homero;
    }
}

// PRE: La accion anteriormente guardado, valor por referencia del struct juego, y las coordenadas nuevas de homero pasadas por referencia. 
// POST: Se mueve la cantidad de veces que le colocamos limitado por el tamano del terreno y ademas si encontramos objetos (barriles y donas) cuando este hace un movimiento valido.. reacciona con estos.

void movimiento_propulsor_escalera(char ultima_accion, juego_t *juego, int *nueva_fila, int *nueva_columna){
    for (int j=0; j<MOV_PROPULSORES; j++){
        int fila_homero=juego->homero.posicion.fil;
        int columna_homero=juego->homero.posicion.col;

        if (ultima_accion=='W' || ultima_accion=='S'){
            (*nueva_fila)=fila_homero-1;
            (*nueva_columna)=columna_homero;    
        } 
        else if (ultima_accion=='A'){
            (*nueva_fila)=fila_homero-1;
            (*nueva_columna)=columna_homero-1;    
        }
        
        else if(ultima_accion=='D'){
            (*nueva_fila)=fila_homero-1;
            (*nueva_columna)=columna_homero+1;     
        }
        validacion_col_fil(juego, *nueva_columna, *nueva_fila);
        reaccion_escalera_objetos(juego);
    }
}

// PRE: La accion anteriormente guardado, valor por referencia del struct juego, y las coordenadas nuevas de homero pasadas por referencia. 
// POST: Se mueve la cantidad de veces que le colocamos limitado por el tamano del terreno y ademas si encontramos objetos (barras y ratas) cuando este hace un movimiento valido.. reacciona con todos.

void movimiento_propulsor_charco (char ultima_accion, juego_t *juego, int *nueva_fila, int *nueva_columna){
    for (int j=0; j<MOV_PROPULSORES; j++){
        int fila_homero=juego->homero.posicion.fil;
        int columna_homero=juego->homero.posicion.col;

        if (ultima_accion=='W' || ultima_accion=='S'){
            (*nueva_fila)=fila_homero+1;
            (*nueva_columna)=columna_homero;    
        } 
        
        else if (ultima_accion=='A'){
            (*nueva_fila)=fila_homero+1;
            (*nueva_columna)=columna_homero-1;    
        }
        
        else if(ultima_accion=='D'){
            (*nueva_fila)=fila_homero+1;
            (*nueva_columna)=columna_homero+1;       
        }

        validacion_col_fil(juego, *nueva_columna, *nueva_fila);
        reaccion_charco_objetos(juego);
    }
}
   
// PRE: Valor por referencia del struct juego anteriormente inicializado, la variable ultima_accion guardada con anterioridad pasadas por valor, coordenadas auxiliares inicializadas en 0, y la posicion del propulsor con el cual coincide con la posicion de homero.
// POST: Homero se desplaza 3 movimientos(dependiendo de la ultim accion), restandole 1 de energia.

void movimiento_propulsor(juego_t *juego,char ultima_accion, int *nueva_fila, int *nueva_columna, int posicion){
    if (juego->propulsores[posicion].tipo=='E'){
        movimiento_propulsor_escalera (ultima_accion, juego, nueva_fila, nueva_columna);
    }
    else{
        movimiento_propulsor_charco (ultima_accion, juego, nueva_fila, nueva_columna);
    }
}
            
// PRE: Valor por referencia del struct juego anteriormente inicializado, la variable ultima_accion guardada con anterioridad pasadas por valor.
// POST: Desplaza a homero, si esta se choca con algun propulsor.

void reaccion_propulsores_homero(juego_t *juego, char ultima_accion){
    coordenada_t posicion_homero= juego->homero.posicion;

    int nueva_fila=0;
    int nueva_columna=0;

    for(int i=0; i<(juego->cantidad_propulsores); i++){
        if(coordenadas_iguales(juego->propulsores[i].posicion, posicion_homero)){            
            movimiento_propulsor(juego, ultima_accion, &nueva_fila, &nueva_columna, i);
        } 
    } 
}

// PRE: El struct juego inicializado, el vector objetos llenos y la posicion del objeto.
// POST: Cambia la visibilidad en true.

void visibilidad_linterna_juego(juego_t *juego, objeto_t objetos[MAX_OBJETOS], int posicion){
    for (int i=0; i<juego->cantidad_herramientas; i++){
        if (coordenadas_iguales(juego->herramientas[i].posicion, objetos[posicion].posicion)){
            juego->herramientas[i].visible=true;
        }
    }

    for (int i=0; i<juego->cantidad_obstaculos; i++){
        if (coordenadas_iguales(juego->obstaculos[i].posicion, objetos[posicion].posicion)){
            juego->obstaculos[i].visible=true;
        }
    }
    for (int i=0; i<juego->cantidad_propulsores; i++){
        if (coordenadas_iguales(juego->propulsores[i].posicion, objetos[posicion].posicion)){
            juego->propulsores[i].visible=true;
        }
    }
    for (int i=0; i<juego->cantidad_barras; i++){
        if (coordenadas_iguales(juego->barras[i].posicion, objetos[posicion].posicion)){
            juego->barras[i].visible=true;
        }
    }
}
        

// PRE: El vector de objetos, el struct juego anteriormente inicializado pasado por referencia, la posicion del objeto con el que la distancia nos dio menor a 2, y la distancia que nos dio en la funcion ´distancia_linterna_objetos´.
// POST: Los objetos a esa distancia, cambia su estado booleano en true. 

void recorrer_linterna_objetos(objeto_t objetos[MAX_OBJETOS], juego_t *juego, int posicion, int distancia){
    if (distancia<=DISTANCIA_MANHATTAN){
        objetos[posicion].visible=true;
        visibilidad_linterna_juego(juego, objetos, posicion);
    }
}
            

// PRE: El vector de objetos con la cantidad de la misma, y coordenada (fila, columna) de algun objeto dentro del vector cuya distancia fue menor/igual a 2.
// POST: Calcula la distancia entre las coordenadas de homero y las coordenadas de todos los objetos que se encuentran en el juego y luego se modifica el struct si cumple con las condiciones de la funcion ´recorrer_linterna_objetos´.  

void distancia_linterna_objetos(objeto_t objetos[MAX_OBJETOS], int tope_objetos, int fila, int columna, juego_t *juego){
    for (int k = 0; k <tope_objetos; k++){                  
        int distancia= abs(fila - objetos[k].posicion.fil)+ abs(columna-objetos[k].posicion.col);
        recorrer_linterna_objetos(objetos, juego, k, distancia);
    }
}

// PRE: El struct juego anteriormente inicializado pasado por referencia, el vector objetos con su tope
// POST: Si la linterna se encuentra activada entonces la visibilidad de ciertos objetos que cumplan con la funcion recorrer objetos, cambian a true. En caso contrario seguiran en false.

void linterna_homero(juego_t *juego,objeto_t objetos[MAX_OBJETOS],int tope_objetos){
    if(juego->homero.linterna_activada){
        distancia_linterna_objetos(objetos, tope_objetos, juego->homero.posicion.fil,juego->homero.posicion.col, juego);
    }
}
    

// PRE: Obtener los valores por referencia del juego anteriormente inicializado, las coordenadas usadas con su tope.
// POST: Si la cantidad de linternas es mayor a 0, la linterna se activa, se le suma 1 movimiento y las posiciones de las ratas cambian cuando se enciende.
//      Si la cantidad de linternas es igual a 0, la linterna no se activa y su valor sera 0. 

void activar_linterna(juego_t *juego){
    if((juego->homero.cantidad_linternas)>0 && !(juego->homero.linterna_activada)){
        juego->homero.linterna_activada=true;
        (juego->homero.cantidad_linternas)--;
        (juego->homero.mov_linterna)++;
        coordenadas_ratas_interruptor_linterna(juego);
    }
    else if (juego->homero.cantidad_linternas==0){
        juego->homero.cantidad_linternas=0;
    }
}


// PRE: Valores por referencia del juego anteriormente inicializado.
// POST: Se le descuentan 1 movimiento de linterna por cada vez que se mueva homero y tenga la linterna encendida. Recordar que al activar la linterna, tiene solo 4 movimientos para que luego se apague.

void movimiento_linterna(juego_t *juego){
    if (juego->homero.linterna_activada){
        (juego->homero.mov_linterna)++;

        if(juego->homero.mov_linterna>=5){
            juego->homero.linterna_activada=false;
            juego->homero.mov_linterna=0;
        }
    }
}

// PRE: El struct juego anteriormente inicializado, la varible bool si se encuentra sobre el interruptor.
// POST: Reaccion de homero con todos los objetos del terreno si este se encuentra en la posicion exacta de estos objetos.

void reaccion_objetos(juego_t *juego, bool *esta_sobre_interruptor, char ultima_accion){
    reaccion_barras_homero(juego);
    reaccion_barril_homero(juego);
    reaccion_ratas_homero(juego);
    reaccion_donas_homero(juego);
    reaccion_interruptor_ratas_homero(juego, esta_sobre_interruptor);
    reaccion_propulsores_homero(juego, ultima_accion);
}
        
// PRE: Obtencion de los valores por referencia del juego, las coordenadas nuevas de homero, y la variable bool si se encuentra sobre el interruptor.
// POST: Como objetivo de la funcion es que homero este dentro de los limites del terreno siempre y que si encuentra algun objeto en su trayectoria, reaccione con todas. Asigna las nuevas coordenadas a las coordenadas reales de homero.

void validacion_col_fil_linterna(juego_t *juego, int columna_homero, int fila_homero, char ultima_accion, bool *esta_sobre_interruptor){
    if ((columna_homero>=0 && columna_homero<MAX_TERRENO)&&(fila_homero>=0 && fila_homero<MAX_TERRENO)){
        juego->homero.posicion.col=columna_homero;
        juego->homero.posicion.fil=fila_homero;
        (juego->homero.energia)--;

        movimiento_linterna(juego);
        reaccion_objetos(juego, esta_sobre_interruptor, ultima_accion);
    }
}

// PRE: Recibe la accion validada ingresada por el usuario, la variable de la accion anterior, el struct juego previamente inicializado y el bool si homero esta sobre el interruptor anteriormente inicializado. 
// POST: Segun la accion validada, si se encuentra dentro del terreno entonces el valor de las coordenadas auxiliares pasan a ser las coordenadas reales de homero y ademas se le quitara 1 de energia. En caso contrario no se movera y no gastara energia.

void posicion_accion_homero(char accion, char *ultima_accion, juego_t *juego, bool *esta_sobre_interruptor){
    coordenada_t posicion_homero=juego->homero.posicion;
    
    int nueva_fila=0;
    int nueva_columna=0;

    switch(accion){
        case 'W':
            nueva_fila=(posicion_homero.fil)-1;
            nueva_columna=posicion_homero.col;    

            (*ultima_accion)='W';
            break;
        case 'A':
            nueva_fila=(posicion_homero.fil);
            nueva_columna=(posicion_homero.col)-1;

            (*ultima_accion)='A';
            break;
        case 'S':
            nueva_fila=(posicion_homero.fil)+1;
            nueva_columna=posicion_homero.col;    

            (*ultima_accion)='S';
            break;
        default:
            nueva_fila=(posicion_homero.fil);
            nueva_columna=(posicion_homero.col)+1;

            (*ultima_accion)='D';
            break;
    }
    validacion_col_fil_linterna(juego, nueva_columna, nueva_fila, *ultima_accion, esta_sobre_interruptor);
}
        

// PRE: Se necesitara que la accion sea valida, el valor por referencia de la variable ultima accion declarada con anterioridad, valor por referencia del struct juego, coordenadas usadas con su tope, y las coordenadas nuevas de homero que se va a modificar dependiendo de la accion.
// POST: Nos devuelve una coordenada nueva (limitada por el tamano del terreno), dependiendo de la accion que se hizo, se le resta 1 de energia por cada movimiento y si tiene la linterna activada, se le resta 1 al movimiento que le quedan por utilizar.

void reaccion_accion_homero(char accion, char *ultima_accion, juego_t *juego, bool *esta_sobre_interruptor){    
    if (accion=='L'){
        activar_linterna(juego);
        (*ultima_accion)='L';
    }
    
    else{
        posicion_accion_homero(accion, ultima_accion, juego, esta_sobre_interruptor);
    }
    
}
    
// PRE: Obtener los valores por referencia del juego y el tope de herramientas inicializado en 0 posteriormente al llamado de la funcion, y las coordenadas usadas con su respectivo tope. 
// POST: Le agrego valores iniciales a todas las herramientas del juego, coordenadas distintas, tipo de objeto, y visibilidad. Con cada herramienta que se agrega, se le suma 1 al tope de herramientas y al tope de coordenadas.

void inicializar_herramientas(objeto_t herramientas[MAX_HERRAMIENTAS], juego_t *juego, int *tope_herramientas, coordenada_t coordenadas_usadas[MAX_OBJETOS], int *tope_coordenadas){
    for (int i=0; i< DONAS; i++){
        herramientas[*tope_herramientas].posicion= coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
        herramientas[*tope_herramientas].tipo= 'D';
        herramientas[*tope_herramientas].visible=false;
        (*tope_herramientas)++;
    }

    for (int i=0; i< INTERRUPTOR; i++){
        herramientas[*tope_herramientas].posicion= coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
        herramientas[*tope_herramientas].tipo= 'I';
        herramientas[*tope_herramientas].visible=true;
        (*tope_herramientas)++;
    }

    juego->cantidad_herramientas=(*tope_herramientas);
}

// PRE: Obtener los valores por referencia del juego y el tope de obstaculos inicializado en 0 posteriormente al llamado de la funcion, y las coordenadas usadas con su respectivo tope.  
// POST: Le agrego valores iniciales a todas los obstaculos del juego, coordenadas distintas, tipo de objeto, y visibilidad. Con cada obstaculo que se agrega, se le suma 1 al tope de obstaculos y al tope de coordenadas.

void inicializar_obstaculos(objeto_t obstaculos[MAX_OBSTACULOS],juego_t *juego, int *tope_obstaculos, coordenada_t coordenadas_usadas[MAX_OBJETOS], int *tope_coordenadas){
    for (int i=0; i< RATAS; i++){
        obstaculos[*tope_obstaculos].posicion= coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
        obstaculos[*tope_obstaculos].tipo= 'R';
        obstaculos[*tope_obstaculos].visible=false;
        (*tope_obstaculos)++;
    }

    for (int i=0; i< BARRIL; i++){
        obstaculos[*tope_obstaculos].posicion= coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
        obstaculos[*tope_obstaculos].tipo= 'A';
        obstaculos[*tope_obstaculos].visible=false;
        (*tope_obstaculos)++;
    }
    juego->cantidad_obstaculos=(*tope_obstaculos);
}

// PRE: Obtener los valores por referencia del juego y el tope de barras inicializado en 0 posteriormente al llamado de la funcion, y las coordenadas usadas con su respectivo tope. 
// POST: Le agrego valores iniciales a todas las barras del juego, coordenadas distintas, tipo de objeto, y visibilidad. Con cada barra que se agrega, se le suma 1 al tope de barras y al tope de coordenadas.

void inicializar_barras(objeto_t barras[MAX_BARRAS], juego_t *juego, int *tope_barras_total, coordenada_t coordenadas_usadas[MAX_OBJETOS], int *tope_coordenadas){
    for (int i=0; i< MAX_BARRAS; i++){
        barras[*tope_barras_total].posicion= coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
        barras[*tope_barras_total].tipo= 'B';
        barras[*tope_barras_total].visible=false;
        (*tope_barras_total)++;
    }
    juego->cantidad_barras=(*tope_barras_total);
}

// PRE: Obtener los valores por referencia del juego y el tope de propulsor inicializado en 0 posteriormente al llamado de la funcion, y las coordenadas usadas con su respectivo tope. 
// POST: Le agrego valores iniciales a todas las propulsor del juego, coordenadas distintas, tipo de objeto, y visibilidad. Con cada herramienta que se agrega, se le suma 1 al tope del propulsor y al tope de coordenadas.

void inicializar_propulsores(objeto_t propulsores[MAX_PROPULSORES],juego_t *juego, int *tope_propulsor_total, coordenada_t coordenadas_usadas[MAX_OBJETOS], int *tope_coordenadas){
    for (int i=0; i<PROPULSOR_E; i++){
        propulsores[*tope_propulsor_total].posicion= coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
        propulsores[*tope_propulsor_total].tipo= 'E';
        propulsores[*tope_propulsor_total].visible=false;
        (*tope_propulsor_total)++;
    }

    for (int i=0; i<PROPULSOR_C; i++){
        propulsores[*tope_propulsor_total].posicion= coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
        propulsores[*tope_propulsor_total].tipo= 'C';
        propulsores[*tope_propulsor_total].visible=false;
        (*tope_propulsor_total)++;
    }
    juego->cantidad_propulsores=(*tope_propulsor_total);
}

// PRE: Los valores del juego pasados por referencia, las coordenadas de un vector vacio con su tope de coordenadas inicializada en 0.
// POST: Valores de homero inicializados como la cantidad de linternas, cantidad de barras, energia, linterna activada, movimientos de lintterna y posicion aleatoria.

void inicializar_homero(juego_t *juego, coordenada_t coordenadas_usadas[MAX_OBJETOS], int *tope_coordenadas){
    juego->homero.posicion=coordenadas_aleatorias_inicial(coordenadas_usadas, tope_coordenadas);
    juego->homero.cantidad_linternas=5;
    juego->homero.cantidad_barras=0;
    juego->homero.energia= 400;
    juego->homero.linterna_activada=false;
    juego->homero.mov_linterna=0;
}
  
  
// PRE: El terreno inicializado, el vector objetos lleno con su tope.
// POST: Si los objetos estan visibles, coloca el tipo de objeto en el terreno segun sus coordenadas especificas.

void objetos_visibles_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], objeto_t objetos[MAX_OBJETOS], int tope_objetos){
    for(int i=0; i<tope_objetos; i++){
        if (objetos[i].visible){
            terreno[objetos[i].posicion.fil][objetos[i].posicion.col]=objetos[i].tipo;
        }
    }
}          

// PRE: Tener una matriz que acepte datos de tipo char anteriormente inicializados, el struct juego y el vector objetos con su tope.
// POST: Se colocan los objetos visibles dentro del terreno cuando homero se posiciona en el interruptor.

void mostrar_interruptor_objetos_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], juego_t juego, objeto_t objetos[MAX_OBJETOS], int tope_objetos){
    coordenada_t posicion_homero= juego.homero.posicion;
    
    for (int j=0; j< (juego).cantidad_herramientas; j++){
        if((juego).herramientas[j].tipo=='I'){
            terreno[(juego).herramientas[j].posicion.fil][(juego).herramientas[j].posicion.col]='I';
            if (coordenadas_iguales(juego.herramientas[j].posicion, posicion_homero)){
                printf("\n\tEstas sobre un interruptor (I), se enciende la luz y cambian las posiciones de las ratas (R)!!!\n\n");
                
                objetos_visibles_terreno(terreno, objetos, tope_objetos);
            }
        }
    }
}

// PRE: Tener una matriz que acepte datos de tipo char anteriormente inicializados, el struct juego y el vector objetos con su tope.
// POST: Se colocan los objetos visibles que esten a una distancia menor igual a 2, cuando homero enciende la linterna.

void mostrar_linterna_objetos_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], juego_t juego, objeto_t objetos[MAX_OBJETOS], int tope_objetos){
    if (juego.homero.linterna_activada){
        printf("\n\tEncendiste la linterna (L), se veran los objetos que se encuentren a 2 distancia manhattan!!\n");
        printf("\n\tSolo tienes 4 movimientos, movimientos %i\n\n", juego.homero.mov_linterna);

        objetos_visibles_terreno(terreno, objetos, tope_objetos);
    }
}

// PRE: Color del titulo.
// POST: Imprime el titulo con el color que se eligio.

void imprimir_titulo(const char *color){
    printf("\n\t\t\t%sDESASTRE NATURAL%s\t\n\n", color, RESET_COLOR);
}

// PRE: Color del encabezado
// POST: Imprime el ancabezado con tabulaciones y saltos de linea.

void imprimir_encabezado(const char *color){
    printf("%s", color);
    printf("\t=================================================================\t\n");
    printf(RESET_COLOR);
}

// PRE: El struct juego anteriormente inicializado pasados por valor.
// POST: Imprime las herramientas de homero actualizadas.

void imprimir_herramientas_homero(juego_t juego){
    imprimir_titulo(COLOR_TITULO);
    imprimir_encabezado(COLOR_MAGENTA);
    printf("\t%s|| Linternas = %s %3i \t%s|| Barras = %s %3i \t%s|| Energia = %s %3i\n",COLOR_ROSA, COLOR_VERDE, juego.homero.cantidad_linternas,COLOR_ROSA, COLOR_VERDE, juego.homero.cantidad_barras,COLOR_ROSA, COLOR_VERDE, juego.homero.energia);
    imprimir_encabezado(COLOR_MAGENTA);
    printf("\n");
}
    
// PRE: El struct juego inicializado en ´inicializar_juego´.
// POST: Imprime las reacciones cuando homero choca con barriles o ratas.

void reaccion_barriles_ratas_objetos_terreno(juego_t juego){
    for (int i=0 ; i<juego.cantidad_obstaculos; i++){
        if(juego.obstaculos[i].tipo=='A' && coordenadas_iguales(juego.obstaculos[i].posicion, juego.homero.posicion)){
            printf("\tOh no!! chocaste con un barril, se te restara 15 de energia\n\n");
        }
        else if(juego.obstaculos[i].tipo=='R' && coordenadas_iguales(juego.obstaculos[i].posicion, juego.homero.posicion)){
            printf("\tOh no!! chocaste con una rata, se te restara 1 a la linterna\n\n");
        }
    }    
}

// El terreno inicializado, el struct juego inicializado en ´inicializar_juego´, el vector objetos_t lleno con su respectivo tope.
// Muestra los objetos y reacciones(barriles y ratas) cuando se cumplan las condiciones de las funciones secundarias  

void mostrar_reacciones_objetos_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], juego_t juego, objeto_t objetos[MAX_OBJETOS], int tope_objetos){
    mostrar_interruptor_objetos_terreno(terreno, juego, objetos, tope_objetos);

    mostrar_linterna_objetos_terreno(terreno, juego, objetos, tope_objetos);

    reaccion_barriles_ratas_objetos_terreno(juego);
}



// Funciones principales del juego

void inicializar_juego(juego_t *juego){
    int tope_herramientas=0;
    int tope_obstaculos=0;
    int tope_barras_total=0;
    int tope_propulsor_total=0;

    coordenada_t coordenadas_usadas[MAX_OBJETOS];
    int tope_coordenadas=0;

    inicializar_homero(juego, coordenadas_usadas, &tope_coordenadas);

    inicializar_herramientas(juego->herramientas, juego, &tope_herramientas, coordenadas_usadas, &tope_coordenadas);
    
    inicializar_obstaculos(juego->obstaculos, juego, &tope_obstaculos, coordenadas_usadas, &tope_coordenadas);
    
    inicializar_barras(juego->barras, juego, &tope_barras_total, coordenadas_usadas, &tope_coordenadas);
    
    inicializar_propulsores(juego->propulsores, juego, &tope_propulsor_total, coordenadas_usadas, &tope_coordenadas);
    return;
}

void realizar_jugada(juego_t *juego, char accion){
    char ultima_accion;

    objeto_t objetos[MAX_OBJETOS];
    int tope_objetos=0;

    bool esta_sobre_interruptor=false;

    reaccion_accion_homero(accion, &ultima_accion, juego, &esta_sobre_interruptor);
    
    interruptor_homero(juego, esta_sobre_interruptor);

    objetos_en_juego(objetos, &tope_objetos, *juego);

    linterna_homero(juego, objetos, tope_objetos);

    objetos_no_visibles(juego, esta_sobre_interruptor);         
    
    return;
}

void mostrar_juego(juego_t juego){
    char terreno[MAX_TERRENO][MAX_TERRENO];

    objeto_t objetos[MAX_OBJETOS];
    int tope_objetos=0;

    imprimir_herramientas_homero(juego);

    objetos_en_juego(objetos, &tope_objetos, juego);

    inicializar_terreno(terreno);

    mostrar_reacciones_objetos_terreno(terreno, juego, objetos, tope_objetos);

    imprimir_terreno(terreno, juego);
}

int estado_juego(juego_t juego){
    if ((juego.homero.energia>0) && (juego.homero.cantidad_barras==MAX_BARRAS)){
        return GANASTE;
    }
    else if((juego.homero.energia==0) && (juego.homero.cantidad_barras<MAX_BARRAS)){
        return PERDISTE;
    }
    return SIGUES_JUGANDO;
}