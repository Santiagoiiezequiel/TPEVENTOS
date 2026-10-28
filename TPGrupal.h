#ifndef TP_GRUPAL_H_INCLUDED
#define TP_GRUPAL_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#define COLOR_TITULO    11
#define COLOR_MENU_PRINCIPAL      14
#define COLOR_TEXTO     15
#define MAXF 30
#define MAXC 45
#define IDENTIFICADOR_REPETIDO -111
#define ERROR_ARCHIVO_LECTURA 0
#define ERROR_ARCHIVO_ESCRITURA 0
#define INSERCION_ID 999



int ValidarUsuario(char *idUsuario);
char* validarIDEvento(char *idBuscado);
void leerCadenaSegura(char *destino, int tamMaximo);
void leerCadenaConValidacion(char *destino, int tamMaximo);

#endif // TP_GRUPAL_H_INCLUDED
