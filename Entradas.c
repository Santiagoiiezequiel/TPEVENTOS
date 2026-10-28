#include "Eventos.h"
#include "TPGrupal.h"
#include "Establecimientos.h"
#include "Entradas.h"
#include "Compras.h"


int modificarMatriz(tSector *sec,int fila, int columna)
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
        {
            if(fila== f && columna == c)
            {
                sec->ubicaciones[f][c].disponibilidad= 'X';
            }
            printf(" %c  ", sec->ubicaciones[f][c].disponibilidad == DISPONIBLE ? 'L' : 'X');
        }

    }
    printf("\n");

    return TODO_OK;
}

int consultarEntrada(const char *idUsuario)
{
    //FILE *pfEntradas = NULL;
    //tEntrada entradaActual;
    FILE *pfCompras = NULL;
    tEstablecimiento *estActual = NULL;
    tEvento eventoActual;
    tCompra compraActual;
    int encontradas = 0;
    int i=0;

    /*
    pfEntradas = fopen("Entradas.bin", "rb");
    if (pfEntradas == NULL)
    {
        printf("\nNo se pudo abrir el archivo de Entradas o aún no hay compras registradas.\n");
        return ERROR_ARCHIVO_LECTURA;
    }
    */
    pfCompras =  fopen("Compras.bin","rb");
    if(!pfCompras)
    {
        printf("\nNo se pudo abrir el archivo de Compras o aún no hay compras registradas.\n");
        return ERROR_ARCHIVO_LECTURA;
    }

    estActual = (tEstablecimiento*) malloc(sizeof(tEstablecimiento));
    if (estActual == NULL)
    {
        return ERROR_MEMORIA;
    }

    pfCompras = fopen("Compras.bin", "rb");
    if (pfCompras == NULL)
    {
        printf("\nNo se pudo abrir el archivo de Compras.\n");
        free(estActual);
        return ERROR_ARCHIVO_LECTURA;
    }

    printf("\n==================================================\n");
    printf("        CONSULTA DE ENTRADAS PARA: %s             \n", idUsuario);
    printf("==================================================\n");

    while (fread(&compraActual, sizeof(tCompra), 1, pfCompras) == 1)
    {

        if (strcmp(compraActual.idUsuario, idUsuario) == 0)
        {
            if (buscarEvento(compraActual.idEvento, &eventoActual) == TODO_OK)
            {
                buscarEstablecimiento(eventoActual.idEstablecimiento, estActual);
            }

            for (i = 0; i < compraActual.cantEntradas; i++)
            {
                encontradas++;
                printf("\n--- Entrada #%d ---\n", encontradas);
                printf("Código de Compra    : %s\n", compraActual.id);
                printf("Evento              : %s\n", eventoActual.nombre);
                printf("Establecimiento     : %s\n", estActual->nombre);
                printf("Sector              : %d\n", compraActual.Entradas[i].sector);
                printf("Fila                : %d\n", compraActual.Entradas[i].fila + 1);
                printf("Butaca              : %d\n", compraActual.Entradas[i].butaca + 1);
                printf("Precio unitario     : $%.2f\n", compraActual.Entradas[i].precio);
                printf("Estado del Evento: %s\n", (eventoActual.estado == DISPONIBLE) ? "DISPONIBLE" : (eventoActual.estado == FINALIZADO) ? "FINALIZADO" : (eventoActual.estado == CANCELADO) ? "CANCELADO" : "DESCONOCIDO");
            }
        }
    }



    if (encontradas == 0)
    {
        printf("\nNo se encontraron entradas registradas para este usuario.\n");
    }
    printf("========================================\n");

    return TODO_OK;
}

int cancelarEntrada(const char* idUsuario)
{
    tCompra compraActual;
    tEvento eventoBuscado;
    char idCompra[10];
    int encontrado=0;
    int i;
    double segundosFaltantes=0;
    time_t tiempo_actual = time(NULL);
    FILE *pfCompras = fopen("Compras.bin","r+b");
    if(!pfCompras)
    {
        printf("\nNo se pudo abrir el archivo de lectura Entrada");
        return ERROR_ARCHIVO_LECTURA;
    }

    //int idSectorBuscado = compraActual.Entradas[i].sector;
    consultarEntrada(idUsuario);
    printf("\nIngrese el codigo de compra para cancelarla");

    scanf("%9s", idCompra);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);


    while (fread(&compraActual, sizeof(tCompra), 1, pfCompras) == 1)
    {


        if (strcmp(compraActual.id, idCompra) == 0 && strcmp(compraActual.idUsuario, idUsuario) == 0)
        {
            encontrado=1;
            if (buscarEvento(compraActual.idEvento, &eventoBuscado) != TODO_OK)
            {
                printf("\n[ERROR] No se pudo encontrar el evento asociado a esta compra.\n");
                fclose(pfCompras);
                return ERROR_EVENTO_NO_ENCONTRADO;
            }

            segundosFaltantes = difftime(eventoBuscado.fechaHora, tiempo_actual);
            if (segundosFaltantes < 259200)
            {
                printf("\n[ERROR] No se puede cancelar. Deben faltar más de 72 horas para el evento.\n");
                fclose(pfCompras);
                return ERROR_TIEMPO_INSUFICIENTE;
            }


            for (i = 0; i < compraActual.cantEntradas; i++)
            {
                strcpy(compraActual.Entradas[i].estado, "Cancelado");
            }
            fseek(pfCompras, -(long)sizeof(tCompra), SEEK_CUR);
            fwrite(&compraActual,sizeof(tCompra),1,pfCompras);
            printf("\n¡La compra con código %s ha sido cancelada exitosamente!\n", idCompra);
            break;
        }
    }


    fclose(pfCompras);

    if (!encontrado)
    {
        printf("\nNo se encontró ninguna compra activa con ese código para el usuario: %s\n", idUsuario);
        return ERROR_COMPRA_NO_ENCONTRADA;
    }

    printf("==================================================\n");
    return TODO_OK;
}


int comprarEntrada(const char* idUsuario)
{
    tEvento evento;
    tEstablecimiento *est = NULL;
    tSector *pSectores = NULL;
    double segundos_transcurridos = 0;
    unsigned idSector;
    char idEvento[10];
    int c;
    int Opcion = 1, fila = 0, columna = 0, tiempoNULL = 0, cantEntradas = 0, confirmacion = 0,k=0;
    int leidoSector = 0,leidoFila=0,leidoColumna=0,leidoOpcion = 0;;
    char codigoCompra[10];
    time_t tiempo_inicio;
    time_t tiempo_actual;

    int filasElegidas[MAX_ENTRADAS_POR_COMPRA];
    int columnasElegidas[MAX_ENTRADAS_POR_COMPRA];

    est = (tEstablecimiento*) malloc(sizeof(tEstablecimiento));
    if (est == NULL)
    {
        return ERROR_MEMORIA;
    }

    consultarEventos();

    printf("\nIngrese el ID del evento deseado: ");
    leerCadenaConValidacion(idEvento, sizeof(idEvento));

    if (buscarEvento(idEvento, &evento) != TODO_OK)
    {
        printf("\nEvento inexistente.\n");
        free(est);
        return ERROR_EVENTO_NO_ENCONTRADO;
    }

    tiempo_inicio = time(NULL);
    printf("Ingresaste al Evento deseado, comienza el temporizador de 5 minutos\n");

    if (buscarEstablecimiento(evento.idEstablecimiento, est) != TODO_OK)
    {
        printf("\nNo se encontró el establecimiento del evento.\n");
        free(est);
        return ERROR_ESTABLECIMIENTO_NO_ENCONTRADO;
    }

    mostrarEstablecimiento(est);


    do {
        printf("\nIngrese el ID del sector deseado: ");
        leidoSector = scanf("%u", &idSector);


        while ((c = getchar()) != '\n' && c != EOF);

        if (leidoSector != 1) {
            printf("[ERROR] Ingrese un número válido para el ID del sector.\n");
        }
    } while (leidoSector != 1);

    pSectores = buscarSector(est, idSector);
    if (!pSectores)
    {
        printf("\nSector inexistente.\n");
        free(est);
        return ERROR_SECTOR_NO_ENCONTRADO;
    }
    actualizarMatrizConVentas(pSectores, idEvento);
    while (Opcion == 1 && segundos_transcurridos <= 300)
    {
        tiempo_actual = time(NULL);
        segundos_transcurridos = difftime(tiempo_actual, tiempo_inicio);

        if (segundos_transcurridos >= 300)
        {
            printf("\n¡Tiempo agotado! Han transcurrido más de 5 minutos.\n");
            tiempoNULL = 1;
            break;
        }

        if (cantEntradas >= MAX_ENTRADAS_POR_COMPRA)
        {
            printf("\nHa alcanzado el límite máximo de entradas por compra.\n");
            break;
        }

        mostrarMatriz(pSectores);
        printf("\nIngrese butaca deseada:\n");



        do {
            printf("FILA: ");
            leidoFila = scanf("%d", &fila);


            while ((c = getchar()) != '\n' && c != EOF);

            if (leidoFila != 1) {
                printf("[ERROR] Ingrese un número válido para la fila.\n");
            }
        } while (leidoFila != 1);


        do {
            printf("COLUMNA: ");
            leidoColumna = scanf("%d", &columna);

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            if (leidoColumna != 1) {
                printf("[ERROR] Ingrese un número válido para la columna.\n");
            }
        } while (leidoColumna != 1);


        if (fila - 1 < 0 || fila - 1 >= pSectores->filas || columna - 1 < 0 || columna - 1 >= pSectores->columnas)
        {
            printf("\n[ERROR] La fila o columna ingresada está fuera de rango.\n");
            continue;
        }


        if (pSectores->ubicaciones[fila - 1][columna - 1].disponibilidad == 'X')
        {
            printf("\n[ERROR] ¡Esa butaca ya está ocupada! Elija otra.\n");
            continue;
        }

        *(filasElegidas + cantEntradas) = fila - 1;
        *(columnasElegidas + cantEntradas) = columna - 1;
        modificarMatriz(pSectores, fila - 1, columna - 1);
        cantEntradas++;

        printf("Estás a tiempo. Segundos transcurridos: %.0f / 300\n", segundos_transcurridos);

        do {
            printf("\n¿Desea elegir otra butaca?\n");
            printf("Presione 1 para SEGUIR ELIGIENDO.\n");
            printf("Presione 0 para TERMINAR la selección y ver el resumen: ");
            leidoOpcion = scanf("%d", &Opcion);

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            if (leidoOpcion != 1) {
                printf("[ERROR] Opción inválida.\n");
            }
        } while (leidoOpcion != 1);
    }

    if (tiempoNULL == 1 || Opcion != 0 || cantEntradas == 0)
    {
        printf("\nCompra cancelada por el usuario o tiempo agotado.\n");
        for (k = 0; k < cantEntradas; k++)
        {
            pSectores->ubicaciones[filasElegidas[k]][columnasElegidas[k]].disponibilidad = LIBRE; // o 1
        }

        free(est);
        return ERROR_TIEMPO_AGOTADO;
    }

    confirmacion = mostrarResumenYConfirmar(&evento, idSector, cantEntradas, pSectores->precio, filasElegidas, columnasElegidas);
    if (confirmacion != 0)
    {
        printf("\nCompra cancelada por el usuario en el resumen final.\n");
        for (k = 0; k < cantEntradas; k++)
        {
            pSectores->ubicaciones[filasElegidas[k]][columnasElegidas[k]].disponibilidad = LIBRE; // o 1
        }
        free(est);
        return ERROR_COMPRA_NO_ENCONTRADA; // O el código de error que manejes
    }

    strcpy(codigoCompra, crearCodigoCompra(&evento, cantEntradas));
    registrarEntrada(&evento, pSectores, cantEntradas, filasElegidas, columnasElegidas, idUsuario, codigoCompra);

    free(est);
    return TODO_OK;
}

int mostrarResumenYConfirmar(tEvento *evento, unsigned idSector, int cantEntradas, float precioUnitario, int filasElegidas[], int columnasElegidas[])
{

    int i = 0;
    int opcion = 0;
    float importeTotal = 0.0f;

    importeTotal = cantEntradas * precioUnitario;

    printf("\n========================================\n");
    printf("          RESUMEN DE LA COMPRA          \n");
    printf("========================================\n");
    printf("Evento ID: %s\n", evento->id);
    printf("Sector: %u\n", idSector);
    printf("Cantidad de entradas: %d\n", cantEntradas);
    printf("Precio unitario: $%.2f\n", precioUnitario);
    printf("Importe total: $%.2f\n", importeTotal);
    printf("Ubicaciones seleccionadas:\n");


    for (i = 0; i < cantEntradas; i++)
    {
        printf(" - Fila: %d, Butaca: %d\n", *(filasElegidas + i) + 1, *(columnasElegidas + i) + 1);
    }
    printf("========================================\n");

    printf("\n¿Confirma la compra definitivamente? (Presione 0 para confirmar, cualquier otro número para cancelar): ");
    scanf("%d", &opcion);

    return opcion;
}


int registrarEntrada(tEvento *evento,tSector *psector,int cantEntradas,int filas[],int columnas[],const char *idUsuario,char *codigoCompra)
{
    tCompra compra;
    memset(&compra, 0, sizeof(tCompra));
    int i=0;
    FILE *pfEntradas = fopen("Entradas.bin", "ab");
    if (pfEntradas == NULL)
    {
        printf("\nNo se pudo crear el archivo de Entradas");
        return ERROR_ARCHIVO_ESCRITURA;
    }

    FILE *pfCompras = fopen("Compras.bin", "ab");
    if (pfCompras == NULL)
    {
        return ERROR_ARCHIVO_ESCRITURA;
    }

    strcpy(compra.id,codigoCompra);
    strcpy(compra.idUsuario,idUsuario);
    strcpy(compra.idEvento,evento->id);
    compra.cantEntradas = cantEntradas;
    compra.importeTotal +=  cantEntradas * psector->precio;
    compra.Entradas->precio= psector->precio;

    for(i=0; i<cantEntradas; i++)
    {
        strcpy(compra.Entradas[i].idCompra, codigoCompra);
        compra.Entradas[i].sector = psector->id;

        compra.Entradas[i].fila = *(filas + i);
        compra.Entradas[i].butaca = *(columnas + i);

        compra.Entradas[i].precio = psector->precio;
        strcpy(compra.Entradas[i].estado, "Vendido");

    }


    fwrite(&compra, sizeof(tCompra), 1, pfCompras);
    fclose(pfCompras);


    for (i = 0; i < cantEntradas; i++)
    {
        fwrite(&(compra.Entradas[i]), sizeof(tEntrada), 1, pfEntradas);
    }
    fclose(pfEntradas);

    return TODO_OK;
}


char* crearCodigoCompra(tEvento *evento,int CantEntradas)
{
    unsigned long long fecha = evento->fechaHora;
    //int anio = fecha / 100000000ULL;
    int mes = (fecha / 1000000ULL) % 100;
    int dia = (fecha / 10000ULL) % 100;
    static char codigoFinal[20];
    int idEventoNum = atoi(evento->id);
    int sumaDigitos = dia + mes + idEventoNum;
    int parte1 = sumaDigitos%1000;
    char letras[4];
    parcearLetras(evento,letras);
    sprintf(codigoFinal, "%03d%s%03d", parte1, letras, CantEntradas);
    return codigoFinal;
}

void parcearLetras(tEvento *evento,char *destino)
{
    char* frase= destino;
    //char* pfrase = frase;
    char *letra= evento->nombre;
    char *pos;
    char abecedario[]="abcdefghijklmnopqrstuvwxyz";
    int i=0;

    while(i<CANTLETRAS && *letra!='\0')
    {

        pos=encontrarLetra(letra,abecedario);
        moverLetra(frase,pos,abecedario,MOVIMIENTO);
        frase++;
        letra++;
        i++;
    }
    *frase = '\0';
}


void* encontrarLetra(char *letra,char *abecedario)
{

    while(*abecedario!='\0')
    {
        if(tolower(*letra) == *abecedario)
        {
            return abecedario;
        }
        abecedario++;
    }

    return NULL;

}

void moverLetra(char *Frase,char* pos,char *abecedario,size_t desplazamiento)
{
    int cantLetras = strlen(abecedario);
    int posicion = pos- abecedario;

    abecedario +=  (posicion + desplazamiento)%cantLetras;
    *Frase = *abecedario;

}
