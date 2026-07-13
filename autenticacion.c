#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autenticacion.h"
#include "estudiantes.h"
#include "docentes.h"
#include "comunes.h"
#include "validaciones.h"

int verificarExistenciaUsuario(const char *correo) {
	if (strcmp(correo, "admin@epn.edu.ec") == 0) return 1;
	
	FILE *archivo = fopen(RUTA_USUARIOS, "r");
	if (!archivo) return 0;
	
	Usuario user;
	while (fscanf(archivo, "%[^;];%[^;];%d\n", user.correo, user.contrasena, &user.tipoRol) != EOF) {
		if (strcmp(user.correo, correo) == 0) {
			fclose(archivo);
			return 1;
		}
	}
	fclose(archivo);
	return 0;
}

void registrarNuevoUsuarioDirecto(const char *correo, const char *contrasena, int rol) {
	FILE *archivo = fopen(RUTA_USUARIOS, "a");
	if (!archivo) return;
	fprintf(archivo, "%s;%s;%d\n", correo, contrasena, rol);
	fclose(archivo);
}

void autoRegistroPublico() {
	char correo[MAX_CADENA], contrasena[MAX_CADENA];
	char id[20], nombre[MAX_CADENA], variableAux[MAX_CADENA];
	int opcionRol = 0, semestre = 0;
	FILE *fFicha;
	
	printf("\n==================================================\n");
	printf("            REGISTRO DE NUEVA CUENTA              \n");
	printf("==================================================\n");
	printf("0. Cancelar y regresar\n");
	printf("1. Estudiante\n");
	printf("2. Docente\n");
	printf("Seleccione su Rol (0-2): ");
	if (scanf("%d", &opcionRol) != 1 || opcionRol == 0) {
		while (getchar() != '\n');
		return;
	}
	getchar();
	
	printf("Ingrese su Correo Institucional: ");
	scanf("%s", correo);
	getchar();
	
	if (verificarExistenciaUsuario(correo)) {
		printf("[ERROR] Este correo ya se encuentra registrado en la institucion.\n");
		printf("Presione ENTER para continuar..."); getchar();
		return;
	}
	
	printf("Cree su contrasena de seguridad: ");
	leerContrasenaOculta(contrasena, MAX_CADENA);
	
	// --- CAPTURA DE DATOS OBLIGATORIOS SEGÚN EL ROL ---
	do {
		printf("Ingrese su numero de cedula (10 digitos, '0' para cancelar): ");
		scanf("%s", id);
		getchar();
		if (strcmp(id, "0") == 0) return;
	} while (!validarCedulaFormato(id));
	
	do {
		printf("Ingrese sus Nombres y Apellidos Completos: ");
		scanf("%[^\n]", nombre);
		getchar();
	} while (!validarSoloLetras(nombre));
	
	if (opcionRol == 1) { // Datos específicos del Estudiante
		seleccionarCarreraEPN(variableAux);
		printf("Ingrese su Semestre actual (1-10): ");
		while (scanf("%d", &semestre) != 1 || semestre < 1 || semestre > 10) {
			printf("[ERROR] Ingrese un numero del 1 al 10: ");
			while (getchar() != '\n');
		}
		getchar();
		
		// Guardar credenciales y luego la ficha académica
		registrarNuevoUsuarioDirecto(correo, contrasena, 1);
		fFicha = fopen(RUTA_ESTUDIANTES, "a");
		if (fFicha) {
			fprintf(fFicha, "%s;%s;%s;%d;%s\n", id, nombre, variableAux, semestre, correo);
			fclose(fFicha);
		}
	} else { // Datos específicos del Docente
		seleccionarCatedraEPN(variableAux);
		
		// Guardar credenciales y luego la ficha técnica del docente
		registrarNuevoUsuarioDirecto(correo, contrasena, 2);
		fFicha = fopen(RUTA_DOCENTES, "a");
		if (fFicha) {
			fprintf(fFicha, "%s;%s;%s;A;%s\n", id, nombre, variableAux, correo);
			fclose(fFicha);
		}
	}
	
	printf("[OK] Cuenta y Ficha Academica creadas con exito. Proceda al Login.\n");
	printf("Presione ENTER para continuar..."); getchar();
}

int iniciarSesion(char *correoLogueado, int *rolAsignado) {
	char correoInput[MAX_CADENA];
	char claveInput[MAX_CADENA];
	
	printf("\n==================================================\n");
	printf("         INICIO DE SESION / PORTAL ACADEMICO      \n");
	printf("==================================================\n");
	printf("(Escriba 'salir' para cerrar el programa)\n\n");
	printf("Correo Electronico: ");
	scanf(" %s", correoInput);
	getchar();
	
	if (strcmp(correoInput, "salir") == 0) {
		exit(0);
	}
	
	printf("Contrasena: ");
	leerContrasenaOculta(claveInput, MAX_CADENA);
	
	if (strcmp(correoInput, "admin@epn.edu.ec") == 0 && strcmp(claveInput, "admin1234") == 0) {
		strcpy(correoLogueado, correoInput);
		*rolAsignado = 3; 
		return 1;
	}
	
	FILE *archivo = fopen(RUTA_USUARIOS, "r");
	if (!archivo) return 0;
	
	Usuario user;
	while (fscanf(archivo, "%[^;];%[^;];%d\n", user.correo, user.contrasena, &user.tipoRol) != EOF) {
		if (strcmp(user.correo, correoInput) == 0 && strcmp(user.contrasena, claveInput) == 0) {
			strcpy(correoLogueado, user.correo);
			*rolAsignado = user.tipoRol;
			fclose(archivo);
			return 1; 
		}
	}
	fclose(archivo);
	return 0;
}
