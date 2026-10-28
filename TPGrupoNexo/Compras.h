#ifndef COMPRAS_H_INCLUDED
#define COMPRAS_H_INCLUDED
#define MAX_ENTRADAS_POR_COMPRA 5
#include "time.h"
#include "Entradas.h"

typedef struct{
    char id[10];
    char idUsuario[9];
    char idEvento[10];
    tEntrada Entradas[MAX_ENTRADAS_POR_COMPRA];
    int cantEntradas;
    float importeTotal;
    unsigned long long fechaCompra;
}tCompra;


#endif // COMPRAS_H_INCLUDED
