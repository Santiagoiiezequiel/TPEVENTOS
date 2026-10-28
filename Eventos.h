#ifndef EVENTOS_H_INCLUDED
#define EVENTOS_H_INCLUDED
#define DISPONIBLE 1
#define CANCELADO -1
#define FINALIZADO 2
#define REPROGRAMADO 3
#define ERROR_EVENTO_NO_ENCONTRADO -99
#define ERROR_CREACION_VEC -77
#define ERROR_DATOS_INVALIDOS -1
#define VECTOR_ERROR_SIN_MEMORIA -1
#include <stdio.h>
#include "Establecimientos.h"
#define LIBRE 1
#define OCUPADO 0
#define TAM 5
#define MINCAP 5

typedef struct{
    char id[10];
    char nombre[30];
    char tipoEvento[30];
    unsigned idEstablecimiento;
    unsigned long long fechaHora;
    int estado;
}tEvento;

typedef struct
{
    void *vec;
    size_t tam;
    size_t ce;
    size_t cap;
}tVector;


void leerCadenaSegura(char *destino, int tamMaximo);
int AltaEvento();
int consultarEventos();
int buscarEvento(const char *idBuscado, tEvento *evento);
int gestionarEvento();
int crearVectorDinamico(tVector *v , size_t tam);
int rankingRecaudacion();
int buscarEvento();
int buscarEventoAdmin();
int reporteOcupacion();
int buscarEventosPorEstablecimientoAdmin();
int contarRegistrosTotales(FILE *pf);
int consultarEventoPorId();
int insertarVector(tVector *v , void *elem,int cmp(const void* , const void*));
int _ampliarVector(tVector *v);
int cmp(const void *a , const void *b);
void mostrarRanking(tVector *v);
int contarEntradasPorCriterio(char *idEvento, const char *estadoBuscado);
int buscarEventoEficiente(const char *idBuscado , tEvento *eventoEncontrado);
float calcularRecaudacionVigente(char *idEvento);
int existeEvento(char *idBuscado);
int consultarVentas();
void imprimirFechaHora(unsigned long long fechaHora);
int contadorMatriz(tSector *sec,int estado);
#endif // EVENTOS_H_INCLUDED
