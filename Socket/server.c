#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0); //tcp
    if (server_fd == -1) {
        perror("socket creation failed...");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed...");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //监听
    if (listen(server_fd, 5) == -1) {
        perror("listen failed...");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("server inited，port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            perror("accept failed...");
            continue;
        }

        printf("client connected：%s\n", inet_ntoa(client_addr.sin_addr));

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (bytes <= 0) {
                printf("empty msg...\n");
                break;
            }
            printf("msg from client: %s\n", buffer);

            char reply[BUFFER_SIZE];
            snprintf(reply, sizeof(reply), "receive: %s", buffer);
            send(client_fd, reply, strlen(reply), 0); //发送回复
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
