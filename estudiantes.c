#include <stdio.h>
#include <string.h>
#include "estudiantes.h"
#include "autenticacion.h"
#include "validaciones.h"
#include "comunes.h"

void seleccionarCarreraEPN(char *destino) {
	int opCarrera = 0;
	do {
		printf("\n--- SELECCIONE LA CARRERA (EPN) ---\n");
		printf("1. Ingenieria en Software\n");
		printf("2. Ingenieria en Sistemas de Informacion\n");
		printf("3. Ingenieria en Computacion\n");
		printf("4. Ingenieria en Telecomunicaciones\n");
		printf("Seleccione una opcion (1-4): ");
		if (scanf("%d", &opCarrera) != 1) {
			while (getchar() != '\n');
			opCarrera = 0;
		}
		
		switch (opCarrera) {
		case 1: strcpy(destino, "Software"); break;
		case 2: strcpy(destino, "Sistemas de Informacion"); break;
		case 3: strcpy(destino, "Computacion"); break;
		case 4: strcpy(destino, "Telecomunicaciones"); break;
		}
	} while (opCarrera < 1 || opCarrera > 4);
	getchar(); 
}

int registrarEstudiantePorAdmin() {
	char id[20], nombre[MAX_CADENA], carrera[MAX_CADENA], correo[MAX_CADENA], contrasena[MAX_CADENA];
	int semestre;
	FILE *archivo;
	
	printf("\n--- REGISTRAR NUEVO ESTUDIANTE (MODO ADMINISTRADOR) ---\n");
	printf("Ingrese el correo institucional (o escriba 'salir' para regresar): ");
	scanf("%s", correo);
	getchar();
	if (strcmp(correo, "salir") == 0) return 0;
	
	if (verificarExistenciaUsuario(correo)) {
		printf("[ERROR] Este correo ya se encuentra registrado.\n");
		printf("Presione ENTER para regresar..."); getchar();
		return 0;
	}
	
	printf("Asigne una contrasena: ");
	leerContrasenaOculta(contrasena, MAX_CADENA);
	
	do {
		printf("Ingrese numero de cedula (10 digitos, '0' para salir): ");
		scanf("%s", id);
		getchar();
		if (strcmp(id, "0") == 0) return 0;
	} while (!validarCedulaFormato(id));
	
	do {
		printf("Ingrese Nombre Completo: ");
		scanf("%[^\n]", nombre);
		getchar();
	} while (!validarSoloLetras(nombre));
	
	seleccionarCarreraEPN(carrera);
	
	printf("Ingrese el Semestre actual (1-10): ");
	while (scanf("%d", &semestre) != 1 || semestre < 1 || semestre > 10) {
		printf("[ERROR] Reintente (1-10): ");
		while (getchar() != '\n');
	}
	getchar();
	
	registrarNuevoUsuarioDirecto(correo, contrasena, 1);
	
	archivo = fopen(RUTA_ESTUDIANTES, "a");
	if (!archivo) return 0;
	fprintf(archivo, "%s;%s;%s;%d;%s\n", id, nombre, carrera, semestre, correo);
	fclose(archivo);
	
	printf("[OK] Estudiante guardado con exito.\n");
	printf("Presione ENTER para continuar..."); getchar();
	return 1;
}

void verPerfilEstudiante(const char *correo) {
	FILE *archivo = fopen(RUTA_ESTUDIANTES, "r");
	if (!archivo) return;
	Estudiante est;
	int encontrado = 0;
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%d;%s\n", est.idEstudiante, est.nombreCompleto, est.carrera, &est.semestre, est.correoElectronico) != EOF) {
		if (strcmp(est.correoElectronico, correo) == 0) {
			printf("\n==================================================\n");
			printf("             MI PERFIL DE ESTUDIANTE              \n");
			printf("==================================================\n");
			printf("Cedula/ID : %s\n", est.idEstudiante);
			printf("Nombre    : %s\n", est.nombreCompleto);
			printf("Carrera   : %s\n", est.carrera);
			printf("Semestre  : %d\n", est.semestre);
			printf("Correo    : %s\n", est.correoElectronico);
			encontrado = 1;
			break;
		}
	}
	fclose(archivo);
	if (!encontrado) printf("\n[AVISO] Ficha del estudiante no completada por el administrador.\n");
	printf("Presione ENTER para regresar..."); getchar();
}

void listarEstudiantes() {
	FILE *archivo = fopen(RUTA_ESTUDIANTES, "r");
	if (!archivo) { printf("\nNo existen estudiantes registrados.\n"); return; }
	Estudiante est;
	printf("\n%-12s | %-22s | %-23s | %-5s\n", "Cedula/ID", "Nombre", "Carrera", "Sem.");
	printf("-----------------------------------------------------------------------\n");
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%d;%s\n", est.idEstudiante, est.nombreCompleto, est.carrera, &est.semestre, est.correoElectronico) != EOF) {
		printf("%-12s | %-22s | %-23s | %-5d\n", est.idEstudiante, est.nombreCompleto, est.carrera, est.semestre);
	}
	fclose(archivo);
	printf("\nPresione ENTER para continuar..."); getchar();
}
