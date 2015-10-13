//
// Created by Kevin on 12/10/2015.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H

typedef struct User {
    char *username;
    int won;
    int played;
} User;

User *newUser(char *username);
User *User_find(char *username);
User *User_login(char *username);

#endif //SERVER_USER_H
