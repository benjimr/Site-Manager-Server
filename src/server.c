#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include "handler.h"
#include "shared.h"

int main(int argc, char * argv[])
{
    int lSock;
    int cSock;
    
    struct sockaddr_in lAddr;
    struct sockaddr_in cAddr;


    socklen_t len = sizeof(cAddr);

    lAddr.sin_family = AF_INET;
    lAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    lAddr.sin_port = htons(7777);

    lSock = socket(AF_INET, SOCK_STREAM, 0);
    bind(lSock, (struct sockaddr*)&lAddr, sizeof(lAddr));

    listen(lSock, 10);

    while(1)
    {
        printf("Waiting for connections...");
        fflush(stdout);
        cSock = accept(lSock, (struct sockaddr *)&cAddr, &len);

        struct connArgs args;
        args.sock = cSock;
        args.addr = cAddr;
        
        char *connectedIP = inet_ntoa(cAddr.sin_addr);

        printf("New connection from %s", connectedIP);
        fflush(stdout);
        pthread_t thread;
        int retval = pthread_create(&thread, NULL, &handler, (void *)&args);

        if(retval != 0)
        {
            printf("Error creating thread");
        };
    };
};

