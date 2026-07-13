#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tutorias.h"
#include "archivos.h"
#include "docentes.h"
#include "estudiantes.h"
#include "comunes.h"
#include "validaciones.h"
#include "reportes.h"

void solicitarTutoriaEstudiante(const char *correoEstudiante) {
	char idEst[20] = "";
	FILE *fEst = fopen(RUTA_ESTUDIANTES, "r");
	if (fEst) {
		Estudiante est;
		while (fscanf(fEst, "%19[^;];%99[^;];%99[^;];%d;%99s\n", est.idEstudiante, est.nombreCompleto, est.carrera, &est.semestre, est.correoElectronico) != EOF) {
			if (strcmp(est.correoElectronico, correoEstudiante) == 0) {
				strcpy(idEst, est.idEstudiante);
				break;
			}
		}
		fclose(fEst);
	}
	
	if (strlen(idEst) == 0) {
		printf("[ERROR] Su ficha academica no esta completa. No puede solicitar tutorias.\n");
		printf("Presione ENTER para regresar..."); getchar();
		return;
	}
	
	printf("\n--- SOLICITAR NUEVA TUTORIA ---\n");
	Tutoria nueva;
	
	printf("Ingrese el tema o motivo de la tutoria (Observacion del Estudiante):\n> ");
	scanf("%99[^\n]", nueva.observacionEstudiante);
	getchar();
	
	nueva.idTutoria = generarIdCorrelativoTutoria();
	strcpy(nueva.idEstudiante, idEst);
	strcpy(nueva.idDocente, "PENDIENTE");
	strcpy(nueva.fecha, "PENDIENTE");
	strcpy(nueva.hora, "PEND");
	strcpy(nueva.observacionDocente, "Ninguna"); // Inicialmente sin revision del docente
	nueva.estadoCita = 0; // ESTADO 0 = PENDIENTE
	
	if (guardarTutoriaTexto(nueva)) {
		printf("\n[OK] Solicitud Nro %d en estado PENDIENTE enviada al Administrador.\n", nueva.idTutoria);
	}
	printf("Presione ENTER para continuar..."); getchar();
}

void asignarTutoriaAdministrador() {
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	if (!archivo) {
		printf("\nNo existen solicitudes de tutorias en el sistema.\n");
		printf("Presione ENTER para continuar..."); getchar();
		return;
	}
	
	Tutoria t;
	int contSolicitudes = 0;
	printf("\n====================================================================\n");
	printf("               SOLICITUDES DE TUTORIAS PENDIENTES                   \n");
	printf("====================================================================\n");
	printf("%-9s | %-15s | %-35s\n", "ID SOLIC.", "ID ESTUDIANTE", "TEMA / OBSERVACION ESTUDIANTE");
	printf("--------------------------------------------------------------------\n");
	
	while (leerSiguienteTutoria(archivo, &t)) {
		if (t.estadoCita == 0) {
			printf("%-9d | %-15s | %-35s\n", t.idTutoria, t.idEstudiante, textoObservacionSegura(t.observacionEstudiante));
			contSolicitudes++;
		}
	}
	fclose(archivo);
	
	if (contSolicitudes == 0) {
		printf("\n[AVISO] No hay solicitudes pendientes de asignacion en este momento.\n");
		printf("Presione ENTER para regresar..."); getchar();
		return;
	}
	
	int idBuscar;
	printf("\nIngrese el ID de la solicitud que desea ASIGNAR (o '0' para cancelar): ");
	if (scanf("%d", &idBuscar) != 1 || idBuscar == 0) {
		while (getchar() != '\n');
		return;
	}
	getchar();
	
	// Mostrar docentes disponibles
	limpiarPantalla();
	listarDocentes();
	char idDoc[20];
	printf("\nIngrese la Cedula/ID del docente asignado: ");
	scanf("%19s", idDoc);
	getchar();
	
	int conteoHorarios[5];
	contarAlumnosPorHorario(conteoHorarios);
	
	int opHorario = 0;
	char horaSeleccionada[MAX_HORA] = "";
	do {
		printf("\n--- SELECCIONE EL HORARIO FIJO INSTITUCIONAL ---\n");
		printf("1. Bloque A: 08:00 - 09:00 (Alumnos asignados: %d)\n", conteoHorarios[0]);
		printf("2. Bloque B: 09:30 - 10:30 (Alumnos asignados: %d)\n", conteoHorarios[1]);
		printf("3. Bloque C: 11:00 - 12:00 (Alumnos asignados: %d)\n", conteoHorarios[2]);
		printf("4. Bloque D: 14:00 - 15:00 (Alumnos asignados: %d)\n", conteoHorarios[3]);
		printf("5. Bloque E: 15:30 - 16:30 (Alumnos asignados: %d)\n", conteoHorarios[4]);
		printf("Seleccione una franja (1-5): ");
		if (scanf("%d", &opHorario) != 1) {
			while (getchar() != '\n');
			opHorario = -1;
		}
		
		switch (opHorario) {
		case 1: strcpy(horaSeleccionada, "08:00"); break;
		case 2: strcpy(horaSeleccionada, "09:30"); break;
		case 3: strcpy(horaSeleccionada, "11:00"); break;
		case 4: strcpy(horaSeleccionada, "14:00"); break;
		case 5: strcpy(horaSeleccionada, "15:30"); break;
		default: printf("[ERROR] Seleccion incorrecta.\n");
		}
	} while (opHorario < 1 || opHorario > 5);
	getchar();
	
	char fechaAsignada[MAX_FECHA];
	do {
		printf("Ingrese la fecha programada (DD/MM/AAAA): ");
		scanf("%10s", fechaAsignada);
		getchar();
		if (!validarFechaHabil(fechaAsignada)) {
			printf("[ERROR] Fecha invalida o corresponde a Sabado/Domingo. Las tutorias solo se pueden asignar en dias habiles (Lunes a Viernes).\n");
		}
	} while (!validarFechaHabil(fechaAsignada));
	
	archivo = fopen(RUTA_TUTORIAS, "r");
	FILE *temporal = fopen("temp.txt", "w");
	if (!archivo || !temporal) return;
	
	int aprobado = 0;
	while (leerSiguienteTutoria(archivo, &t)) {
		if (t.idTutoria == idBuscar && t.estadoCita == 0) {
			strcpy(t.idDocente, idDoc);
			strcpy(t.hora, horaSeleccionada);
			strcpy(t.fecha, fechaAsignada);
			t.estadoCita = 1; // CAMBIA A ESTADO 1 = ACTIVA
			aprobado = 1;
		}
		fprintf(temporal, "%d;%s;%s;%s;%s;%d;%s;%s\n", t.idTutoria, t.idEstudiante, t.idDocente, t.fecha, t.hora, t.estadoCita, t.observacionEstudiante, t.observacionDocente);
	}
	fclose(archivo); fclose(temporal);
	remove(RUTA_TUTORIAS); rename("temp.txt", RUTA_TUTORIAS);
	
	if (aprobado) {
		printf("[OK] La tutoria Nro %d ahora se encuentra ACTIVA.\n", idBuscar);
	} else {
		printf("[ERROR] No se pudo procesar la asignacion.\n");
	}
	printf("Presione ENTER para continuar..."); getchar();
}

void consultarMisTutoriasEstudiante(const char *correoEstudiante) {
	char idEst[20] = "";
	FILE *fEst = fopen(RUTA_ESTUDIANTES, "r");
	if (fEst) {
		Estudiante est;
		while (fscanf(fEst, "%19[^;];%99[^;];%99[^;];%d;%99s\n", est.idEstudiante, est.nombreCompleto, est.carrera, &est.semestre, est.correoElectronico) != EOF) {
			if (strcmp(est.correoElectronico, correoEstudiante) == 0) {
				strcpy(idEst, est.idEstudiante);
				break;
			}
		}
		fclose(fEst);
	}
	
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	if (!archivo) {
		printf("\nNo existen registros de tutorias.\n");
		printf("Presione ENTER para regresar..."); getchar();
		return;
	}
	
	Tutoria t;
	printf("\n========================================================================================================\n");
	printf("                                        ESTADO DE MIS TUTORIAS                                          \n");
	printf("========================================================================================================\n");
	printf("%-4s | %-11s | %-10s | %-6s | %-11s | %-20s | %-20s\n", "ID", "Docente ID", "Fecha", "Hora", "Estado", "Obs. Estudiante", "Obs. Docente");
	printf("--------------------------------------------------------------------------------------------------------\n");
	while (leerSiguienteTutoria(archivo, &t)) {
		if (strcmp(t.idEstudiante, idEst) == 0) {
			char estStr[15];
			if (t.estadoCita == 0) strcpy(estStr, "Pendiente");
			else if (t.estadoCita == 1) strcpy(estStr, "Activa");
			else if (t.estadoCita == 2) strcpy(estStr, "Completada");
			else strcpy(estStr, "Cancelada");
			
			printf("%-4d | %-11s | %-10s | %-6s | %-11s | %-20s | %-20s\n",
				   t.idTutoria, t.idDocente, t.fecha, t.hora, estStr,
				   textoObservacionSegura(t.observacionEstudiante), textoObservacionSegura(t.observacionDocente));
		}
	}
	fclose(archivo);
	printf("\nPresione ENTER para regresar..."); getchar();
}

void consultarTutoriasDocente(const char *correoDocente) {
	char idDoc[20] = "";
	FILE *fDoc = fopen(RUTA_DOCENTES, "r");
	if (fDoc) {
		Docente doc;
		while (fscanf(fDoc, "%19[^;];%99[^;];%99[^;];%c;%99s\n", doc.idDocente, doc.nombreCompleto, doc.catedraMateria, &doc.estado, doc.correoElectronico) != EOF) {
			if (strcmp(doc.correoElectronico, correoDocente) == 0) {
				strcpy(idDoc, doc.idDocente);
				break;
			}
		}
		fclose(fDoc);
	}
	
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	if (!archivo) {
		printf("\nNo tiene tutorias asignadas todavia.\n");
		printf("Presione ENTER para regresar..."); getchar();
		return;
	}
	
	Tutoria t;
	printf("\n%-4s | %-11s | %-10s | %-6s | %-11s | %-20s | %-20s\n", "ID", "Estud. ID", "Fecha", "Hora", "Estado", "Obs. Estudiante", "Obs. Docente");
	printf("--------------------------------------------------------------------------------------------------------\n");
	int contTutorias = 0;
	while (leerSiguienteTutoria(archivo, &t)) {
		if (strcmp(t.idDocente, idDoc) == 0) {
			char estStr[15];
			if (t.estadoCita == 0) strcpy(estStr, "Pendiente");
			else if (t.estadoCita == 1) strcpy(estStr, "Activa");
			else if (t.estadoCita == 2) strcpy(estStr, "Completada");
			else strcpy(estStr, "Cancelada");
			printf("%-4d | %-11s | %-10s | %-6s | %-11s | %-20s | %-20s\n",
				   t.idTutoria, t.idEstudiante, t.fecha, t.hora, estStr,
				   textoObservacionSegura(t.observacionEstudiante), textoObservacionSegura(t.observacionDocente));
			contTutorias++;
		}
	}
	fclose(archivo);
	
	if (contTutorias == 0) {
		printf("\n[AVISO] Aun no tiene tutorias asignadas.\n");
	}
	printf("\nPresione ENTER para regresar..."); getchar();
}

void modificarEstadoTutoriaDocente(const char *correoDocente) {
	consultarTutoriasDocente(correoDocente);
	int idBuscar;
	printf("\nIngrese el ID de la tutoria activa a finalizar (o '0' para regresar): ");
	if (scanf("%d", &idBuscar) != 1 || idBuscar == 0) { while (getchar() != '\n'); return; }
	getchar();
	
	char nuevaObsDocente[MAX_CADENA];
	printf("Ingrese las conclusiones u observaciones de la tutoria:\n> ");
	scanf("%99[^\n]", nuevaObsDocente);
	getchar();
	
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	FILE *temporal = fopen("temp.txt", "w");
	if (!archivo || !temporal) return;
	
	Tutoria t;
	int modificado = 0;
	while (leerSiguienteTutoria(archivo, &t)) {
		if (t.idTutoria == idBuscar && t.estadoCita == 1) {
			t.estadoCita = 2; // CAMBIA A ESTADO 2 = COMPLETADA
			strcpy(t.observacionDocente, nuevaObsDocente);
			modificado = 1;
		}
		fprintf(temporal, "%d;%s;%s;%s;%s;%d;%s;%s\n", t.idTutoria, t.idEstudiante, t.idDocente, t.fecha, t.hora, t.estadoCita, t.observacionEstudiante, t.observacionDocente);
	}
	fclose(archivo); fclose(temporal);
	remove(RUTA_TUTORIAS); rename("temp.txt", RUTA_TUTORIAS);
	
	if (modificado) printf("[OK] Tutoria marcada como COMPLETADA con sus respectivas observaciones.\n");
	else printf("[ERROR] No se pudo actualizar (Asegurese de que la tutoria este en estado Activa).\n");
	printf("Presione ENTER para continuar..."); getchar();
}

void cancelarTutoriaPorAdministrador() {
	listarTodasLasTutorias();
	int idBuscar;
	printf("\n[ADMIN] Ingrese el ID de la tutoria a CANCELAR (o '0' para regresar): ");
	if (scanf("%d", &idBuscar) != 1 || idBuscar == 0) { while (getchar() != '\n'); return; }
	getchar();
	
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	FILE *temporal = fopen("temp.txt", "w");
	if (!archivo || !temporal) return;
	
	Tutoria t;
	int modificado = 0;
	while (leerSiguienteTutoria(archivo, &t)) {
		if (t.idTutoria == idBuscar) {
			t.estadoCita = 3; // CAMBIA A ESTADO 3 = CANCELADA
			modificado = 1;
		}
		fprintf(temporal, "%d;%s;%s;%s;%s;%d;%s;%s\n", t.idTutoria, t.idEstudiante, t.idDocente, t.fecha, t.hora, t.estadoCita, t.observacionEstudiante, t.observacionDocente);
	}
	fclose(archivo); fclose(temporal);
	remove(RUTA_TUTORIAS); rename("temp.txt", RUTA_TUTORIAS);
	
	if (modificado) printf("[OK] Tutoria Nro %d actualizada al estado CANCELADA.\n", idBuscar);
	else printf("[ERROR] No se encontro la tutoria indicada.\n");
	printf("Presione ENTER para continuar..."); getchar();
}

void listarTodasLasTutorias() {
	FILE *archivo = fopen(RUTA_TUTORIAS, "r");
	if (!archivo) { printf("\nNo se registran tutorias.\n"); return; }
	Tutoria t;
	printf("\n=== REPORTE GENERAL DE TUTORIAS (SISTEMA) ===\n");
	printf("%-4s | %-11s | %-11s | %-10s | %-5s | %-11s | %-20s | %-20s\n",
		   "ID", "Estudiante", "Docente", "Fecha", "Hora", "Estado", "Obs. Estudiante", "Obs. Docente");
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	while (leerSiguienteTutoria(archivo, &t)) {
		char estStr[15];
		if (t.estadoCita == 0) strcpy(estStr, "Pendiente");
		else if (t.estadoCita == 1) strcpy(estStr, "Activa");
		else if (t.estadoCita == 2) strcpy(estStr, "Completada");
		else strcpy(estStr, "Cancelada");
		printf("%-4d | %-11s | %-11s | %-10s | %-5s | %-11s | %-20s | %-20s\n",
			   t.idTutoria, t.idEstudiante, t.idDocente, t.fecha, t.hora, estStr,
			   textoObservacionSegura(t.observacionEstudiante), textoObservacionSegura(t.observacionDocente));
	}
	fclose(archivo);
	printf("\nPresione ENTER para continuar..."); getchar();
}
