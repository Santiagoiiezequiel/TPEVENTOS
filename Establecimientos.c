#include "Establecimientos.h"
#include "Eventos.h"
#include "TPGrupal.h"
#include "Compras.h"
#define LIBRE 1
#define OCUPADO 0


tSector *buscarSector(tEstablecimiento *est, unsigned idSector)
{
    tSector *p = est->sectores;
    tSector *fin = est->sectores + MAX_SECTORES;

    while (p < fin)
    {
        if (p->id != 0 && p->id == idSector)
            return p;
        p++;
    }
    return NULL;
}

void actualizarMatrizConVentas(tSector *pSector, const char *idEventoBuscado)
{
    FILE *pfCompras = NULL;
    tCompra compraActual;
    int i;
    int fila=0;
    int butaca=0;
    pfCompras = fopen("Compras.bin", "rb");
    if (pfCompras == NULL)
    {
        printf("\nError en la apertura del archivo lectura");
        return;
    }


    while (fread(&compraActual, sizeof(tCompra), 1, pfCompras) == 1)
    {

        if (strcmp(compraActual.idEvento, idEventoBuscado) == 0)
        {
            for ( i = 0; i < compraActual.cantEntradas; i++)
            {

                if (strcmp(compraActual.Entradas[i].estado, "Cancelado") != 0 && compraActual.Entradas[i].sector == pSector->id)
                {
                    fila = compraActual.Entradas[i].fila;
                    butaca = compraActual.Entradas[i].butaca;
                    pSector->ubicaciones[fila][butaca].disponibilidad = OCUPADO;
                }
            }
        }
    }

    fclose(pfCompras);
}

int consultarUbiyCapacidad()
{
    tEstablecimiento est;
    tSector *pSectores;
    tEvento evento;
    char idEvento[10];
    unsigned idSector;

    printf("\nID del evento: ");
    scanf("%9s", idEvento);

    if (buscarEvento(idEvento, &evento) != TODO_OK)
    {
        printf("\nEvento inexistente.\n");
        return ERROR_EVENTO_NO_ENCONTRADO;
    }

    if (buscarEstablecimiento(evento.idEstablecimiento, &est) != TODO_OK)
    {
        printf("\nNo se encontro el establecimiento.\n");
        return ERROR_ESTABLECIMIENTO_NO_ENCONTRADO;
    }

    printf("\nID del sector: ");
    scanf("%u", &idSector);

    pSectores = buscarSector(&est, idSector);
    if (!pSectores)
    {
        printf("\nSector inexistente.\n");
        return ERROR_SECTOR_NO_ENCONTRADO;
    }

    actualizarMatrizConVentas(pSectores, idEvento);
    mostrarMatriz(pSectores);
    return TODO_OK;
}

void mostrarMatriz(const tSector *sec)
{
    int f, c;

    printf("\nSector: %s | Precio: %.2f", sec->nombre, sec->precio);
    printf("\n[L] = libre   [X] = ocupada\n\n    ");

    for (c = 0; c < sec->columnas; c++)
        printf("%3d ", c + 1);

    for (f = 0; f < sec->filas; f++)
    {
        printf("\nF%-2d ", f + 1);
        for (c = 0; c < sec->columnas; c++)
            printf(" %c  ", sec->ubicaciones[f][c].disponibilidad == DISPONIBLE ? 'L' : 'X');
    }
    printf("\n");
}

int buscarEstablecimiento(unsigned id, tEstablecimiento *est)
{
    FILE *pf = fopen("Establecimiento.bin", "rb");
    if (!pf)
        return ERROR_APERTURA_ARCHIVO;

    while (fread(est, sizeof(tEstablecimiento), 1, pf) == 1)
    {
        if (est->id == id)
        {
            fclose(pf);
            return TODO_OK;
        }
    }
    fclose(pf);
    return ERROR_ESTABLECIMIENTO_NO_ENCONTRADO;
}

int consultarEstablecimientos()
{
    tEvento evento;
    tEstablecimiento est;
    char idEvento[10];

    printf("\nID del evento: ");
    scanf("%9s", idEvento);

    if (buscarEvento(idEvento, &evento) != TODO_OK)
    {
        printf("\nEvento inexistente.\n");
        return ERROR_EVENTO_NO_ENCONTRADO;
    }

    if (buscarEstablecimiento(evento.idEstablecimiento, &est) != TODO_OK)
    {
        printf("\nNo se encontro el establecimiento del evento.\n");
        return ERROR_ESTABLECIMIENTO_NO_ENCONTRADO;
    }

    mostrarEstablecimiento(&est);

    return TODO_OK;
}


void mostrarEstablecimiento(const tEstablecimiento *est)
{
    int i;

    printf("\n==================================================");
    printf("\nID: %u", est->id);
    printf("\nNOMBRE: %s", est->nombre);
    printf("\nDIRECCION: %s", est->direccion);
    printf("\nCAPACIDAD: %u", est->capacidadTotal);
    printf("\n------------- SECTORES -------------");

    for (i = 0; i < MAX_SECTORES; i++)
    {
        if (est->sectores[i].id != 0)
            printf("\nID: %u | %s | CAP: %u | PRECIO: %.2f",
                   est->sectores[i].id, est->sectores[i].nombre,
                   est->sectores[i].capacidad, est->sectores[i].precio);
    }
    printf("\n==================================================\n");
}

void inicializarSectorDinamico(tSector* sector)
{
    int f=0,c=0;
    sector->ubicaciones= (tUbicacion**)malloc(sector->filas*sizeof(tUbicacion*));

    if (sector->ubicaciones == NULL) {
        printf("Error de memoria en filas!\n");
        return;
    }

    for( f = 0; f < sector->filas; f++)
    {

        sector->ubicaciones[f] = (tUbicacion*)malloc(sector->columnas*sizeof(tUbicacion));
        if (sector->ubicaciones[f] == NULL) {
            printf("Error de memoria en columnas!\n");
            return;
        }
        for( c = 0; c < sector->columnas; c++)
        {

            sector->ubicaciones[f][c].disponibilidad = LIBRE;
        }
    }
}

void liberarSectores(tSector* sector)
{

    int f;

    for(f=0; f<sector->filas; f++)
    {
        free(sector->ubicaciones[f]);
    }

    free(sector->ubicaciones);
    sector->ubicaciones=NULL;
    printf("\nMatriz liberado correctamente!");
}

/**
typedef struct{
    unsigned id;
    char nombre[15];
    unsigned capacidad;
    float precio;
    int filas;
    int columnas;
    tUbicacion ubicaciones[MAXF][MAXC];
}tSector;
{
    unsigned id;
    char nombre[30];
    char direccion[30];
    unsigned capacidadTotal;
    tSector sectores[MAX_SECTORES];
} tEstablecimiento;
**/

int crearArchEstablecimiento()
{
    int i,j;
    tEstablecimiento auxest[]=
    {
        {
            1111,
            "Estadio Monumental",
            "Av.José Figueroa Alcorta 7597",
            85018,
            {
                {1, "Platea Baja", 15000, 45000.0, 10, 20} // Solo defines los datos base del sector
            }
        },
        {
            2222,
            "Estadio Velez Sarfield",
            "Av.Juan.B Justo 920",
            49540,
            {
                {1, "Platea Baja", 15000, 45000.0, 10, 20},
                {2, "Popular", 45018, 15000.0, 15, 30}
            }
            // Solo defines los datos base del sector
        },

        {
            3333,
            "Luna Park",
            "Av. Corrientes 990",
            10000,
            {
                {1, "Campo", 5000, 25000.0, 20, 25},        // Sector 0
                {2, "Pullman", 3000, 35000.0, 10, 10}
            }
            // Sector 1
        }
    };
//tEstablecimiento *auxest=(tEstablecimiento*) malloc(sizeof(tEstablecimiento));
    int cantEstablecimientos = 3;

    for(i = 0; i < cantEstablecimientos; i++)
    {

        for(j = 0; j < MAX_SECTORES; j++)
        {
            if(auxest[i].sectores[j].id != 0) // Si el sector existe
            {
                inicializarSectorDinamico(&auxest[i].sectores[j]);
            }
        }
    }


    FILE *pf= fopen("Establecimiento.bin","wb");
    if(!pf)
    {
        printf("\nError en la creacion del archivo establecimiento");
        return ERROR_APERTURA_ARCHIVO;
    }
    fwrite(auxest,sizeof(tEstablecimiento),cantEstablecimientos,pf);


    fclose(pf);
    return TODO_OK;
}


int liberarMemoriaSectores()
{
    int cantEstablecimientos=3;
    tEstablecimiento *auxest = (tEstablecimiento*)malloc(cantEstablecimientos * sizeof(tEstablecimiento));
    if (auxest == NULL)
    {
        printf("Error de asignación de memoria\n");
        return 0;
    }
    int i,j;
    FILE *pf= fopen("Establecimiento.bin","rb");
    if(!pf)
    {
        printf("\nError en la creacion del archivo establecimiento");
        free(auxest);
        return ERROR_APERTURA_ARCHIVO;
    }
    fread(auxest,sizeof(tEstablecimiento),cantEstablecimientos,pf);

    for(i = 0; i < cantEstablecimientos; i++)
    {

        for(j = 0; j < MAX_SECTORES; j++)
        {
            if(auxest[i].sectores[j].id != 0) // Si el sector existe
            {
                liberarSectores(&auxest[i].sectores[j]);
            }
        }
    }


    fclose(pf);
    return TODO_OK;
}
