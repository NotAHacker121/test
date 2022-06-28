#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    char *ip = "127.0.0.19";
    int port = 5590;
    int sender_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sender_sock < 0)
    {
        printf("[-]socket creation failed\n");
        exit(1);
    }
    else
    {
        printf("[+]UDP socket succesfully created\n");
    }

    struct sockaddr_in sender_addr;
    socklen_t len;

    memset(&sender_addr, '\0', sizeof(sender_addr));
    sender_addr.sin_family = AF_INET;
    sender_addr.sin_port = port;
    sender_addr.sin_addr.s_addr = inet_addr(ip);

    int i = 0, frameno;
    printf("Enter the number of frames: ");
    scanf("%d", &frameno);

    while (frameno > 0)
    {
        char buffer[1024];
        bzero(buffer, 1024);
        sprintf(buffer, "%d", i);

        sendto(sender_sock, buffer, 1024, 0, (struct sockaddr *)&sender_addr, sizeof(sender_addr));
        printf("Frame number :%s\n", buffer);

        bzero(buffer, 1024);
        len = sizeof(sender_addr);
        recvfrom(sender_sock, buffer, 1024, 0, (struct sockaddr *)&sender_addr, &len);
        printf("Acknowledgement Number: %s\n", buffer);

        i++;
        frameno--;
    }

    close(sender_sock);

    return 0;
}
