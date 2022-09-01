#include "split.h"
#include <stdlib.h>

#define FIN '\0'



/*
    Recibe un string y su separador. Cuenta la cantidad de veces que aparece el separador
    en el string.
*/
size_t  contar_cantidad_separadores(const char* string, char separador){
    size_t  cantidad_separadores = 0;
    size_t i = 0;
    
    while(string[i] != FIN){
        if(string[i] == separador)
            cantidad_separadores++;

        i++;
    }

    return cantidad_separadores;
}
 

/*
    Recibe un string y su separador. Cuenta la cantidad de letras hasta que se encuentre
    con el separador o el final del string.
*/
size_t contar_cantidad_letras(const char* string, char separador){

    size_t cantidad_letras = 0;

    while(string[cantidad_letras] != FIN && string[cantidad_letras] != separador){
        cantidad_letras++;
    }

    return cantidad_letras;
}


/*
    Recibe el string y la cantidad de letras necesarias. Copia el contenido del string desde la posicion
    actual hasta la cantidad de letras en un string nuevo y lo devuelve.
*/
char* copiar_palabra(const char* string, size_t cantidad_letras){
    char* string_nuevo = malloc((cantidad_letras + 1)*sizeof(char));
    if (!string_nuevo){
        return NULL;
    }


    for (size_t i = 0; i < cantidad_letras; ++i){
        string_nuevo[i] = string[i];
    }

    string_nuevo[cantidad_letras] = FIN;
    return string_nuevo;
}



/*
    Recibe un vector de punter char y libera uno a uno cada puntero
*/
void liberar_todo(char** vector){
    int i = 0;
    while(vector[i]){
        free(vector[i]);
        i++;
    }
}


/*
    Recibe un string y un separador. Busca el separador por todo el string, divide el
    contenido que esta entre cada separador y devuelve un vector con cada contenido separado 
    en un nuevo string.

    EJ: ("123:456:789:0", ':') => ["123", "456", "789", "0", NULL]
*/
char** split(const char* string, char separador){
    if (!string){
        return NULL;
    }
    

    size_t cantidad_separadores = contar_cantidad_separadores(string, separador);
    size_t cantidad_palabras = (cantidad_separadores + 1);
    

    char** palabras_separadas = calloc((cantidad_palabras + 1), sizeof(void*));;


    if (!palabras_separadas)
        return NULL;

    for (int n = 0; n < cantidad_palabras; ++n){
        size_t indice_string = contar_cantidad_letras(string, separador);
        char* string_auxiliar = copiar_palabra(string, indice_string);

        if (!string_auxiliar){
            liberar_todo(palabras_separadas);
            return NULL;
        }


        palabras_separadas[n] = string_auxiliar;
        string += indice_string + 1;
    }    

    return palabras_separadas;
}