#include <stdio.h>
#include <string.h>
#include "reportes.h"
#include "comunes.h"
#include "archivos.h"
#include "validaciones.h"

static void obtenerNombreEstudiante(const char *id, char *destino) {
	FILE *f = fopen(RUTA_ESTUDIANTES, "r");
	strcpy(destino, "No Encontrado");
	if (!f) return;
	Estudiante e;
	while (fscanf(f, "%19[^;];%99[^;];%99[^;];%d;%99s\n", e.idEstudiante, e.nombreCompleto, e.carrera, &e.semestre, e.correoElectronico) != EOF) {
		if (strcmp(e.idEstudiante, id) == 0) {
			strcpy(destino, e.nombreCompleto);
			break;
		}
	}
	fclose(f);
}

static void obtenerNombreDocente(const char *id, char *destino) {
	FILE *f = fopen(RUTA_DOCENTES, "r");
	strcpy(destino, "No Encontrado");
	if (!f) return;
	Docente d;
	while (fscanf(f, "%19[^;];%99[^;];%99[^;];%c;%99s\n", d.idDocente, d.nombreCompleto, d.catedraMateria, &d.estado, d.correoElectronico) != EOF) {
		if (strcmp(d.idDocente, id) == 0) {
			strcpy(destino, d.nombreCompleto);
			break;
		}
	}
	fclose(f);
}

void mostrarReporteTutorias() {
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	if (!archivo) {
		printf("\nNo hay tutorias registradas en el sistema.\n");
		return;
	}
	
	Tutoria t;
	char nombreEst[MAX_CADENA];
	char nombreDoc[MAX_CADENA];
	
	printf("\n=================================================================================================================================\n");
	printf("                                              REPORTE CENTRALIZADO DE TUTORIAS\n");
	printf("=================================================================================================================================\n");
	printf("%-5s | %-20s | %-20s | %-10s | %-5s | %-10s | %-20s | %-20s\n",
		   "ID", "Estudiante", "Docente", "Fecha", "Hora", "Estado", "Obs. Estudiante", "Obs. Docente");
	printf("---------------------------------------------------------------------------------------------------------------------------------\n");
	
	while (leerSiguienteTutoria(archivo, &t)) {
		obtenerNombreEstudiante(t.idEstudiante, nombreEst);
		obtenerNombreDocente(t.idDocente, nombreDoc);
		
		char txtEstado[15];
		if (t.estadoCita == 0) strcpy(txtEstado, "Pendiente");
		else if (t.estadoCita == 1) strcpy(txtEstado, "Programada");
		else if (t.estadoCita == 2) strcpy(txtEstado, "Completada");
		else strcpy(txtEstado, "Cancelada");
		
		printf("%-5d | %-20s | %-20s | %-10s | %-5s | %-10s | %-20s | %-20s\n",
			   t.idTutoria, nombreEst, nombreDoc, t.fecha, t.hora, txtEstado,
			   textoObservacionSegura(t.observacionEstudiante), textoObservacionSegura(t.observacionDocente));
	}
	printf("=================================================================================================================================\n");
	fclose(archivo);
	printf("\nPresione ENTER para continuar..."); getchar();
}

void contarAlumnosPorHorario(int conteo[5]) {
	// Los 5 bloques fijos institucionales definidos en asignarTutoriaAdministrador()
	const char *claveHora[5] = { "08:00", "09:30", "11:00", "14:00", "15:30" };
	for (int i = 0; i < 5; i++) conteo[i] = 0;
	
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	if (!archivo) return;
	
	Tutoria t;
	while (leerSiguienteTutoria(archivo, &t)) {
		// Solo cuenta tutorias que estan ACTUALMENTE ocupando el horario, es
		// decir en estado Activa (1). Las Pendientes (0) aun no tienen hora
		// real ("PEND"), y las Completadas (2) o Canceladas (3) ya liberaron
		// el cupo, por lo que no deben seguir contando como ocupado.
		if (t.estadoCita != 1) continue;
		for (int i = 0; i < 5; i++) {
			if (strcmp(t.hora, claveHora[i]) == 0) {
				conteo[i]++;
				break;
			}
		}
	}
	fclose(archivo);
}

void mostrarEstadisticaHorarios() {
	const char *etiqueta[5] = { "Bloque A: 08:00 - 09:00", "Bloque B: 09:30 - 10:30",
		"Bloque C: 11:00 - 12:00", "Bloque D: 14:00 - 15:00",
		"Bloque E: 15:30 - 16:30" };
	int conteo[5];
	contarAlumnosPorHorario(conteo);
	
	printf("\n==================================================================\n");
	printf("         CANTIDAD DE ALUMNOS ASIGNADOS POR HORARIO\n");
	printf("==================================================================\n");
	printf("%-28s | %s\n", "Horario", "Alumnos Asignados");
	printf("------------------------------------------------------------------\n");
	for (int i = 0; i < 5; i++) {
		printf("%-28s | %d\n", etiqueta[i], conteo[i]);
	}
	printf("==================================================================\n");
	printf("\nPresione ENTER para continuar..."); getchar();
}
