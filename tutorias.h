#ifndef TUTORIAS_H
#define TUTORIAS_H

void solicitarTutoriaEstudiante(const char *correoEstudiante); 
void asignarTutoriaAdministrador();                             
void consultarMisTutoriasEstudiante(const char *correoEstudiante);
void consultarTutoriasDocente(const char *correoDocente);
void modificarEstadoTutoriaDocente(const char *correoDocente);
void cancelarTutoriaPorAdministrador();
void listarTodasLasTutorias();

#endif
