#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archivos.h"

int guardarTutoriaTexto(Tutoria tut) {
	FILE *archivo = fopen(RUTA_TUTORIAS, "a");
	if (!archivo) return 0;
	
	// Guardamos incluyendo las observaciones del estudiante y docente
	fprintf(archivo, "%d;%s;%s;%s;%s;%d;%s;%s\n",
			tut.idTutoria, tut.idEstudiante, tut.idDocente,
			tut.fecha, tut.hora, tut.estadoCita,
			tut.observacionEstudiante, tut.observacionDocente);
	fclose(archivo);
	return 1;
}

static void copiarCampo(char *destino, size_t tamDestino, const char *origen) {
	strncpy(destino, origen, tamDestino - 1);
	destino[tamDestino - 1] = '\0';
}

// Lee un registro de tutoria por linea y separa manualmente los 8 campos por
// ';'. A diferencia de fscanf("%[^;]", ...), esto SI permite campos vacios
// (por ejemplo una observacion en blanco), que con los especificadores %[...]
// provocaban una falla de conversion y descartaban el registro completo.
int leerSiguienteTutoria(FILE *archivo, Tutoria *t) {
	char linea[2048];
	
	while (fgets(linea, sizeof(linea), archivo)) {
		size_t len = strlen(linea);
		while (len > 0 && (linea[len - 1] == '\n' || linea[len - 1] == '\r')) {
			linea[--len] = '\0';
		}
		if (len == 0) continue; // linea en blanco, ignorar y seguir
		
		char *campos[8];
		int n = 0;
		char *cursor = linea;
		campos[n++] = cursor;
		while (n < 8) {
			char *sep = strchr(cursor, ';');
			if (!sep) break;
			*sep = '\0';
			cursor = sep + 1;
			campos[n++] = cursor;
		}
		
		if (n != 8) {
			// Registro corrupto o de un formato antiguo con menos campos:
			// se descarta esta linea y se continua con la siguiente.
			continue;
		}
		
		char *fin;
		long idTutoria = strtol(campos[0], &fin, 10);
		if (*fin != '\0') continue; // el ID no era numerico: linea invalida
		
		long estado = strtol(campos[5], &fin, 10);
		if (*fin != '\0') continue; // el estado no era numerico: linea invalida
		
		t->idTutoria = (int)idTutoria;
		t->estadoCita = (int)estado;
		copiarCampo(t->idEstudiante, sizeof(t->idEstudiante), campos[1]);
		copiarCampo(t->idDocente, sizeof(t->idDocente), campos[2]);
		copiarCampo(t->fecha, sizeof(t->fecha), campos[3]);
		copiarCampo(t->hora, sizeof(t->hora), campos[4]);
		copiarCampo(t->observacionEstudiante, sizeof(t->observacionEstudiante), campos[6]);
		copiarCampo(t->observacionDocente, sizeof(t->observacionDocente), campos[7]);
		return 1;
	}
	
	return 0; // fin de archivo
}

int generarIdCorrelativoTutoria() {
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	if (!archivo) return 1;
	
	int id = 0;
	Tutoria tut;
	while (leerSiguienteTutoria(archivo, &tut)) {
		id = tut.idTutoria;
	}
	fclose(archivo);
	return id + 1;
}

void restablecerBaseDeDatos() {
	FILE *f1 = fopen(RUTA_USUARIOS, "w"); if (f1) fclose(f1);
	FILE *f2 = fopen(RUTA_ESTUDIANTES, "w"); if (f2) fclose(f2);
	FILE *f3 = fopen(RUTA_DOCENTES, "w"); if (f3) fclose(f3);
	FILE *f4 = fopen(RUTA_TUTORIAS, "w"); if (f4) fclose(f4);
	printf("\n[OK] Base de datos restablecida por completo. Registros borrados.\n");
	printf("Presione ENTER para continuar..."); getchar();
}
