#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validaciones.h"

#ifdef _WIN32
#include <conio.h> // Captura directa en tiempo real en Windows/ZinJai
#else
#include <termios.h>
#include <unistd.h>

// Implementacion portable de getch() para sistemas tipo Unix/Linux,
// de forma que el programa compile y funcione igual fuera de Windows.
static int getch(void) {
	struct termios oldAttr, newAttr;
	int ch;
	
	tcgetattr(STDIN_FILENO, &oldAttr);
	newAttr = oldAttr;
	newAttr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);
	
	ch = getchar();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr);
	return ch;
}
#endif

int validarCedulaFormato(const char *cedula) {
	if (strlen(cedula) != 10) return 0;
	for (int i = 0; i < 10; i++) {
		if (!isdigit((unsigned char)cedula[i])) return 0;
	}
	return 1;
}

int validarSoloLetras(const char *cadena) {
	if (strlen(cadena) == 0) return 0;
	for (int i = 0; cadena[i] != '\0'; i++) {
		if (!isalpha((unsigned char)cadena[i]) && cadena[i] != ' ') return 0;
	}
	return 1;
}

void limpiarPantalla() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void leerContrasenaOculta(char *destino, int max_longitud) {
	int i = 0;
	int ch;
	while (1) {
		ch = getch();
		if (ch == 13 || ch == 10) { // Enter
			destino[i] = '\0';
			break;
		} else if (ch == 8 || ch == 127) { // Backspace / Delete
			if (i > 0) {
				i--;
				printf("\b \b");
			}
		} else if (i < max_longitud - 1) {
			destino[i++] = (char)ch;
			printf("*");
		}
	}
	fflush(stdout);
	printf("\n");
}

int validarFechaHabil(const char *fecha) {
	int dia, mes, anio;
	
	// La fecha debe venir exactamente como DD/MM/AAAA (10 caracteres)
	if (strlen(fecha) != 10 || fecha[2] != '/' || fecha[5] != '/') return 0;
	if (sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio) != 3) return 0;
	
	if (mes < 1 || mes > 12 || dia < 1 || anio < 1900) return 0;
	
	int esBisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
	int diasPorMes[12] = { 31, esBisiesto ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (dia > diasPorMes[mes - 1]) return 0; // fecha imposible (ej. 31/02)
	
	// Congruencia de Zeller (calendario gregoriano) para obtener el dia
	// de la semana sin depender de funciones de <time.h>.
	int q = dia;
	int m = mes;
	int y = anio;
	if (m < 3) {
		m += 12;
		y -= 1;
	}
	int k = y % 100;
	int j = y / 100;
	int h = (q + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
	// h: 0 = Sabado, 1 = Domingo, 2 = Lunes, 3 = Martes, 4 = Miercoles, 5 = Jueves, 6 = Viernes
	
	if (h == 0 || h == 1) return 0; // cae en fin de semana: no es un dia habil valido
	return 1;
}

const char *textoObservacionSegura(const char *obs) {
	if (obs == NULL || obs[0] == '\0') return "Sin observaciones";
	
	for (int i = 0; obs[i] != '\0'; i++) {
		// Si aparece algun caracter no imprimible, se asume que el registro
		// esta corrupto (por ejemplo, proveniente de una version anterior
		// del archivo con menos campos) y se evita mostrar simbolos raros.
		if (!isprint((unsigned char)obs[i]) && !isspace((unsigned char)obs[i])) {
			return "Sin observaciones";
		}
	}
	return obs;
}
