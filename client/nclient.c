#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>  // For gethostbyname()

#define SERVER1_IP "nserver1"
#define SERVER1_PORT 5001
#define SERVER2_IP "nserver2"
#define SERVER2_PORT 5002

void print_menu() {
    printf("\n\nChoose an option:\n");
    printf("1. Connect to server 1\n");
    printf("2. Connect to server 2\n");
    printf("3. Exit\n");
}

void print_server1_menu() {
    printf("\n\nChoose an option:\n");
    printf("1. Get server priority\n");
    printf("2. Get server process id and descriptor\n");
    printf("3. Exit to main menu\n");
}

void print_server2_menu() {  
    printf("\n\nChoose an option:\n");  
    printf("1. Get client keyboard layout code\n");  
    printf("2. Get server operating system version\n");  
    printf("3. Exit to main menu\n");  
}

int main(int argc, char const *argv[]) {

    int socket_desc;
    struct sockaddr_in server;
    char message[1024], server_reply[1024];
    int option = 0;
    struct hostent *he;
    struct in_addr **addr_list;
    int server_port;

    while (1) {
        print_menu();
        scanf("%d", &option);

        switch (option) {
            case 1:
                he = gethostbyname(SERVER1_IP);
                server_port = SERVER1_PORT;
                break;
            case 2:
                he = gethostbyname(SERVER2_IP);
                server_port = SERVER2_PORT;
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid option\n");
                continue;
        }

        if (he == NULL) {
            herror("gethostbyname");
            return 1;
        }

        addr_list = (struct in_addr **)he->h_addr_list;

        // Создаем сокет
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1) {
            printf("Could not create socket");
            return 1;
        }

        // Настраиваем структуру адреса сервера
        server.sin_addr = *addr_list[0];
        server.sin_family = AF_INET;
        server.sin_port = htons(server_port);

        // Подключаемся к серверу
        if (connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
            printf("Connection error");
            return 1;
        }

        printf("Connected to server %d\n", option);

        while (1) {
            if (option == 1) {  
                print_server1_menu();  
            } else if (option == 2) {  
                print_server2_menu();  
            }
            
            int server_option;
            scanf("%d", &server_option);

            switch (server_option) {
                case 1:
                case 2:
                    sprintf(message, "%d", server_option);
                    if (send(socket_desc, message, strlen(message), 0) < 0) {
                        printf("Send failed");
                        return 1;
                    }
                    
                    // Получаем ответ от сервера  
                    memset(server_reply, 0, sizeof(server_reply));  // Очищаем буфер перед получением нового сообщения
                    if (recv(socket_desc, server_reply, 1024, 0) < 0) {
                        printf("Receive failed");
                        return 1;
                    }

                    printf("Server reply: %s\n", server_reply);
                    break;
                case 3:
                    close(socket_desc);
                    goto MAIN_MENU;
                default:
                    printf("Invalid option\n");
                    break;
            }
        }

        MAIN_MENU: continue;
    }

    return 0;
}
