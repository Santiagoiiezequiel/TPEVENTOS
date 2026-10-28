#ifndef ESTABLECIMIENTOS_H_INCLUDED
#define ESTABLECIMIENTOS_H_INCLUDED
#define MAX_SECTORES 15
#include "TPGrupal.h"
#define ERROR_APERTURA_ARCHIVO -1
#define ERROR_SECTOR_NO_ENCONTRADO -2
#define TODO_OK 0
#define ERROR_ESTABLECIMIENTO_NO_ENCONTRADO -99
#define LIBRE 1
#define OCUPADO 0

typedef struct{
    int fila;
    int columna;
    int disponibilidad;
}tUbicacion;


typedef struct{
    unsigned id;
    char nombre[15];
    unsigned capacidad;
    float precio;
    int filas;
    int columnas;
    tUbicacion **ubicaciones;

}tSector;

typedef struct{
    unsigned id;
    char nombre[30];
    char direccion[30];
    unsigned capacidadTotal;
    tSector sectores[MAX_SECTORES];
}tEstablecimiento;


tSector *buscarSector(tEstablecimiento *est, unsigned idSector);
int crearArchEstablecimiento();
void inicializarSectorDinamico(tSector* sector);
void liberarSectores(tSector* sector);
int liberarMemoriaSectores();
int consultarEstablecimientos();
int consultarUbiyCapacidad();
int buscarEstablecimiento(unsigned id, tEstablecimiento *est);
void mostrarEstablecimiento(const tEstablecimiento *est);
void mostrarMatriz(const tSector *sec);
void actualizarMatrizConVentas(tSector *pSector, const char *idEventoBuscado);

#endif // ESTABLECIMIENTOS_H_INCLUDED
