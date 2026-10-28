#include "Eventos.h"
#include "TPGrupal.h"
#include "Establecimientos.h"
#include "Compras.h"
/**
|||||||||||||||||ADMINISTRADOR||||||||||||||||||
**/



int AltaEvento()
{
    FILE *pfescritura = NULL;
    FILE *pflectura = NULL;

    tEvento nuevoEvento = {0};
    tEvento eventoActual = {0};
    int idRepetido = 0;
    int idInsertado = 0;
    int c;

    while ((c = getchar()) != '\n' && c != EOF);


    printf("=== ALTA DE NUEVO EVENTO ===\n");
    printf("\nIngrese ID del evento (max 9 chars): ");
    leerCadenaSegura(nuevoEvento.id, sizeof(nuevoEvento.id));

    printf("\nIngrese nombre del evento: ");
    leerCadenaSegura(nuevoEvento.nombre, sizeof(nuevoEvento.nombre));

    printf("\nIngrese tipo de evento: ");
    leerCadenaSegura(nuevoEvento.tipoEvento, sizeof(nuevoEvento.tipoEvento));

    char bufferEntrada[50];

    printf("\nIngrese ID de establecimiento: ");
    fflush(stdout);
    if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin) != NULL)
    {
        sscanf(bufferEntrada, "%u", &nuevoEvento.idEstablecimiento);
    }

    printf("\nIngrese fecha y hora (ej: 202610152030): ");
    fflush(stdout);
    if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin) != NULL)
    {
        sscanf(bufferEntrada, "%llu", &nuevoEvento.fechaHora);
    }

    nuevoEvento.estado = 1;

    // 3. Abrimos lectura y escritura en simultáneo para hacer UN SOLO recorrido
    pflectura = fopen("Eventos.bin", "rb");
    pfescritura = fopen("Eventostemp.bin", "wb");

    if (!pfescritura)
    {
        if (pflectura) fclose(pflectura);
        printf("\nError al Abrir archivo Eventos escritura");
        return ERROR_ARCHIVO_ESCRITURA;
    }

    if (pflectura != NULL)
    {
        while (fread(&eventoActual, sizeof(tEvento), 1, pflectura) == 1)
        {
            // Validamos duplicado al vuelo en el mismo bucle
            if (strcmp(eventoActual.id, nuevoEvento.id) == 0)
            {
                idRepetido = 1;
                break; // Rompemos la lectura porque ya sabemos que está repetido
            }


            if (!idInsertado && strcmp(nuevoEvento.id, eventoActual.id) < 0)
            {
                fwrite(&nuevoEvento, sizeof(tEvento), 1, pfescritura);
                idInsertado = 1;
            }

            // Escribimos el evento actual del archivo original
            fwrite(&eventoActual, sizeof(tEvento), 1, pfescritura);
        }
        fclose(pflectura);
    }

    if (idRepetido)
    {
        fclose(pfescritura);
        remove("Eventostemp.bin");
        printf("Error: Ya existe un evento con el identificador '%s'.\n", nuevoEvento.id);
        return IDENTIFICADOR_REPETIDO;
    }

    // Si el archivo estaba vacío o el ID nuevo es el mayor de todos, va al final
    if (!idInsertado)
    {
        fwrite(&nuevoEvento, sizeof(tEvento), 1, pfescritura);
    }

    fclose(pfescritura);


    remove("Eventos.bin");
    rename("Eventostemp.bin", "Eventos.bin");
    fflush(stdout);
    printf("¡Evento registrado y ordenado por ID con éxito!\n");

    return INSERCION_ID;
}

int contadorMatriz(tSector *sec,int estado)
{
    int f, c;
    int cont=0;

    for (f = 0; f < sec->filas; f++)
    {
        for (c = 0; c < sec->columnas; c++)
        {
            if(sec->ubicaciones[f][c].disponibilidad==estado)
            {
                cont++;
            }
        }
    }
    return cont;
}

int reporteOcupacion()
{
    char idEvento[10];
    tEvento evento;
    tEstablecimiento est;
    int capacidadTotalGlobal = 0;
    int cantOcupadas= 0;
    int cantDisponibles = 0;
    int i;
    float porcentajeOcupacion=0;
    printf("\n--- REPORTE DE OCUPACION DE EVENTO ---");
    printf("\nIngrese el ID del evento: ");
    leerCadenaConValidacion(idEvento, sizeof(idEvento));
    if (buscarEvento(idEvento, &evento) != TODO_OK)
    {
        printf("\n[ERROR] Evento inexistente.\n");
        return ERROR_EVENTO_NO_ENCONTRADO;
    }

    if (buscarEstablecimiento(evento.idEstablecimiento, &est) != TODO_OK)
    {
        printf("\n[ERROR] No se encontró el establecimiento asociado.\n");
        return ERROR_ESTABLECIMIENTO_NO_ENCONTRADO;
    }
    actualizarMatrizConVentas(est.sectores, idEvento);
    for (i = 0; i < MAX_SECTORES; i++)
    {
        if(est.sectores[i].id != 0)
        {
            printf("\n--- Sector: %s ---", est.sectores->nombre);
            cantOcupadas += contadorMatriz(&est.sectores[i],OCUPADO);
            cantDisponibles += contadorMatriz(&est.sectores[i],LIBRE);
        }
        capacidadTotalGlobal+= (est.sectores[i].filas * est.sectores[i].columnas);
    }

    porcentajeOcupacion = (capacidadTotalGlobal > 0) ? ((float)cantOcupadas / capacidadTotalGlobal) * 100 : 0;

    printf("\n==================================================");
    printf("\n            REPORTE GLOBAL DE OCUPACION           ");
    printf("\n==================================================");
    printf("\n- Capacidad Total: %d", capacidadTotalGlobal);
    printf("\n- Ubicaciones Ocupadas: %d", cantOcupadas);
    printf("\n- Ubicaciones Disponibles: %d", cantDisponibles);
    printf("\n- Porcentaje de Ocupación: %.2f%%\n", porcentajeOcupacion);
    printf("==================================================\n");

    return TODO_OK;
}


int buscarEventoAdmin()
{
    char idEvento[9];
    tEvento eventoEncontrado; // Declaramos la estructura local que vamos a llenar

    printf("\nIngrese el ID del evento para su búsqueda: ");
    scanf("%9s", idEvento);


    if (!buscarEventoEficiente(idEvento, &eventoEncontrado))
    {
        printf("\n[ERROR] No existe ningún evento activo con el código: %s\n", idEvento);
        return ERROR_EVENTO_NO_ENCONTRADO;
    }


    printf("\n==================================================");
    printf("\n             EVENTO ENCONTRADO                    ");
    printf("\n==================================================");
    printf("\n- ID: %s", eventoEncontrado.id);
    printf("\n- Nombre: %s", eventoEncontrado.nombre);
    printf("\n- Tipo: %s", eventoEncontrado.tipoEvento);
    printf("\n==================================================\n");

    return TODO_OK;
}

int buscarEventoEficiente(const char *idBuscado, tEvento *eventoEncontrado)
{
    FILE *pf= fopen("Eventos.bin","rb");
    if(!pf)
    {
        printf("\nNo se pudo abrir el archivo de lectura");
        return ERROR_ARCHIVO_LECTURA;
    }
    int cantRegistros=0;
    //long tamArchivo=0;
    int inf=0;
    int sup =0;
    int encontrado=0;
    int mitad=0;
    int comparacion=0;
    fseek(pf,0,SEEK_END);
    //tamArchivo=ftell(pf);
    cantRegistros= contarRegistrosTotales(pf);
    sup= cantRegistros-1;

    while(inf<= sup && !encontrado)
    {
        mitad= inf + (sup-inf)/2;

        fseek(pf,mitad*sizeof(tEvento),SEEK_SET);
        fread(eventoEncontrado,sizeof(tEvento),1,pf);
        comparacion = strcmp(eventoEncontrado->id,idBuscado);
        if(comparacion==0)
        {
            encontrado=1;
        }
        else if(comparacion<0)
        {
            inf = mitad+1;
        }
        else
        {
            sup = mitad-1;
        }
    }
    fclose(pf);
    return encontrado;
}

int buscarEventosPorEstablecimientoAdmin()
{
    int idEstablecimientoBuscado;
    printf("\nIngrese el ID del establecimiento (teatro/estadio): ");
    scanf("%d", &idEstablecimientoBuscado);

    FILE *pf = fopen("Eventos.bin", "rb");
    if (!pf)
    {
        printf("\n[ERROR] No se pudo abrir el archivo de eventos.\n");
        return ERROR_ARCHIVO_LECTURA;
    }

    tEvento aux;
    int encontrados = 0;

    printf("\n==================================================");
    printf("\n      EVENTOS PARA EL ESTABLECIMIENTO: %d         ", idEstablecimientoBuscado);
    printf("\n==================================================");

    // Recorremos todo el archivo binario secuencialmente para filtrar
    while (fread(&aux, sizeof(tEvento), 1, pf) == 1)
    {
        // Asumiendo que tu campo en tEvento se llama idEstablecimiento
        if (aux.idEstablecimiento == idEstablecimientoBuscado)
        {
            printf("\n- ID del Evento: %s", aux.id);
            printf("\n- Nombre: %s", aux.nombre);
            printf("\n- Tipo: %s", aux.tipoEvento);
            printf("\n--------------------------------------------------");
            encontrados++;
        }
    }

    fclose(pf);

    if (encontrados == 0)
    {
        printf("\nNo se encontraron eventos activos para este establecimiento.\n");
    }
    printf("\n==================================================\n");

    return TODO_OK;
}

int gestionarEvento()
{
    FILE *pf=fopen("Eventos.bin","r+b");
    if(!pf)
    {
        printf("\nError a la apertura de archivo Eventos");
        return ERROR_ARCHIVO_LECTURA;
    }
    tEvento eventoActual;
    char idEvento[9];
    int opcion=0;
    int encontrado=0;

    consultarEventos();
    printf("\nIngrese el evento deseado a gestionar:");
    scanf("%9s",idEvento);

    while (fread(&eventoActual, sizeof(tEvento), 1, pf) == 1)
    {
        if(strcmp(eventoActual.id,idEvento)==0)
        {
            encontrado=1;
            printf("\n|OPCIONES PARA GESTIONAR|(1: Disponible, -1: Cancelado, 2:Finalizado , 3: Reprogramado)");
            printf("\nIngrese el deseado:");
            scanf("%d",&opcion);
            switch(opcion)
            {
            case 1:
                eventoActual.estado = 1;
                fseek(pf, -(long)sizeof(tEvento), SEEK_CUR);
                fwrite(&eventoActual,sizeof(tEvento),1,pf);
                break;

            case -1:
                eventoActual.estado = -1;
                fseek(pf, -(long)sizeof(tEvento), SEEK_CUR);
                fwrite(&eventoActual,sizeof(tEvento),1,pf);
                break;

            case 2:
                eventoActual.estado = 2;
                fseek(pf, -(long)sizeof(tEvento), SEEK_CUR);
                fwrite(&eventoActual,sizeof(tEvento),1,pf);
                break;
            case 3:
                eventoActual.estado = 3;
                fseek(pf, -(long)sizeof(tEvento), SEEK_CUR);
                fwrite(&eventoActual,sizeof(tEvento),1,pf);
                break;
            default:
                printf("Opción inválida.\n");
                break;
            }
        }
    }

    if (!encontrado)
    {
        printf("\nNo se encontró ninguna evento activo con ese código: %s\n", idEvento);
        return ERROR_EVENTO_NO_ENCONTRADO;
    }
    printf("\nEvento: %s gestionado correctamente!",idEvento);
    fclose(pf);
    return TODO_OK;
}
/**
    //////////////////////USUARIO/////////////////////////////////
**/

void imprimirFechaHora(unsigned long long fechaHora)
{
    unsigned long long temp = fechaHora;

    int minuto = temp % 100;
    temp /= 100;

    int hora = temp % 100;
    temp /= 100;

    int dia = temp % 100;
    temp /= 100;

    int mes = temp % 100;
    temp /= 100;

    int anio = temp; // Lo que queda es el año

    // Muestra en formato DD/MM/YYYY - HH:MM
    printf("%02d/%02d/%d - %02d:%02d\n", dia, mes, anio, hora, minuto);
}


int buscarEvento(const char *idBuscado, tEvento *evento)
{
    FILE *pf = fopen("Eventos.bin", "rb");
    if (!pf)
        return ERROR_APERTURA_ARCHIVO;

    while (fread(evento, sizeof(tEvento), 1, pf) == 1)
    {
        if (strcmp(evento->id, idBuscado) == 0)
        {
            fclose(pf);
            return TODO_OK;
        }
    }

    fclose(pf);
    return ERROR_EVENTO_NO_ENCONTRADO;
}

int contarRegistrosTotales(FILE *pf)
{
    rewind(pf); // Volvemos al inicio del archivo
    tEvento temp;
    int contador = 0;
    while (fread(&temp, sizeof(tEvento), 1, pf) == 1)
    {
        contador++;
    }
    return contador;
}

int consultarEventoPorId()
{
    char idBuscado[9];
    tEvento eventoEncontrado;


    consultarEventos();
    printf("\nIngrese el ID del evento a buscar: ");
    scanf("%9s", idBuscado);


    if (buscarEventoEficiente(idBuscado, &eventoEncontrado))
    {

        printf("\n==================================================");
        printf("\n                 EVENTO ENCONTRADO               ");
        printf("\n==================================================");
        printf("\n- ID:     %s", eventoEncontrado.id);
        printf("\n- Nombre: %s", eventoEncontrado.nombre);
        printf("\n- Tipo:   %s", eventoEncontrado.tipoEvento);
        printf("\n==================================================\n");

       ;
    }
    else
    {
        printf("\n[ERROR] No se encontró ningún evento con el ID: %s\n", idBuscado);
        return ERROR_EVENTO_NO_ENCONTRADO;
    }

    return TODO_OK;
}

int contarEntradasPorCriterio(char *idEvento, const char *estadoBuscado)
{
    tCompra compraActual;
    int contador = 0;

    FILE *pf = fopen("Compras.bin", "rb");
    if (!pf)
    {
        return 0;
    }

    while (fread(&compraActual, sizeof(tCompra), 1, pf) == 1)
    {
        if (strcmp(compraActual.idEvento, idEvento) == 0)
        {
            // Comparamos contra el estado que nos pasaron por parámetro ("Cancelado", "Vendido", etc.)
            if (strcmp(compraActual.Entradas[0].estado, estadoBuscado) == 0)
            {
                contador += compraActual.cantEntradas;
            }
        }
    }

    fclose(pf);
    return contador;
}

float calcularRecaudacion(char *idEvento)
{
    tCompra compraActual;
    float recaudacion = 0;

    FILE *pf = fopen("Compras.bin", "rb");
    if (!pf) return 0;

    while (fread(&compraActual, sizeof(tCompra), 1, pf) == 1)
    {
        if (strcmp(compraActual.idEvento, idEvento) == 0)
        {
            if (strcmp(compraActual.Entradas[0].estado, "Cancelado") != 0)
            {
                recaudacion += compraActual.importeTotal;
            }
        }
    }
    fclose(pf);
    return recaudacion;
}

int consultarVentas()
{
    char idEvento[9];
    int cantVendidas=0;
    int cantCanceladas=0;
    float recaudacionTotalVendidas=0;

    consultarEventos();
    printf("\nIngrese el ID del evento a gestionar: ");
    scanf("%9s", idEvento);

    if (!existeEvento(idEvento))
    {
        printf("\n[ERROR] No existe ningún evento activo con el código: %s\n", idEvento);
        return ERROR_EVENTO_NO_ENCONTRADO;
    }

    cantVendidas = contarEntradasPorCriterio(idEvento, "Vendido");
    cantCanceladas = contarEntradasPorCriterio(idEvento, "Cancelado");
    recaudacionTotalVendidas= calcularRecaudacion(idEvento);

    printf("\n==================================================");
    printf("\n       REPORTE DE VENTAS PARA EL EVENTO          ");
    printf("\n==================================================");
    printf("\n- Entradas vendidas:   %d", cantVendidas);
    printf("\n- Entradas canceladas: %d", cantCanceladas);
    printf("\n- Recaudación vigente: $%.2f", recaudacionTotalVendidas);
    printf("\n==================================================\n");

    return TODO_OK;
}

int existeEvento(char *idBuscado)
{
    tEvento eventoEncontrado;

    return buscarEventoEficiente(idBuscado, &eventoEncontrado);
}

int consultarEventos()
{

    FILE *pf=fopen("Eventos.bin","rb");
    if(!pf)
    {
        printf("\nError a la apertura de archivo Eventos");
        return ERROR_ARCHIVO_LECTURA;
    }
    tEvento eventoActual;


    printf("\n\t=============EVENTOS===============\n");
    while (fread(&eventoActual, sizeof(tEvento), 1, pf) == 1)
    {
        if (eventoActual.estado == DISPONIBLE)
        {
            printf("\n-----------------------------------------------------");
            printf("\nID: %s",eventoActual.id);
            printf("\nNOMBRE: %s",eventoActual.nombre);
            printf("\nTIPO: %s",eventoActual.tipoEvento);
            printf("\nESTADO: %d",eventoActual.estado);
            printf("\nID ESTABLECIMIENTO: %u",eventoActual.idEstablecimiento);
            printf("\nFECHA/HORA: ");
            imprimirFechaHora(eventoActual.fechaHora);
            printf("\n-----------------------------------------------------");
        }

    }
    fclose(pf);

    return 1;
}

/**
Ejercicio Extra Propuesto: "Ranking de Recaudación por Tipo de Evento"
El administrador necesita un reporte consolidado que lea el archivo Eventos.bin,
 Compras.bin y Entradas.bin, y genere un listado dinámico en memoria (utilizando malloc y realloc)
  ordenado de mayor a menor por la recaudación total obtenida, agrupado por tipo de evento (ej: recitales, teatro, etc.).
   El resultado debe mostrarse en pantalla y además guardarse en un nuevo archivo de texto o binario llamado RankingRecaudacion.bin.**/


int crearVectorDinamico(tVector *v , size_t tamanio)
{
    v->vec = malloc(tamanio* TAM);
    if(!v->vec)
    {
        return ERROR_CREACION_VEC;
    }
    v->tam= tamanio;
    v->ce=0;
    v->cap= TAM;

    return 1;

}

int rankingRecaudacion()
{
    //int cantVendidas=0;
    //float recaudacionTotalVendidas=0;
    tEvento evento;
    FILE *pfEventos=fopen("Eventos.bin","rb");
    if(!pfEventos)
    {
        printf("\nError a la apertura de archivo Eventos");
        return ERROR_ARCHIVO_LECTURA;
    }
    tVector vector;
    if(!crearVectorDinamico(&vector,sizeof(tEvento)))
       {
           printf("\nError en la creacion del vector dinamico para la recaudacion");
           return ERROR_CREACION_VEC;
       }

    while(fread(&evento,sizeof(tEvento),1,pfEventos)==1)
        {

           //recaudacionTotalVendidas= calcularRecaudacion(evento.id);
            insertarVector(&vector,&evento,cmp);
        }

    mostrarRanking(&vector);
    return TODO_OK;
}

void mostrarRanking(tVector *v)
{
    printf("\n========================================\n");
    printf("   RANKING DE RECAUDACIÓN DE EVENTOS    \n");
    printf("========================================\n");

    if (v->ce == 0)
    {
        printf("El vector está vacío.\n");
        return;
    }

    for (size_t i = 0; i < v->ce; i++)
    {
        // Nos posicionamos en el elemento i usando aritmética de punteros con char*
        tEvento *eventoActual = (tEvento*)((char*)v->vec + (i * v->tam));

        // Calculamos o leemos su recaudación
        float recaudacion = calcularRecaudacion(eventoActual->id);

        printf("%d. Evento ID: %u | Nombre: %s | Recaudación: $%.2f\n",i + 1,eventoActual->id,eventoActual->nombre,recaudacion); // Ajustá el campo según cómo se llame en tu struct tEventorecaudacion);
    }
    printf("========================================\n");
}

int cmp(const void *a , const void *b)
{
    tEvento *pa = (tEvento*)a;
    tEvento *pb = (tEvento*)b;
    float recA=0;
    float recB=0;

    recA=calcularRecaudacion(pa->id);
    recA=calcularRecaudacion(pb->id);
    if (recA < recB) return 1;   // Para ordenar de mayor a menor
    if (recA > recB) return -1;
    return 0;
}

int insertarVector(tVector *v , void *elem,int cmp(const void* , const void*))
{
    int i;
    if(v->ce==0)
    {
        memcpy(v->vec,elem,v->tam);
        v->ce++;
        return TODO_OK;
    }
    i = v->ce-1;

    while (i >= 0)
    {

        void *elemActual = (char*)v->vec + (i * v->tam);

        // Comparamos la recaudación del elemento actual con la nueva
        if (cmp(elemActual, elem) > 0)
        {
            break; // Encontramos su posición
        }

        // Si es menor, desplazamos el bloque de memoria una posición hacia adelante
        void *destino = (char*)v->vec + ((i + 1) * v->tam);
        memcpy(destino, elemActual, v->tam);
        i--;
    }

    // Insertamos el nuevo elemento en la posición que quedó libre
    void *posInsercion = (char*)v->vec + ((i + 1) * v->tam);
    memcpy(posInsercion, elem, v->tam);
    v->ce++;
    return TODO_OK;
}
