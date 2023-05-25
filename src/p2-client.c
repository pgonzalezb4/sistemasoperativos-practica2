/*
 * Connects to the server database and displays CLI menu in order to perform queries.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include "ride.h"
#define SA struct sockaddr
#define SERVER_PORT 8080

int goption(int min, int max);

int main()
{
    int server_fd;
    struct sockaddr_in server_addr;

    /* Initialize socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket failed.");
        exit(EXIT_FAILURE);
    }

    /* Configure socket address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));
    bzero(server_addr.sin_zero, 8);

    /* Connect to server socket */
    if (connect(server_fd, (SA *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed.");
        exit(EXIT_FAILURE);
    }

    Ride ride;
    ride.source_id = -1;
    ride.dest_id = -1;
    ride.hour = -1;

    int option;
    bool exit = false;

    /* Menu prompt loop */
    while (!exit)
    {

        printf("\n");
        printf("Bienvenido:\n");
        printf("1. Ingresar origen\n");
        printf("2. Ingresar destino\n");
        printf("3. Ingresar hora\n");
        printf("4. Buscar tiempo de viaje medio\n");
        printf("5. Salir\n");

        option = goption(1, 5);

        switch (option)
        {

        case 1:
            printf("Seleccione el ID de origen (1 - 1160): \n");
            ride.source_id = goption(1, 1160);
            break;
        case 2:
            printf("Seleccione el ID de destino (1 - 1160): \n");
            ride.dest_id = goption(1, 1160);
            break;
        case 3:
            printf("Seleccione la hora (0 - 23): \n");
            ride.hour = goption(0, 23);
            break;
        case 4:
            if (ride.source_id == -1 || ride.dest_id == -1 || ride.hour == -1)
            {
                printf("Porfavor complete la información del viaje.\n");
                break;
            }
            send(server_fd, &ride, sizeof(ride), 0);
            read(server_fd, &ride.avg_time, sizeof(ride.avg_time));

            if (ride.avg_time < 0)
                printf("No hay un viaje con las especificaciones mencionadas.\n");
            else
                printf("El tiempo medio de viaje es: %0.2f.\n", ride.avg_time);

            ride.source_id = -1;
            ride.dest_id = -1;
            ride.hour = -1;

            break;

        case 5:
            ride.source_id = -1;
            exit = true;
            send(server_fd, &ride, sizeof(ride), 0);
            close(server_fd);
            break;
        }
    }

    return 0;
}

/* Get number from stdin between min and max. */
int goption(int min, int max)
{
    char input[100];
    int option;
    char *remaining;
    for (;;)
    {
        fgets(input, sizeof(input), stdin);
        option = strtol(input, &remaining, 10);

        if (remaining[0] == '\n')
            if (option >= min && option <= max)
            {
                printf("\n");
                break;
            }
            else
                printf("Seleccione un numero entre %d y %d.\n", min, max);
        else
            printf("Seleccione una opcion correcta.\n");
    }
    return option;
}
