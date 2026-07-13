#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include "comunes.h"

int guardarTutoriaTexto(Tutoria tut);
int generarIdCorrelativoTutoria();
void restablecerBaseDeDatos();

// Lee el siguiente registro valido de tutoria desde 'archivo' hacia 't'.
// Si una linea no calza con el formato esperado (por ejemplo datos antiguos
// o corruptos), la descarta por completo y sigue con la siguiente linea en
// vez de quedarse "atascada" en el mismo punto del archivo.
// Devuelve 1 si logro leer un registro completo, 0 si llego al final del archivo.
int leerSiguienteTutoria(FILE *archivo, Tutoria *t);

#endif
