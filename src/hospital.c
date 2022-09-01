#include "hospital.h"
#include "split.h"
#include <string.h>
#include <stdio.h>

#define FIN '\0'
#define SALTO_DE_LINEA '\n'

struct _hospital_pkm_t{
    size_t cantidad_pokemon;
    pokemon_t* vector_pokemones;

    size_t cantidad_entrenadores;
};
 
struct _pkm_t{
    char* nombre;
    size_t nivel;
};


/*
*   Pre condiciones: recibe el nombre de un archivo.
*
*   Post condiciones: lee una linea y la devuelve.
*
*/
char* leer_linea_de_archivo(FILE* archivo){
    size_t caracteres_leidos = 0;
    size_t tope_dinamico = 200;
    char* linea_leida = malloc(tope_dinamico*sizeof(char));

    if (!linea_leida)
        return NULL;

    while(fgets(linea_leida + caracteres_leidos, (int)(tope_dinamico - caracteres_leidos), archivo)){
        size_t leido = strlen(linea_leida + caracteres_leidos);
        
        if (leido > 0 && *(linea_leida + caracteres_leidos + leido - 1) == SALTO_DE_LINEA){
            *(linea_leida + caracteres_leidos + leido - 1) = FIN;
            return linea_leida;
        }
        else{
            char* auxiliar = realloc(linea_leida, tope_dinamico*2);
            
            if (!auxiliar){
                free(auxiliar);
                return NULL;
            }
            
            linea_leida = auxiliar;
            tope_dinamico *= 2;
        }
        caracteres_leidos += leido;
    }

    if (caracteres_leidos == 0){
        free(linea_leida);
        return NULL;
    }
    return linea_leida;
}


/*
*   Pre condiciones: recibe un string y un separador en especifico.
*
*   Post condiciones: cuenta la cantidad de veces que aparece el separador en el string dado.
*
*/
size_t  contar_separadores(char* string, char separador){
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
*   Pre condiciones: debe recibir un puntero a hospital_t y un vector de strings.
*   en la linea leida.
*
*   Post condiciones: actualiza los datos de los pokemones en el hospital_t.
*
*/
bool actualizar_pkmnes_en_hospital(hospital_t* hospital, char** datos_separados, size_t cantidad_pokemones_en_archivo){
    int j = 2;
    int nivel_pokemon;

    for (size_t i = 0; i < cantidad_pokemones_en_archivo; ++i){
        size_t largo_nombre_pokemon = (strlen(datos_separados[j]) + 1);
        hospital->vector_pokemones[(hospital->cantidad_pokemon) - cantidad_pokemones_en_archivo + i].nombre = calloc(largo_nombre_pokemon, sizeof(char));
        
        if (!(hospital->vector_pokemones[(hospital->cantidad_pokemon) - cantidad_pokemones_en_archivo + i].nombre)){
            return true;
        }

        strcpy(hospital->vector_pokemones[(hospital->cantidad_pokemon) - cantidad_pokemones_en_archivo + i].nombre, datos_separados[j]);
        nivel_pokemon = atoi(datos_separados[j + 1]);
        hospital->vector_pokemones[(hospital->cantidad_pokemon) - cantidad_pokemones_en_archivo + i].nivel = (size_t)nivel_pokemon;
        j += 2;
    }

    return false;
}


/*
*   Pre condiciones: debe recibir un puntero a hospital_t, un vector de strings y la cantidad de pokemones
*   en la linea leida.
*
*   Post condiciones: actualiza los datos del hospital_t pasado en los parametros con los datos
*   del vector de strings.
*
*/
bool subir_datos_al_hospital(hospital_t* hospital, char** datos_separados, size_t cantidad_pokemones_en_archivo){
    if(!hospital)
        return true;

    pokemon_t* vector_pokemones_auxiliar = realloc(hospital->vector_pokemones, (hospital->cantidad_pokemon)*sizeof(pokemon_t));
    if(!vector_pokemones_auxiliar)
        return true;

    hospital->vector_pokemones = vector_pokemones_auxiliar;

    if(actualizar_pkmnes_en_hospital(hospital, datos_separados, cantidad_pokemones_en_archivo))
        return true;
    
    return false;
}


/*
*   Pre condiciones: --------------------------------------------------------
*
*   Post condiciones: Crea un hospital_t y inicializa la cantidad de entrenadores en 0.
*
*/
hospital_t* hospital_crear(){
    hospital_t* hospital_nuevo = calloc(1, sizeof(hospital_t));
    if (!hospital_nuevo)
        return NULL;
    hospital_nuevo->cantidad_entrenadores = 0;

    return hospital_nuevo;
}


/*
*   Pre condiciones: recibe un puntero a hospital_t y el nombre del archivo a leer.
*
*   Post condiciones: lee todas las lineas del archivo, agrega todos estos datos al hospital
*   y devuelve true si todo funciono correctamente. De no ser asi devuelve false.
*
*/
bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){    
    FILE* archivo_leido = fopen(nombre_archivo, "r");
    
    if(!archivo_leido)
        return NULL;

    bool fallo_subir_datos = false;
    size_t cantidad_palabras;

    char* datos_leidos = leer_linea_de_archivo(archivo_leido);
    while(!fallo_subir_datos && datos_leidos != NULL){
        char** datos_separados = split(datos_leidos, ';');
        if (!datos_leidos){
            return false;
        }
        cantidad_palabras = (contar_separadores(datos_leidos, ';') + 1);
        hospital->cantidad_pokemon += ((cantidad_palabras - 2) / 2);
        hospital->cantidad_entrenadores++;
        fallo_subir_datos = subir_datos_al_hospital(hospital, datos_separados, ((cantidad_palabras - 2) / 2));
        
        free(datos_leidos);
        datos_leidos = leer_linea_de_archivo(archivo_leido);
        
        for (int n = 0; n < cantidad_palabras; ++n){
            free(datos_separados[n]);
        }
        free(datos_separados);
    }
    
    if(fallo_subir_datos){
        return false;    
    }
    
    fclose(archivo_leido);
    return true;
}


/*
*   Pre condiciones: recibe un puntero a hospital_t.
*
*   Post condiciones: devuelve la cantidad de pokemones que se encuentran en el hospital.
*   Si el hospital es NULL devuelve 0.
*
*/
size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if (!hospital){
        return 0;
    }
    return hospital->cantidad_pokemon;
}


/*
*   Pre condiciones: recibe un puntero a hospital_t.
*
*   Post condiciones: devuelve la cantidad de entrenadores que se encuentran en el hospital.
*   Si el hospital es NULL devuelve 0.
*
*/
size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if (!hospital){
        return 0;
    }
    return hospital->cantidad_entrenadores;
}


/*
*   Pre condiciones: recibe un puntero a hospital_t.
*
*   Post condiciones: ordena de forma alfabetica, utilizando el algoritmo de burbujeo, el vector
*   de pokemones.
*
*/
void ordernar_pokemones_alfabeticamente(hospital_t* hospital){
    for (size_t i = 0; i < hospital->cantidad_pokemon; ++i){
        for (size_t j = 0; j < (hospital->cantidad_pokemon - i - 1); ++j){
            if (strcmp(hospital->vector_pokemones[j].nombre, hospital->vector_pokemones[j+1].nombre) > 0){
                
                size_t largo_nombre_pos_min = (strlen(hospital->vector_pokemones[j+1].nombre) + 1);
                char* auxiliar = malloc(largo_nombre_pos_min*sizeof(char));
                strcpy(auxiliar, hospital->vector_pokemones[j+1].nombre);
                
                size_t largo_nombre_pos_actual = (strlen(hospital->vector_pokemones[j].nombre) + 1);
                hospital->vector_pokemones[j+1].nombre = realloc(hospital->vector_pokemones[j+1].nombre, largo_nombre_pos_actual*sizeof(char));
                strcpy(hospital->vector_pokemones[j+1].nombre, hospital->vector_pokemones[j].nombre);

                hospital->vector_pokemones[j].nombre = realloc(hospital->vector_pokemones[j].nombre, largo_nombre_pos_min*sizeof(char));

                if (!auxiliar)
                    return;
                if (!hospital->vector_pokemones[j+1].nombre)
                    return;
                strcpy(hospital->vector_pokemones[j].nombre, auxiliar);
                size_t nivel_auxiliar = hospital->vector_pokemones[j].nivel;
                hospital->vector_pokemones[j+1].nivel = hospital->vector_pokemones[j].nivel;
                hospital->vector_pokemones[j].nivel = nivel_auxiliar; 
                
                free(auxiliar);
            }
        }

        
    }

}


/*
*   Pre condiciones: recibe un puntero a hospital_t y una funcion que deve devolver un booleano y su parametro
*   es un puntero a un pokemon_t.
*
*   Post condiciones: aplica la funcion pasada por parametro a todos los pokemones del hospital
*   en orden alfabetico y devuelve a cuantos de estos al aplicarle la funcion devuelve true 
*   (al primer false corta la iteracion y devuelve la cantidad de pokemones a los que se le aplico
*   la funcion hasta ese momento, contando el que devolvio false). Si el hospital es NULL devuelve 0.
*
*/
size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if (!hospital)
        return 0;
    
    if (!funcion)
        return 0;

    ordernar_pokemones_alfabeticamente(hospital);

    bool funciona = true;
    size_t pokemones_a_los_que_se_aplico_funcion = 1;
    for (size_t i = 0; i < hospital->cantidad_pokemon; ++i){
        funciona = funcion((&(*hospital).vector_pokemones[i]));
        if (!funciona){
            return (pokemones_a_los_que_se_aplico_funcion);
        }
        pokemones_a_los_que_se_aplico_funcion++;
    }


    return hospital->cantidad_pokemon;
}


/*
*   Pre condiciones: recibe un puntero a hospital_t.
*
*   Post condiciones: libera toda la memoria que se habia pedido para cada variable que haya usado malloc
*   o calloc y finalmente libera la memoria del hospital entero.
*
*/
void hospital_destruir(hospital_t* hospital){
    if (!hospital)
        return;
    for (int i = 0; i < hospital->cantidad_pokemon; ++i){
        free(hospital->vector_pokemones[i].nombre);    
    }

    free(hospital->vector_pokemones);
    free(hospital);
}


/*
*   Pre condiciones: recibe un puntero a pokemon_t.
*
*   Post condiciones: devuelve el nivel del pokemon. Si el pokemon es NULL devuelve 0. 
*
*/
size_t pokemon_nivel(pokemon_t* pokemon){
    if (!pokemon)
        return 0;

    return pokemon->nivel;
}


/*
*   Pre condiciones: recibe un puntero a pokemon_t.
*
*   Post condiciones: devuelve el nombre del pokemon. Si el pokemon es NULL devuelve NULL.
*
*/
const char* pokemon_nombre(pokemon_t* pokemon){
    if (!pokemon)
        return NULL;
    
    return pokemon->nombre;
}

bool funcion_aplicar(pokemon_t* p){
    if(strcmp(p->nombre, "miltank") != 0){
        return true;    
    }
    return false;
}