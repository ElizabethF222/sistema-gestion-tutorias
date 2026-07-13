#ifndef COMUNES_H
#define COMUNES_H

#define MAX_CADENA 100
#define MAX_FECHA 11
#define MAX_HORA 6
#define MAX_ID 20

#define RUTA_ESTUDIANTES "estudiantes.txt"
#define RUTA_DOCENTES "docentes.txt"
#define RUTA_TUTORIAS "tutorias.txt"
#define RUTA_USUARIOS "usuarios.txt"

typedef struct {
	char correo[MAX_CADENA];
	char contrasena[MAX_CADENA];
	int tipoRol; // 1 = Estudiante, 2 = Docente, 3 = Administrador
} Usuario;

typedef struct {
	char idEstudiante[MAX_ID];
	char nombreCompleto[MAX_CADENA];
	char carrera[MAX_CADENA];
	int semestre;
	char correoElectronico[MAX_CADENA];
} Estudiante;

typedef struct {
	char idDocente[MAX_ID];
	char nombreCompleto[MAX_CADENA];
	char catedraMateria[MAX_CADENA];
	char estado;
	char correoElectronico[MAX_CADENA];
} Docente;

typedef struct {
	int idTutoria;
	char idEstudiante[MAX_ID];
	char idDocente[MAX_ID];
	char fecha[MAX_FECHA];
	char hora[MAX_HORA];
	int estadoCita; // 0 = Pendiente, 1 = Activa, 2 = Completada, 3 = Cancelada
	char observacionEstudiante[MAX_CADENA];
	char observacionDocente[MAX_CADENA];
} Tutoria;

#endif
