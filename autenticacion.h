#ifndef AUTENTICACION_H
#define AUTENTICACION_H

int iniciarSesion(char *correoLogueado, int *rolAsignado);
void registrarNuevoUsuarioDirecto(const char *correo, const char *contrasena, int rol);
int verificarExistenciaUsuario(const char *correo);
void autoRegistroPublico();

#endif
