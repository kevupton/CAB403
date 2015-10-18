//
// Created by Kevin on 10/10/2015.
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include "control.h"

const int MAX_PLAYERS = 10;
const char *AUTHENTICATION_FILE = "Authentication.txt";
const char *HANGMAN_WORDS_FILE = "hangman_text.txt";

/**
 * Instantiates a new Controller object
 *
 * @param argv the input arguments from the console application
 *
 * @return Control* the new control
 */
Control *newControl(char *argv[]) {
    Control *c = malloc(sizeof(Control));
    c->conn = newConnection(argv[1]);
    c->exit_signal = 0;

    if (c->conn != NULL) { //if the connection exists.
        c->users = newList(sizeof(User *));
        c->instances = newList(sizeof(Instance*));
        c->auth = newList((2*sizeof(char *)));
        c->words = newList(2*sizeof(char*));
        c->leaderWorker = newLeaderWorker(); //the leaderboard results worker

        _boot(c); //boot the application
    }

    return c;
}

/**
 *  Boots the application and all of the workers.
 *
 *  @param control the instance of the controller
 */
void _boot(Control *control) {
    int i;
    Instance *instance;

    //instantiate the thread pool for the defined number of users
    for (i = 0; i < MAX_PLAYERS; i++) {
        instance = newInstance(); //create the new instance
        instance->_thread_index = i; //give the instance its index
        List_add(control->instances, instance); //add it to the list of instances
    }

    //load the application specific functionality
    _start_worker(control);
    _load_authentication(control);
    _load_words(control);
}

/**
 * Creates the thread which handles the incoming connections.
 *
 * @param control the instance of the control
 */
void _start_worker(Control *control) {
    //Accept and incoming connection
    puts("Waiting for incoming connections...\n");

    if( pthread_create( &control->_control_thread, NULL ,  Connection_listen , control->conn) < 0)
    { //attempt to start the connection listener
        perror("could not create connection listener");
        return;
    }
}

/**
 * The method for catching the ctrl + c handler.
 *
 * @param sig
 */
void _signal_catcher(int sig) {
    control->exit_signal = 1;
}

/**
 * The main controller method responsible for keeping the application from exitting.
 */
void Controller_run() {
    signal(SIGINT, _signal_catcher);
    while (!control->exit_signal) {sleep(1);} //wait for the ctrl + c signal  TODO : change to semaphore
    puts("\nExitting the application...");
    Close_connections();
}

/**
 * Load the authentication text file into memory
 *
 * @param control the instance of the controller
 */
void _load_authentication(Control *control) {
    FILE *fp;
    char *fuser, *fpass, buf[100];

    //attempt to load the text file
    fp = fopen(AUTHENTICATION_FILE, "r");
    if (fp == NULL) {
        puts("Unable to read Auth file");
        exit(EXIT_FAILURE);
    }

    void **pair;

    fgets(buf, sizeof buf, fp);
    while (fgets(buf, sizeof buf, fp) != NULL) { //get each line
        //allocate the memory for the user pass
        fuser = malloc(10 * sizeof(char));
        fpass = malloc(10 * sizeof(char));

        pair = malloc(2*sizeof(char*)); //the container
        sscanf(buf, "%s %s", fuser, fpass);

        pair[0] = fuser;
        pair[1] = fpass;

        List_add(control->auth, pair); //add the container to the list
    }

    //close connection
    fclose(fp);
}

/**
 * Load the hangman words into memory from the hangma_text file
 *
 * @param control the instnace of the controller
 */
void _load_words(Control *control) {
    FILE *fp;
    char **words, buf[100];
    int nb_words;

    //attempt to open the hangman file
    fp = fopen(HANGMAN_WORDS_FILE, "r");
    if (fp == NULL) {
        puts("Unable to read hangman_text.txt file");
        exit(EXIT_FAILURE);
    }

    void **pair;

    fgets(buf, sizeof buf, fp);
    while (fgets(buf, sizeof buf, fp) != NULL) { //for each line of the file
        //allocate memory
        pair = malloc(2*sizeof(char*));
        words = _get_words(buf, &nb_words, ",\n"); //split the word

        pair[0] = words[0];
        pair[1] = words[1];

        List_add(control->words, pair); //add to list
    }

    fclose(fp);
}

/**
 * Close the server down gracefully.
 * Closes all of the connections and joins all the threads.
 */
void Close_connections() {
    int i;
    Instance *in;

    LeaderWorker_close(); //stop leaderworker

    for (i = 0; i < control->instances->count; i++) { //foreach instance
        in = List_get(control->instances, i);
        in->keep_alive = 0;
        close(in->_sock);
        Instance_wake(in);
        pthread_join(in->_thread, NULL);
        //close conn and join thread.
    }

    //close accepter connection and join remaining threads.
    close(control->conn->_sock);
    pthread_join(control->_control_thread, NULL);
    pthread_join(control->leaderWorker->_thread, NULL);

    puts("\nGoodbye.\n");
}
