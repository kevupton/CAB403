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

Connection *newConnection(char *port) {
    Connection *conn = malloc(sizeof(Connection));

    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    conn->port = atoi(port);

    if (conn->port == 0) {
        puts("Invalid Port.");
        return NULL;
    }

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( conn->port );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return NULL;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  Connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return NULL;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return NULL;
    }

    return conn;
}

/*
 * This will handle connection for each client
 * */
void *Connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size, size = (DATA_LENGTH + 1) * DATA_SIZE;
    char client_message[60];

    //Send some messages to the client
//    message = "\nGreetings! I am your connection handler\n";
//    write(sock , message , strlen(message));
//
//    message = "Now type something and i shall repeat what you type \n";
//    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , size , 0)) > 0 )
    {
        char **data = _read_msg(client_message, size);
        int len = size/DATA_SIZE;
        Event_run(data, len);

        //Send the message back to client
        //write(sock , client_message , strlen(client_message));
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}

int Connection_initialise(Connection *c) {


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

char **_read_msg(char *msg, int len) {

    int i, nb = len/DATA_SIZE, z, y;
    char **arr = malloc(nb * sizeof(char*));

    for (i = 0; i < nb; i += 1) {
        y = i * DATA_SIZE;
        arr[i] = malloc(DATA_SIZE * sizeof(char));
        for (z = 0; z < DATA_SIZE && (z + y) < len; z += 1) {
            arr[i][z] = msg[z + y];
        }

        for (z = DATA_SIZE - 1; z >= 0; z -= 1) {
            if (arr[i][z] == ' ' || arr[i][z] == '\n' || arr[i][z] == '\t') {
                arr[i][z] = '\0';
            } else {
                break;
            }
        }
    }

    return arr;
}

int Connection_send(Connection *c, char *msg) {
    //Send some data
    return send(c->_sock , msg , strlen(msg) , 0);
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