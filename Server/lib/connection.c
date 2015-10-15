//
// Created by Kevin on 10/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <stdarg.h>
#include "connection.h"

const int DATA_LENGTH = 1000;

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
                char **data = _get_words(client_message, &nb_words, ",");
                Event_run(instance, data, nb_words);

                memset(client_message, 0, DATA_LENGTH);
            }
            if(read_size == 0)
            {
                fflush(stdout);
            }
            printf("Client disconnected - Freeing thread %d\n", instance->_thread_index);
            Instance_reset(instance);
        }
        if (instance->keep_alive) sleep(1);
    }
}

void *Connection_listen(void *connection) {
    Connection *conn = connection;
    int client_sock , c, val;
    struct sockaddr_in client;

    socklen_t len = sizeof(val);

    c = sizeof(struct sockaddr_in);

    while((client_sock = accept(conn->_sock, (struct sockaddr *)&client, (socklen_t*)&c)) != -1)
    {
        puts("Client attempting connect...");
        Instance *i = Instance_get_available();
        if (i != NULL) {
            i->_sock = client_sock;
            i->in_use = 1;
            Connection_write(client_sock, _prepare_msg(3, "connect", "1", ""));
        } else {
            Connection_write(client_sock, _prepare_msg(3, "connect", "0", "Too many users"));
            puts("Out of threads. Denying entry.");
        }
    }

    puts("Connection closed.");
}

void Connection_write(int sock, char *msg) {
    write(sock , msg , strlen(msg));
    free(msg);
}

char *_prepare_msg(int len, ...) {
    int i;
    char **data = malloc(len * sizeof(char *));

    va_list args;
    va_start( args, len );

    for (i = 0; i < len; i++) {
        data[i] = va_arg ( args, char* );
    }

    va_end( args );

    char *msg = _concat_array(len, data, ",");

    free(data);

    return msg;
}

char *_concat_array(int len, char **data, char *join) {
    int i, y, z, t = 0, str_len_i, len_join = (int) strlen(join);

    char *msg = malloc(DATA_LENGTH * sizeof(char));
    memset(msg, 0, DATA_LENGTH);

    for (i = 0; i < len; i++) {
        str_len_i = (int) strlen(data[i]);
        for (z = 0; z < str_len_i; z++) {
            if (data[i][z] == '\0') break;

            msg[t] = data[i][z];
            t++;
            if (i != len - 1 && z == str_len_i - 1) {
                for (y = 0; y < len_join; y++) {
                    msg[t] = join[y];
                    t++;
                }
            }
        }
    }

    return msg;
}