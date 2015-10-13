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
#include <stdarg.h>
#include "connection.h"

static const int DATA_LENGTH = 1000;

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
    int n = 0, nb_words;
    char server_reply[DATA_LENGTH];

    while(control->keep_alive)
    {
        memset(server_reply, 0, DATA_LENGTH);

        //Receive a reply from the server
        if((n = recv(control->conn->_sock , server_reply , DATA_LENGTH , 0)) <= 0)
        {
            char **data = _get_words(server_reply, &nb_words, ",");
            Event_run(data, nb_words);
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
    Connection_send(_prepare_msg(3, "login", username, password));
}

char *_prepare_msg(int len, ...) {
    int i, z, t = 0;
    char *msg = malloc(DATA_LENGTH * sizeof(char));
    va_list args;
    va_start( args, len );

    for (i = 0; i < len; i++) {
        char *arg = va_arg ( args, char* );
        for (z = 0; z < strlen(arg); z++) {
            msg[t] = arg[z];
            t++;
            if (i != len - 1 && z == strlen(arg) - 1) {
                msg[t] = ',';
                t++;
            }
        }
    }

    va_end( args );

    return msg;
}