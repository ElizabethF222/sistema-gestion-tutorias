#include <stdio.h>
#include <stdlib.h>
#include "comunes.h"
#include "autenticacion.h"
#include "estudiantes.h"
#include "docentes.h"
#include "tutorias.h"
#include "archivos.h"
#include "validaciones.h"
#include "reportes.h"

void menuEstudiante(const char *correo) {
	int op;
	do {
		limpiarPantalla();
		printf("\n==================================================\n");
		printf("               MENU DEL ESTUDIANTE                \n");
		printf("==================================================\n");
		printf("1. Ver mi Perfil Estudiantil\n");
		printf("2. Solicitar Nueva Tutoria (Aprobacion por Admin)\n");
		printf("3. Consultar Estado de Mis Tutorias\n");
		printf("4. Cerrar Sesion\n");
		printf("Seleccione una opcion: ");
		if (scanf("%d", &op) != 1) op = 0;
		{ int c; while ((c = getchar()) != '\n' && c != EOF); } // descarta el resto de la linea (incluido el Enter)
		
		limpiarPantalla();
		
		switch (op) {
		case 1: verPerfilEstudiante(correo); break;
		case 2: solicitarTutoriaEstudiante(correo); break;
		case 3: consultarMisTutoriasEstudiante(correo); break;
		case 4: printf("\nSesion cerrada correctamente.\n"); break;
		default: printf("[ERROR] Opcion invalida.\n"); printf("Presione ENTER para continuar..."); getchar();
		}
	} while (op != 4);
}

void menuDocente(const char *correo) {
	int op;
	do {
		limpiarPantalla();
		printf("\n==================================================\n");
		printf("                 MENU DEL DOCENTE                 \n");
		printf("==================================================\n");
		printf("1. Ver mi Perfil Docente\n");
		printf("2. Consultar Tutorias Asignadas\n");
		printf("3. Registrar Tutorias como Completadas\n");
		printf("4. Cerrar Sesion\n");
		printf("Seleccione una opcion: ");
		if (scanf("%d", &op) != 1) op = 0;
		{ int c; while ((c = getchar()) != '\n' && c != EOF); } // descarta el resto de la linea (incluido el Enter)
		
		limpiarPantalla();
		
		switch (op) {
		case 1: verPerfilDocente(correo); break;
		case 2: consultarTutoriasDocente(correo); break;
		case 3: modificarEstadoTutoriaDocente(correo); break;
		case 4: printf("\nSesion cerrada correctamente.\n"); break;
		default: printf("[ERROR] Opcion invalida.\n"); printf("Presione ENTER para continuar..."); getchar();
		}
	} while (op != 4);
}

void menuAdministrador() {
	int op;
	do {
		limpiarPantalla();
		printf("\n==================================================\n");
		printf("              MENU DE ADMINISTRADOR               \n");
		printf("==================================================\n");
		printf("1. Registrar Ficha de Estudiante\n");
		printf("2. Registrar Ficha de Docente Tutor\n");
		printf("3. Asignar/Aprobar Tutorias Solicitadas\n");
		printf("4. Listar Todos los Estudiantes\n");
		printf("5. Listar Todos los Docentes\n");
		printf("6. Cancelar una Tutoria del Sistema\n");
		printf("7. Visualizar Historial General de Tutorias\n");
		printf("8. Reporte Centralizado de Tutorias (con nombres)\n");
		printf("9. Cantidad de Alumnos Asignados por Horario\n");
		printf("10. Restablecer Base de Datos por Completo\n");
		printf("11. Cerrar Sesion\n");
		printf("Seleccione una opcion: ");
		if (scanf("%d", &op) != 1) op = 0;
		{ int c; while ((c = getchar()) != '\n' && c != EOF); } // descarta el resto de la linea (incluido el Enter)
		
		limpiarPantalla();
		
		switch (op) {
		case 1: registrarEstudiantePorAdmin(); break;
		case 2: registrarDocentePorAdmin(); break;
		case 3: asignarTutoriaAdministrador(); break;
		case 4: listarEstudiantes(); break;
		case 5: listarDocentes(); break;
		case 6: cancelarTutoriaPorAdministrador(); break;
		case 7: listarTodasLasTutorias(); break;
		case 8: mostrarReporteTutorias(); break;
		case 9: mostrarEstadisticaHorarios(); break;
		case 10: restablecerBaseDeDatos(); break;
		case 11: printf("\nSesion de administrador finalizada.\n"); break;
		default: printf("[ERROR] Opcion invalida.\n"); printf("Presione ENTER para continuar..."); getchar();
		}
	} while (op != 11);
}

int main() {
	char correoLogueado[MAX_CADENA];
	int rolAsignado = 0;
	int opFallo = 0;
	
	limpiarPantalla();
	
	while (1) {
		printf("\n==================================================\n");
		printf("       SISTEMA DE GESTION DE TUTORIAS (EPN)       \n");
		printf("==================================================\n");
		
		if (iniciarSesion(correoLogueado, &rolAsignado)) {
			limpiarPantalla();
			printf("\n[OK] Autenticacion Exitosa. Bienvenido: %s\n", correoLogueado);
			
			if (rolAsignado == 1) menuEstudiante(correoLogueado);
			else if (rolAsignado == 2) menuDocente(correoLogueado);
			else if (rolAsignado == 3) menuAdministrador();
			
			limpiarPantalla();
		} else {
			limpiarPantalla();
			printf("\n[ERROR] Las credenciales ingresadas son incorrectas o no existen.\n");
			printf("--------------------------------------------------\n");
			printf("Que desea hacer?\n");
			printf("1. Intentar iniciar sesion de nuevo\n");
			printf("2. Registrarse en el sistema (Estudiante / Docente)\n");
			printf("Seleccione una opcion (1-2): ");
			
			if (scanf("%d", &opFallo) != 1) {
				while (getchar() != '\n');
				opFallo = 1;
			}
			getchar();
			
			if (opFallo == 2) {
				limpiarPantalla();
				autoRegistroPublico();
			}
			limpiarPantalla();
		}
	}
	return 0;
}
