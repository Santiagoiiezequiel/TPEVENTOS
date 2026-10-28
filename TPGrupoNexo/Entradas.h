#ifndef ENTRADAS_H_INCLUDED
#define ENTRADAS_H_INCLUDED
#include "time.h"
#define CONFIRMAR_ENTRADA 0
#define CANTLETRAS 3
#define MOVIMIENTO 5
#define ERROR_MEMORIA -999
#define ERROR_TIEMPO_AGOTADO -888
#define ERROR_COMPRA_NO_ENCONTRADA -777
#define ERROR_TIEMPO_INSUFICIENTE -666

typedef struct{
    char idCompra[10];
    int sector;
    int fila;
    int butaca;
    float precio;
    char estado[10];
}tEntrada;


int comprarEntrada(const char* idUsuario);
int modificarMatriz(tSector *sec,int fila , int columna);
char* crearCodigoCompra(tEvento *evento,int CantEntradas);
void parcearLetras(tEvento *evento,char *destino);
void moverLetra(char *Frase,char* pos,char *abecedario ,size_t desplazamiento);
void* encontrarLetra(char *letra ,char *abecedario);
int mostrarResumenYConfirmar(tEvento *evento, unsigned idSector, int cantEntradas, float precioUnitario, int filasElegidas[], int columnasElegidas[]);
int registrarEntrada(tEvento *evento,tSector *psector,int cantEntradas,int filas[],int columnas[],const char *idUsuario,char *codigoCompra);
int consultarEntrada(const char *idUsuario);
void devolverButacas(tSector *pSector, int filas[], int columnas[], int cantEntradas);
int cancelarEntrada(const char* idUsuario);
#endif // ENTRADAS_H_INCLUDED
