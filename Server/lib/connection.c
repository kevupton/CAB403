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

    struct sockaddr_in server ;

    conn->port = atoi(port);

    if (conn->port == 0) {
        puts("Invalid Port.");
        return NULL;
    }

    //Create socket
    conn->_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (conn->_sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( conn->port );

    //Bind
    if( bind(conn->_sock,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return NULL;
    }
    puts("bind done");

    //Listen
    listen(conn->_sock , 3);

    return conn;
}

/*
 * This will handle connection for each client
 * */
void *Connection_handler(void *i)
{
    //Get the socket descriptor
    int read_size, nb_words;
    char client_message[DATA_LENGTH];
    Instance *instance = i;

    while (instance->keep_alive) {
        if (instance->in_use) {
            //Receive a message from client
            printf("Client connected to thread %d\n", List_index(control->instances, instance));
            while((read_size = recv(instance->_sock , client_message , DATA_LENGTH , 0)) > 0)
            {
                char **data = _get_words(client_message, &nb_words);
                Event_run(instance, data, nb_words);

                memset(client_message, 0, DATA_LENGTH);
            }
            Instance_reset(instance);

            if(read_size == 0)
            {
                puts("Client disconnected");
                fflush(stdout);
            }
            else if(read_size == -1)
            {
                perror("recv failed");
            }
            puts("Connection Closed");
        }
        sleep(1);
    }
}

void *Connection_listen(void *connection) {
    Connection *conn = connection;
    int client_sock , c;
    struct sockaddr_in client;

    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(conn->_sock, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        Instance *i = Instance_get_available();
        if (i != NULL) {
            i->_sock = client_sock;
            i->in_use = 1;
            Connection_write(client_sock, _prepare_msg(2, "connect", "1"));
        } else {
            Connection_write(client_sock, _prepare_msg(3, "connect", "0", "Too many users"));
        }
    }

    if (client_sock < 0)
    {
        perror("accept failed");
    }
}

int Connection_initialise(Connection *c) {


    return 1;
}

void Connection_close(Connection *c) {
    //close(c->_sock);
}

//char **_read_msg(char *msg, int len) {
//    int i, nb = len/DATA_SIZE, z, y;
//    char **arr = malloc(nb * sizeof(char*));
//
//    for (i = 0; i < nb; i += 1) {
//        y = i * DATA_SIZE;
//        arr[i] = malloc(DATA_SIZE * sizeof(char));
//        for (z = 0; z < DATA_SIZE && (z + y) < len; z += 1) {
//            arr[i][z] = msg[z + y];
//        }
//
//        for (z = DATA_SIZE - 1; z >= 0; z -= 1) {
//            if (arr[i][z] == ' ' || arr[i][z] == '\n' || arr[i][z] == '\t') {
//                arr[i][z] = '\0';
//            } else {
//                break;
//            }
//        }
//    }
//
//    return arr;
//}

int Connection_write(int sock, char *msg) {
    return write(sock , msg , strlen(msg));
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