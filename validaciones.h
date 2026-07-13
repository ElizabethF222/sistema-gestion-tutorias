#ifndef VALIDACIONES_H
#define VALIDACIONES_H

int validarCedulaFormato(const char *cedula);
int validarSoloLetras(const char *cadena);
void limpiarPantalla();
void leerContrasenaOculta(char *destino, int max_longitud);

// Devuelve el texto tal cual si es una observacion valida e imprimible;
// si esta vacia o contiene datos corruptos/no imprimibles (por ejemplo,
// registros heredados de una version anterior del archivo), devuelve
// "Sin observaciones" en su lugar.
const char *textoObservacionSegura(const char *obs);

// Valida que 'fecha' venga en formato DD/MM/AAAA y que corresponda a un
// dia habil (Lunes a Viernes). Devuelve 1 si es una fecha valida y habil,
// 0 si el formato es incorrecto o si cae en Sabado/Domingo.
int validarFechaHabil(const char *fecha);

#endif
