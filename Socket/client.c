#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    //创建socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //tcp
    if (sockfd == -1) {
        perror("socket creation failed...");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //连接服务器
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed...");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("connect success!!!\n");

    while (1) {
        printf("send msg to server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "quit") == 0) {
            break;
        }

        //发送消息
        send(sockfd, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);
        recv(sockfd, buffer, BUFFER_SIZE, 0); //接收响应
        printf("response from server: %s\n", buffer);
    }

    close(sockfd); //记得关闭
    return 0;
}
