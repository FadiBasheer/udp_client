#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAX 80
#define PORT 2007
#define SA struct sockaddr



// Client side implementation of UDP client-server model

#define MAXLINE 1024

// Driver code
int UDP_client() {
    int sockfd;
    //  char buffer[MAXLINE];
    char *bye = "bye";
    struct sockaddr_in servaddr;


    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    // servaddr.sin_addr.s_addr = INADDR_ANY;

    inet_pton(AF_INET, "192.168.0.31", &servaddr.sin_addr);

    int n, len;
    char *str;

    /* Initial memory allocation */
    str = (char *) malloc(100);

    for (int i = 0; i < 5; ++i) {
        sprintf(str, "%d %s%c", i, "hello", '\0');
        sendto(sockfd, str, strlen(str),
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        printf("Hello message sent.\n");
        sleep(1);
    }

//    n = recvfrom(sockfd, (char *) buffer, MAXLINE,
//                 MSG_WAITALL, (struct sockaddr *) &servaddr,
//                 &len);
//    buffer[n] = '\0';
//    printf("Server UDP: %s\n", buffer);

    close(sockfd);
    return 0;
}


void func(int sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("\nEnter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));

        if ((strncmp(buff, "exit", 4)) == 0) {
            read(sockfd, buff, sizeof(buff));
            printf("From Server : %s", buff);
            printf("Client Exit...\n");
            break;
        }

        bzero(buff, sizeof(buff));

        sleep(3);

//        strncpy(buff, "exit", 4);
//        write(sockfd, buff, sizeof(buff));

        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);

        UDP_client();
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.0.31");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    } else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
