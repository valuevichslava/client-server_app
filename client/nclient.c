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

    int socket_desc;  // Дескриптор сокета  
    struct sockaddr_in server;  // Структура, содержащая информацию о сервере  
    char message[1024], server_reply[1024];  // Буферы для сообщений  
    int option = 0;  // Переменная для хранения выбранного пользователем пункта меню  
    struct hostent *he;  // Структура, содержащая информацию о хосте  
    struct in_addr **addr_list;  // Список IP-адресов хоста  
    int server_port;  // Порт сервера  
  
    // Основной цикл программы  
    while (1) {  
        print_menu();  // Выводим главное меню  
        scanf("%d", &option);  // Читаем выбор пользователя  
  
        // В зависимости от выбора пользователя подключаемся к нужному серверу  
        switch (option) {  
            case 1:  // Если выбран первый сервер  
                he = gethostbyname(SERVER1_IP);  // Получаем информацию о первом сервере  
                server_port = SERVER1_PORT;  // Устанавливаем порт первого сервера  
                break;  
            case 2:  // Если выбран второй сервер  
                he = gethostbyname(SERVER2_IP);  // Получаем информацию о втором сервере  
                server_port = SERVER2_PORT;  // Устанавливаем порт второго сервера  
                break;  
            case 3:  // Если выбран пункт "Выход"  
                exit(0);  // Завершаем программу  
            default:  // Если выбран несуществующий пункт меню  
                printf("Invalid option\n");  // Выводим сообщение об ошибке  
                continue;  // Возвращаемся к началу цикла  
        }  
  
        // Если информация о сервере не получена, выводим сообщение об ошибке  
        if (he == NULL) {  
            herror("gethostbyname");  
            return 1;  
        }  
  
        addr_list = (struct in_addr **)he->h_addr_list;  // Получаем список IP-адресов сервера  
  
        // Создаем сокет  
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);  
        if (socket_desc == -1) {  
            printf("Could not create socket");  
            return 1;  
        }  
  
        // Настраиваем структуру адреса сервера  
        server.sin_addr = *addr_list[0];  // Устанавливаем IP-адрес сервера  
        server.sin_family = AF_INET;  // Устанавливаем семейство адресов  
        server.sin_port = htons(server_port);  // Устанавливаем порт сервера  
  
        // Подключаемся к серверу  
        if (connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {  
            printf("Connection error");  
            return 1;  
        }  
  
        printf("Connected to server %d\n", option);  // Выводим сообщение о подключении к серверу  
  
        // Второй цикл, в котором обрабатываются команды, отправляемые на сервер  
        while (1) {  
            // Выводим меню в зависимости от выбранного сервера  
            if (option == 1) {    
                print_server1_menu();    
            } else if (option == 2) {    
                print_server2_menu();    
            }  
              
            int server_option;  // Переменная для хранения выбранного пункта меню  
            scanf("%d", &server_option);  // Читаем выбор пользователя  
  
            // Обрабатываем выбор пользователя  
            switch (server_option) {  
                case 1:  
                case 2:  
                    sprintf(message, "%d", server_option);  // Формируем сообщение для отправки на сервер  
                    // Отправляем сообщение на сервер  
                    if (send(socket_desc, message, strlen(message), 0) < 0) {  
                        printf("Send failed");  // Если сообщение не удалось отправить, выводим сообщение об ошибке  
                        return 1;  
                    }  
                      
                    // Получаем ответ от сервера    
                    memset(server_reply, 0, sizeof(server_reply));  // Очищаем буфер перед получением нового сообщения  
                    if (recv(socket_desc, server_reply, 1024, 0) < 0) {  
                        printf("Receive failed");  // Если ответ не удалось получить, выводим сообщение об ошибке  
                        return 1;  
                    }  
  
                    printf("Server reply: %s\n", server_reply);  // Выводим ответ сервера  
                    break;  
                case 3:  // Если выбран пункт "Выход"  
                    close(socket_desc);  // Закрываем сокет  
                    goto MAIN_MENU;  // Возвращаемся к главному меню  
                default:  // Если выбран несуществующий пункт меню  
                    printf("Invalid option\n");  // Выводим сообщение об ошибке  
                    break;  
            }  
        }  
  
        MAIN_MENU: continue;  // Метка для возвращения к главному меню  
    }  
  
    return 0;  
}  
