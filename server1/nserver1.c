#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <signal.h>

#define PORT 5001
#define FLAG_FILE "/tmp/server1_flag"

void *connection_handler(void *);
void cleanup(int);

int main(int argc, char const *argv[]) {
    // Проверяем наличие файла-флага
    int flag_fd = open(FLAG_FILE, O_CREAT | O_EXCL, 0644);
    if (flag_fd < 0) {
        printf("Server 1 is already running\n");
        return 1;
    }
    // Регистрируем обработчик сигналов для удаления файла-флага при завершении  
    signal(SIGINT, cleanup);  // обработка Ctrl+C  
    signal(SIGTERM, cleanup); // обработка kill
    
    int socket_desc, new_socket, c, *new_sock;
    struct sockaddr_in server, client;

    // Создаем сокет
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
        return 1;
    }

    // Настраиваем структуру адреса сервера
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Привязываем сокет к адресу сервера
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("Bind failed");
        return 1;
    }

    // Слушаем входящие соединения
    listen(socket_desc, 3);

    // Принимаем входящие соединения и создаем отдельный поток для каждого клиента
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c))) {
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;
        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *) new_sock) < 0) {
            printf("Could not create thread");
            return 1;
        }
    }

    if (new_socket < 0) {
        printf("Accept failed");
        return 1;
    }

    return 0;
}

void cleanup(int signum){
    remove(FLAG_FILE);
    exit(signum);
}

void *connection_handler(void *socket_desc) {
    int sock = *(int *) socket_desc;
    int read_size;
    char *message, client_message[1024];

    // Получаем запрос от клиента
    memset(client_message, 0, sizeof(client_message));
    while ((read_size = recv(sock, client_message, sizeof(client_message), 0)) > 0) {
        time_t current_time = time(NULL);
        char time_str[50];
        strftime(time_str, 50, "%Y-%m-%d %H:%M:%S", localtime(&current_time));

        if (strcmp(client_message, "1") == 0) {
            int priority = getpriority(PRIO_PROCESS, getpid());
            char priority_str[50];
            sprintf(priority_str, "%d", priority);
            message = malloc(strlen(time_str) + strlen(priority_str) + 3);
            sprintf(message, "%s: %s", time_str, priority_str);
        } else if (strcmp(client_message, "2") == 0) {
            pid_t pid = getpid();
            char pid_str[50];
            sprintf(pid_str, "%d", pid);
            int fd = sock;
            char fd_str[50];
            sprintf(fd_str, "%d", fd);
            message = malloc(strlen(time_str) + strlen(pid_str) + strlen(fd_str) + 5);
            sprintf(message, "%s: %s, %s", time_str, pid_str, fd_str);
        } else {
            message = "Invalid command";
        }
        
        // Отправляем ответ клиенту  
        write(sock, message, strlen(message));  
  
        // Освобождаем память, выделенную под сообщение  
        free(message);  
        // Очищаем буфер сообщения клиента  
        memset(client_message, 0, sizeof(client_message));
   }
   
   if (read_size == 0) {  
        puts("Client disconnected");  
    } else if (read_size == -1) {  
        perror("recv failed");  
    }

    // Освобождаем сокет и завершаем поток
    close(sock);
    
    free(socket_desc);
    //free(message);
    pthread_exit(NULL);
}
