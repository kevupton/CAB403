//
// Created by Kevin on 10/10/2015.
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include "control.h"


const int MAX_PLAYERS = 10;

Control *newControl(char *argv[]) {
    Control *c = malloc(sizeof(Control));
    c->conn = newConnection(argv[1]);
    c->exit_signal = 0;

    if (c->conn != NULL) {
        c->users = newList(sizeof(User *));
        c->instances = newList(sizeof(Instance*));
        c->auth = newList((2*sizeof(char *)));
        c->words = newList(2*sizeof(char*));
        c->leaderWorker = newLeaderWorker();

        _boot(c);
    }

    return c;
}

void _boot(Control *control) {
    int i;
    Instance *instance;

    for (i = 0; i < MAX_PLAYERS; i++) {
        instance = newInstance();
        instance->_thread_index = i;
        List_add(control->instances, instance);
    }

    _start_worker(control);
    _load_authentication(control);
    _load_words(control);
}

void _start_worker(Control *control) {
    //Accept and incoming connection
    puts("Waiting for incoming connections...\n");

    int *new_sock;
    pthread_t thread;

    if( pthread_create( &thread, NULL ,  Connection_listen , control->conn) < 0)
    {
        perror("could not create thread");
        return;
    }

    control->_control_thread = thread;
}

void _signal_catcher(int sig) {
    control->exit_signal = 1;
}

void Controller_run() {
    signal(SIGINT, _signal_catcher);
    while (!control->exit_signal) {sleep(1);}
    puts("\nExitting the application...");
    Close_connections();
}

void _load_authentication(Control *control) {
    FILE *fp;
    char *fuser, *fpass, buf[100];

    fp = fopen("Authentication.txt", "r");
    if (fp == NULL) {
        puts("Unable to read Auth file");
        exit(EXIT_FAILURE);
    }

    void **pair;

    fgets(buf, sizeof buf, fp);
    while (fgets(buf, sizeof buf, fp) != NULL) {
        fuser = malloc(10 * sizeof(char));
        fpass = malloc(10 * sizeof(char));

        pair = malloc(2*sizeof(char*));
        sscanf(buf, "%s %s", fuser, fpass);

        pair[0] = fuser;
        pair[1] = fpass;

        List_add(control->auth, pair);
    }

    fclose(fp);
}

void _load_words(Control *control) {
    FILE *fp;
    char **words, buf[100];
    int nb_words;

    fp = fopen("hangman_text.txt", "r");
    if (fp == NULL) {
        puts("Unable to read hangman_text.txt file");
        exit(EXIT_FAILURE);
    }

    void **pair;

    fgets(buf, sizeof buf, fp);
    while (fgets(buf, sizeof buf, fp) != NULL) {
        pair = malloc(2*sizeof(char*));
        words = _get_words(buf, &nb_words, ",\n");

        pair[0] = words[0];
        pair[1] = words[1];

        List_add(control->words, pair);
    }

    fclose(fp);
}


void Close_connections() {
    int i;
    Instance *in;

    LeaderWorker_close();

    for (i = 0; i < control->instances->count; i++) {
        in = List_get(control->instances, i);
        in->keep_alive = 0;
        close(in->_sock);
        Instance_wake(in);
        pthread_join(in->_thread, NULL);
    }

    close(control->conn->_sock);
    pthread_join(control->_control_thread, NULL);
    pthread_join(control->leaderWorker->_thread, NULL);

    puts("\nGoodbye.\n");
}
