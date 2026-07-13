#ifndef REPORTES_H
#define REPORTES_H

void mostrarReporteTutorias();
void mostrarEstadisticaHorarios();

// Calcula, a partir de tutorias.txt, cuantos alumnos tienen asignado cada
// uno de los 5 bloques fijos institucionales (indices 0..4, en el mismo
// orden que los bloques A-E). Solo cuenta tutorias que ya tienen un
// horario real (estadoCita distinto de Pendiente). 'conteo' debe ser un
// arreglo de al menos 5 enteros, que esta funcion sobrescribe por completo.
void contarAlumnosPorHorario(int conteo[5]);

#endif
