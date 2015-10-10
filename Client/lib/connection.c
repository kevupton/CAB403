//
// Created by Kevin on 10/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <pthread.h>
#include "connection.h"

Connection *newConnection(char *ip, char *port) {
    Connection *c = malloc(sizeof(Connection));
    c->ip_address = ip;
    c->port = atoi(port);

    if (c->port == 0) {
        puts("Invalid Port.");
        return NULL;
    }

    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    //Create socket
    c->_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (c->_sock == -1)
    {
        printf("Could not create socket");
        return NULL;
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(c->ip_address);
    server.sin_family = AF_INET;
    server.sin_port = htons( c->port ); //1134

    //Connect to remote server
    if (connect(c->_sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return NULL;
    }

    return c;
}

int Connection_initialise(Connection *c) {
    if (pthread_create(&c->listener, NULL, Connection_listen, c)) {
        return 0;
    }
    return 1;
}

void Connection_close(Connection *c) {
    close(c->_sock);
}

void *Connection_listen(Connection *c) {
    char server_reply[2000];

    //begin the thread
    //keep communicating with server
    while(1)
    {
        //Receive a reply from the server
        if( recv(c->_sock , server_reply , 2000 , 0) < 0)
        {
            puts("Connection Closed");
            //end the game
            break;
        }

        puts("Server reply :");
        puts(server_reply);
    }
}

int Connection_send(Connection *c, char *msg) {
    //Send some data
    return send(c->_sock , msg , strlen(msg) , 0);
}