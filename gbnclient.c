#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdlib.h>

void sendWindowPackets(int *sockfd, struct sockaddr_in addr, int packets[], int windowStart, int windowEnd)
{
    char buffer[1024];
    while (windowStart < windowEnd)
    {
        bzero(buffer, 1024);
        sprintf(buffer, "%d", packets[windowStart]);
        printf("Client: Sending packet %s\n", buffer);
        sendto(*sockfd, buffer, 1024, 0, (struct sockaddr *)&addr, sizeof(addr));
        windowStart++;
    }
}

void main()
{
    char *ip = "127.0.0.100";
    int port = 5567;

    int sockfd;
    struct sockaddr_in addr;
    char buffer[1024];
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    int packets[5];
    for (int i = 0; i < 5; ++i)
        packets[i] = i + 1;

    int windowStart = 0;
    int windowEnd = windowStart + 3;

    int flag = 1;
    sendWindowPackets(&sockfd, addr, packets, windowStart, windowEnd);
    while (windowStart != windowEnd)
    {
        bzero(buffer, 1024);
        addr_size = sizeof(addr);

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
            printf("Error\n");

        else
        {
            bzero(buffer, 1024);
            addr_size = sizeof(addr);

            int rec = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&addr, &addr_size);
            if (rec < 0)
            {
                printf("Client: Timeout error! Sending window packets again\n");
                sendWindowPackets(&sockfd, addr, packets, windowStart, windowEnd);
            }
            else
            {
                printf("Client: Received acknowledgement for packet %s\n", buffer);
                if (atoi(buffer) != packets[windowStart])
                {
                    printf("Client: Wrong acknowledgement! Sending window packets again\n");
                    sendWindowPackets(&sockfd, addr, packets, windowStart, windowEnd);
                }
                else
                {
                    windowStart++;
                    if (windowEnd < 5)
                        windowEnd++;

                    if (windowStart != windowEnd)
                    {
                        bzero(buffer, 1024);
                        if (flag == 1)
                        {
                            sprintf(buffer, "%d", packets[windowEnd - 1]);
                            printf("Client: Sending packet %s\n", buffer);
                            sendto(sockfd, buffer, 1024, 0, (struct sockaddr *)&addr, sizeof(addr));
                        }
                        if (windowEnd == 5)
                            flag = 0;
                    }
                }
            }
        }

        sleep(1);
    }
}
