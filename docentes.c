#include <stdio.h>
#include <string.h>
#include "docentes.h"
#include "autenticacion.h"
#include "validaciones.h"
#include "comunes.h"

void seleccionarCatedraEPN(char *destino) {
	int opCatedra = 0;
	do {
		printf("\n--- SELECCIONE SU CATEDRA / MATERIA ---\n");
		printf("1. Programacion Fundamental / Avanzada\n");
		printf("2. Estructuras de Datos y Algoritmos\n");
		printf("3. Sistemas Operativos y Redes\n");
		printf("4. Base de Datos y Sistemas de Informacion\n");
		printf("Seleccione una opcion (1-4): ");
		if (scanf("%d", &opCatedra) != 1) {
			while (getchar() != '\n');
			opCatedra = 0;
		}
		
		switch (opCatedra) {
		case 1: strcpy(destino, "Programacion"); break;
		case 2: strcpy(destino, "Estructuras de Datos"); break;
		case 3: strcpy(destino, "Sistemas Operativos"); break;
		case 4: strcpy(destino, "Bases de Datos"); break;
		}
	} while (opCatedra < 1 || opCatedra > 4);
	getchar();
}

void registrarDocentePorAdmin() {
	char id[20], nombre[MAX_CADENA], catedra[MAX_CADENA], correo[MAX_CADENA], contrasena[MAX_CADENA];
	FILE *archivo;
	
	printf("\n--- REGISTRAR NUEVO DOCENTE (MODO ADMINISTRADOR) ---\n");
	printf("Ingrese el correo institucional (o escriba 'salir' para regresar): ");
	scanf("%s", correo);
	getchar();
	if (strcmp(correo, "salir") == 0) return;
	
	if (verificarExistenciaUsuario(correo)) {
		printf("[ERROR] Este correo ya existe.\n");
		printf("Presione ENTER para regresar..."); getchar();
		return;
	}
	
	printf("Asigne una contrasena: ");
	leerContrasenaOculta(contrasena, MAX_CADENA);
	
	do {
		printf("Ingrese numero de cedula (10 digitos, '0' para salir): ");
		scanf("%s", id);
		getchar();
		if (strcmp(id, "0") == 0) return;
	} while (!validarCedulaFormato(id));
	
	do {
		printf("Ingrese su Nombre Completo: ");
		scanf("%[^\n]", nombre);
		getchar();
	} while (!validarSoloLetras(nombre));
	
	seleccionarCatedraEPN(catedra);
	
	registrarNuevoUsuarioDirecto(correo, contrasena, 2);
	
	archivo = fopen(RUTA_DOCENTES, "a");
	if (!archivo) return;
	fprintf(archivo, "%s;%s;%s;A;%s\n", id, nombre, catedra, correo);
	fclose(archivo);
	
	printf("[OK] Docente Guardado.\n");
	printf("Presione ENTER para continuar..."); getchar();
}

void verPerfilDocente(const char *correo) {
	FILE *archivo = fopen(RUTA_DOCENTES, "r");
	if (!archivo) return;
	Docente doc;
	int encontrado = 0;
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%c;%s\n", doc.idDocente, doc.nombreCompleto, doc.catedraMateria, &doc.estado, doc.correoElectronico) != EOF) {
		if (strcmp(doc.correoElectronico, correo) == 0) {
			printf("\n==================================================\n");
			printf("               MI PERFIL DE DOCENTE               \n");
			printf("==================================================\n");
			printf("Cedula/ID : %s\n", doc.idDocente);
			printf("Nombre    : %s\n", doc.nombreCompleto);
			printf("Catedra   : %s\n", doc.catedraMateria);
			printf("Estado    : %s\n", (doc.estado == 'A') ? "Activo" : "Inactivo");
			encontrado = 1;
			break;
		}
	}
	fclose(archivo);
	if (!encontrado) printf("\n[AVISO] Ficha de docente no completada por el administrador.\n");
	printf("Presione ENTER para regresar..."); getchar();
}

void listarDocentes() {
	FILE *archivo = fopen(RUTA_DOCENTES, "r");
	if (!archivo) { printf("\nNo hay docentes registrados.\n"); return; }
	Docente doc;
	printf("\n%-12s | %-25s | %-25s\n", "ID/Cedula", "Nombre", "Catedra/Materia");
	printf("-----------------------------------------------------------------------\n");
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%c;%s\n", doc.idDocente, doc.nombreCompleto, doc.catedraMateria, &doc.estado, doc.correoElectronico) != EOF) {
		printf("%-12s | %-25s | %-25s\n", doc.idDocente, doc.nombreCompleto, doc.catedraMateria);
	}
	fclose(archivo);
	printf("\nPresione ENTER para continuar..."); getchar();
}
