#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

int main()
{
    char *ip = "127.0.0.19";
    int port = 5590;
    int receiver_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (receiver_sock < 0)
    {
        printf("[-]socket creation failed\n");
        exit(1);
    }
    else
    {
        printf("[+]UDP socket succesfully created\n");
    }

    struct sockaddr_in receiver_addr;
    socklen_t len;

    memset(&receiver_addr, '\0', sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = port;
    receiver_addr.sin_addr.s_addr = inet_addr(ip);

    int b = bind(receiver_sock, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
    if (b < 0)
    {
        printf("[-]Binding error\n");
        exit(1);
    }
    else
    {
        printf("[+]Binding succesfull\n");
    }

    char buffer[1024];

    while (1)
    {
        len = sizeof(receiver_addr);

        bzero(buffer, 1024);

        int t;
        do
        {
            t = recvfrom(receiver_sock, buffer, 1024, 0, (struct sockaddr *)&receiver_addr, &len);
            printf("Frame Number: %s\n", buffer);

            char frameno[100];
            bzero(frameno, 100);
            strcpy(frameno, buffer);
            bzero(buffer, 1024);
            strcpy(buffer, frameno);

            sendto(receiver_sock, buffer, 1024, 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));

            printf("Acknowledgement sent to receiver: %s\n", buffer);
        } while (t > 0);
    }

    return 0;
}
