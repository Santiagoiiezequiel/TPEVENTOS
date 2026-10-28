#include "TPGrupal.h"
#include "Eventos.h"
#include "Establecimientos.h"
#include "Compras.h"

int main()
{
    setlocale(LC_ALL, ""); // Para ñ o tildes
    int opcion, eventoUser, eventoAdmin;
    char idUsuario[9];

    crearArchEstablecimiento();
    /**unsigned long long tiempoInicioCompra; esto va en COMPRA**/
    do
    {
        printf("\n=== SISTEMA DE GESTION DE EVENTOS ===\n");
        printf("Seleccione el perfil de inicio de sesión:\n");
        printf("1. ADMIN\n");
        printf("2. USER\n");
        printf("0. Salir\n");
        printf("Elija una opción: ");
        scanf("%d", &opcion);




        switch(opcion)
        {
        case 1: // MENÚ ADMIN
            do
            {
                printf("\n--- Menú ADMINISTRADOR ---\n");
                printf("1. Dar Alta evento/s\n");
                printf("2. Buscar evento\n");
                printf("3. Gestionar evento\n");
                printf("4. Consultar establecimiento\n");
                printf("5. Consultar ventas\n");
                printf("6. Consultar eventos por Establecimiento\n");
                printf("7. Reporte de ocupacion\n");
                printf("8. Ranking de Recaudacion por tipo de Evento\n");
                printf("0. Volver al menú principal\n");
                printf("Opción: ");
                scanf("%d", &eventoAdmin);

                switch(eventoAdmin)
                {
                case 1:
                    AltaEvento();
                    break;
                case 2:
                    buscarEventoAdmin();
                    break;
                case 3:
                    gestionarEvento();
                    break;
                case 4:
                    consultarEstablecimientos();
                    break;

                case 5:
                    consultarVentas();
                    break;
                case 6:
                    buscarEventosPorEstablecimientoAdmin();
                    break;
                case 7:
                    reporteOcupacion();
                    break;
                case 8:
                    rankingRecaudacion();
                    break;
                case 0:
                    printf("Volviendo...\n");
                    break;
                default:
                    printf("Opción inválida.\n");
                    break;
                }
            }
            while(eventoAdmin != 0);
            break;

        case 2: // MENÚ USER

            ValidarUsuario(idUsuario);

            do
            {
                printf("\n--- Menú USUARIO (%s) ---\n", idUsuario);
                printf("1. Comprar entrada/s\n");
                printf("2. Consultar Entrada/s\n");
                printf("3. Cancelar entrada/s\n");
                printf("4. Consultar eventos\n");
                printf("5. Consultar establecimiento\n");
                printf("6. Consultar ubicacion y capacidad\n");
                printf("0. Volver al menú principal\n");
                printf("Opción: ");
                scanf("%d", &eventoUser);
                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                switch(eventoUser)
                {
                case 1:

                    comprarEntrada(idUsuario);
                    break;
                case 2:
                    consultarEntrada(idUsuario);
                    break;
                case 3:
                    cancelarEntrada(idUsuario);
                    break;
                case 4:
                    consultarEventos();
                    break;
                case 5:
                    consultarEstablecimientos();
                    break;
                case 6:
                    consultarUbiyCapacidad();
                    break;
                case 0:
                    printf("Volviendo...\n");
                    break;
                default:
                    printf("Opción inválida.\n");
                    break;
                }
            }
            while(eventoUser != 0);
            break;

        case 0:
            printf("Saliendo del programa...\n");
            break;

        default:
            printf("\nOpción inválida, inténtelo de nuevo.\n");
            break;
        }
    }
    while(opcion != 0);
    liberarMemoriaSectores();
    printf("Hasta pronto!\n");
    return 0;
}
