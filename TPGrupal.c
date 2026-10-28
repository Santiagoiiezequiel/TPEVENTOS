#include "TPGrupal.h"


void leerCadenaSegura(char *destino, int tamMaximo)/**Esto es para el administrador , no hay restriccion ya que deberia saber los limites**/
{

    fflush(stdout);

    int i = 0;
    int c;
    char *ptr = destino;


    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (i < tamMaximo - 1)
        {
            *ptr = (char)c;  // Asignamos el valor a la dirección de memoria
            ptr++;
            i++;
        }

    }
    *ptr = '\0';
}

void leerCadenaConValidacion(char *destino, int tamMaximo)
{
    int valido = 0;

    while (!valido)
    {
        fflush(stdout);

        int i = 0;
        int c;
        char bufferTemp[100];

        // 1. DESCARTAR BASURA: salto cualquier enter o espacio sobrante del buffer anterior
        while ((c = getchar()) == '\n' || c == '\r' || c == ' ');

        if (c == EOF) return;

        // Guardamos el primer caracter real que ingresó el usuario
        bufferTemp[i++] = (char)c;

        // 2. Leemos el resto de la cadena hasta el Enter
        while ((c = getchar()) != '\n' && c != EOF)
        {
            if (i < sizeof(bufferTemp) - 1)
            {
                bufferTemp[i] = (char)c;
            }
            i++;
        }

        int idxTermino = (i < sizeof(bufferTemp)) ? i : sizeof(bufferTemp) - 1;
        bufferTemp[idxTermino] = '\0';


        if (i >= tamMaximo)
        {
            printf("\n[ERROR] El texto es demasiado largo. Maximo permitido: %d caracteres. Intente nuevamente.\n\n", tamMaximo - 1);
        }
        else if (i == 0) //si el usuario ingreso sin apretar nada.
        {
            printf("\n[ERROR] El campo no puede estar vacio. Intente nuevamente.\n\n");
        }
        else
        {

            int j = 0;
            while (bufferTemp[j] != '\0')
            {
                destino[j] = bufferTemp[j];
                j++;
            }
            destino[j] = '\0';

            valido = 1;
        }
    }
}


char *validarIDEvento(char *idBuscado)
{
    int caracter, idValido;
    char ingresoId[10], *salto;
     do
    {
        caracter = getchar();

    }while(caracter != '\n' && caracter != EOF);

    idValido = 0;

    do
    {
        printf("\nEscriba su ID del EVENTO (10 caracteres):  ");

        if (fgets(ingresoId, sizeof ingresoId, stdin) == NULL)
        {
            printf("No se pudo leer el ID.\n");
            return NULL;
        }

         salto = strchr(ingresoId, '\n');

        if (salto != NULL)
        {
            *salto = '\0';
        }

        if (strlen(ingresoId) > 10)
        {
            do
            {
                caracter = getchar();

            }while(caracter != '\n' && caracter != EOF);

            printf("\n El ID no puede superar los 8 caracteres.\n");
        }
        else if (strlen(ingresoId) < 8)
        {
            printf("\n El ID debe tener exactamente 10 caracteres.\n");
        }
        else
        {
            strcpy(idBuscado, ingresoId);
            idValido = 1;
        }

    } while (!idValido);

    return idBuscado;
}

int ValidarUsuario(char *idUsuario)
{
    int caracter, idValido;
    char ingresoId[10], *salto;

    do
    {
        caracter = getchar();

    }while(caracter != '\n' && caracter != EOF);

    idValido = 0;

    do
    {
        printf("\nEscriba su ID de usuario (8 caracteres):  ");

        if (fgets(ingresoId, sizeof ingresoId, stdin) == NULL)
        {
            printf("No se pudo leer el ID.\n");
            return 1;
        }

         salto = strchr(ingresoId, '\n');

        if (salto != NULL)
        {
            *salto = '\0';
        }

        if (strlen(ingresoId) > 8)
        {
            do
            {
                caracter = getchar();

            }while(caracter != '\n' && caracter != EOF);

            printf("\n El ID no puede superar los 8 caracteres.\n");
        }
        else if (strlen(ingresoId) < 8)
        {
            printf("\n El ID debe tener exactamente 8 caracteres.\n");
        }
        else
        {
            strcpy(idUsuario, ingresoId);
            idValido = 1;
        }

    } while (!idValido);

    return 0;
}
