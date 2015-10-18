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

/**
 * Initialises the Connection class
 *
 * @param port the string port to assign the Connection to
 *
 * @return Connection* the instance of the Connection
 */
Connection *newConnection(char *port) {
    Connection *conn = malloc(sizeof(Connection));

    struct sockaddr_in server ;
    int setting = 1;
    conn->keep_alive = 1;
    conn->port = atoi(port);

    if (conn->port == 0) {
        puts("Invalid Port.");
        return NULL;
    }

    //Create socket
    conn->_sock = socket(AF_INET , SOCK_STREAM , 0);
    setsockopt(conn->_sock,SOL_SOCKET,SO_REUSEADDR,&setting,sizeof(int));
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
    printf("Bound to port: %d\n", conn->port);

    //Listen
    listen(conn->_sock , 3);

    return conn;
}

/**
 * Is the pthread method assigned to each Instance.
 * Used to talk to the specific client, and handle each of their incoming requests.
 * Is sleeping when there is nobody connected, and resets itself when someone disconnects.
 *
 * @param void
 *
 * @return void*
 */
void *Connection_handler(void *i)
{
    //Get the socket descriptor
    int read_size, nb_words;
    char client_message[DATA_LENGTH];
    Instance *instance = i;
    fd_set rfds;
    int retval;
    int error = 0;
    socklen_t len = sizeof (error);
    char **data;

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    //do while the instance is told to keep going
    while (instance->keep_alive) {
        if (instance->in_use) {
            FD_ZERO(&rfds);
            FD_SET(instance->_sock, &rfds);

            //check to see if there is any information to receive
            read_size = 0;
            retval = select(instance->_sock + 1, &rfds, NULL, NULL, &timeout);
            if (retval > 0) {  //if there is information to be received
                //get the information
                read_size = recv(instance->_sock , client_message , DATA_LENGTH , 0);
                if (read_size > 0) { //if there was content
                    data = _get_words(client_message, &nb_words, ",");
                    Event_run(instance, data, nb_words); //run the associated event
                    memset(client_message, 0, DATA_LENGTH);
                }
            }
            if (retval == -1 || (retval > 0 && read_size <= 0)) { //if there was a problem with the data received then reset
                fflush(stdout);
                printf("Client disconnected - Freeing thread %d\n", instance->_thread_index);
                Instance_reset(instance);
            }
        } else {
            //if the instance is supposed to keep running, then put it to sleep,
            //and wait for a connection
            if (instance->keep_alive) {
                Instance_sleep(instance);
            };
        }
    }
    if (instance->in_use) { //free the memory
        fflush(stdout);
        printf("Client disconnected - Freeing thread %d\n", instance->_thread_index);
        Instance_reset(instance);
    }
}

/**
 * Is the pthread method assigned to the Control worker.
 * Used to handle the incoming connections, and assign each connection to a thread
 * If it is available.
 *
 * @param void
 *
 * @return void*
 */
void *Connection_listen(void *connection) {
    Connection *conn = connection;
    int client_sock , c, val, retval = 0, error = 0;
    struct sockaddr_in client;
    struct timeval timeout;
    fd_set rfds;
    socklen_t err_len = sizeof(error);
    socklen_t len = sizeof(val);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    c = sizeof(struct sockaddr_in);

    while (retval != -1) { //while there is a connection there
        FD_ZERO(&rfds);
        FD_SET(conn->_sock, &rfds);

        //check if there is data to be received
        retval = select(conn->_sock + 1, &rfds, NULL, NULL, &timeout);
        if (retval > 0) { //if there is data
            //get the socket connection
            client_sock = accept(conn->_sock, (struct sockaddr *)&client, (socklen_t*)&c);
            if (client_sock != -1) { //if the socket connection exists
                puts("Client attempting connect...");
                //get the next available instance to assign
                Instance *i = Instance_get_available();
                if (i != NULL) { //if there is an available instance
                    Instance_assign(i, client_sock); //assign it to the current connection
                    Connection_write(client_sock, _prepare_msg(3, "connect", "1", ""));
                    printf("Client connected to thread %d\n", i->_thread_index);
                } else { //else tell the user that there are no available connections
                    Connection_write(client_sock, _prepare_msg(3, "connect", "0", "Too many users"));
                    puts("Out of threads. Denying entry.");
                }
            } else { //else stop the thread.
                break;
            }
        }
    }

    puts("Connection closed.");
}

/**
 * Writes the client attached to the socket, a specific message
 *
 * @param msg the message to send.
 */
void Connection_write(int sock, char *msg) {
    write(sock , msg , strlen(msg));
    free(msg); //free the message being sent
}

/**
 * Prepares a list of strings to be sent to client.
 * Concatenates them with a comma and returns the result.
 *
 * @param len the number of strings in the arguments
 * @param ... a variable number strings.
 *
 * @return char* returns the concatenated string.
 */
char *_prepare_msg(int len, ...) {
    int i;
    char **data = malloc(len * sizeof(char *));

    va_list args;
    va_start( args, len );

    //create the array of strings.
    for (i = 0; i < len; i++) {
        data[i] = va_arg ( args, char* );
    }

    va_end( args );

    char *msg = _concat_array(len, data, ",");

    free(data);

    return msg;
}

/**
 * Concatenate the specific array of strings.
 *
 * @param len the length of the array
 * @param data the array of strings
 * @param join the string to join the array
 *
 * @return the resulting string.
 */
char *_concat_array(int len, char **data, char *join) {
    int i, y, z, t = 0, str_len_i, len_join = (int) strlen(join);

    char *msg = malloc(DATA_LENGTH * sizeof(char));
    memset(msg, 0, DATA_LENGTH);

    //concatenate the strings
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