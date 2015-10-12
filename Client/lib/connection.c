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

static const int DATA_LENGTH = 30;
static const int DATA_SIZE = 10;

Connection *newConnection(char *ip, char *port) {
    Connection *c = malloc(sizeof(Connection));
    c->ip_address = ip;
    c->port = atoi(port);

    if (c->port == 0) {
        puts("Invalid Port.");
        return NULL;
    }

    struct sockaddr_in server;

    //Create socket
    c->_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (c->_sock == -1)
    {
        printf("Could not create socket");
        return NULL;
    }
    //puts("Socket created");

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

int Connection_initialise() {
    if (pthread_create(&control->conn->listener, NULL, Connection_listen, control->conn)) {
        return 0;
    }
    return 1;
}

void Connection_close() {
    close(control->conn->_sock);
    control->conn = NULL;
}

void *Connection_listen() {
    //begin the thread
    //keep communicating with server
    int n = 0;
    char server_reply[2000];

    while(1)
    {
        memset(server_reply, 0, 2000);

        //Receive a reply from the server
        if((n = recv(control->conn->_sock , server_reply , 2000 , 0)) <= 0)
        {
            puts("Connection Closed");
            Connection_close();
            Control_exit();
            break;
        }
        if (n != 0) {
            puts("Server reply :");
            puts(server_reply);
        }
    }
}

int Connection_send(char *msg) {
    //Send some data
    return send(control->conn->_sock , msg , strlen(msg) , 0);
}

int Connection_login(char *username, char *password) {
    char *data[2] = {username, password};
    Connection_send(_prepare_msg("login", data, 2));
}

char *_prepare_msg(char *route, char **data, int len) {
    int i;
    char *to_send[DATA_LENGTH + 1];

    for (i = 0; i < DATA_LENGTH + 1; i++) {
        to_send[i] = malloc(DATA_SIZE * sizeof(char));
    }
    strncpy(to_send[0], route, sizeof(route));

    for (i = 0; i < len && i < DATA_LENGTH; i += 1) {
        strncpy(to_send[i + 1], data[i], sizeof(data[i]));
    }

    char *str = malloc(((DATA_LENGTH + 1) * DATA_SIZE) * sizeof(char));
    int z;
    for (i = 0; i < DATA_LENGTH + 1; i += 1) {
        for (z = 0; z < DATA_SIZE; z += 1) {
            str[(i * DATA_SIZE) + z] = to_send[i][z] != '\0'? to_send[i][z]: ' ';
        }
    }

    return str;
}