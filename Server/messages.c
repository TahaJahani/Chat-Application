#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "connection.h"
#include "files.h"
//-------Variables---------------
extern struct user{
    char token[35];
    char username[80];
    char channel[80];
    int read;
    int joined;
};
extern struct user allUsers[100];
extern int onlineUsers;
//-------Functions---------------
char* generateToken() {
    char* thisToken = calloc(31,1);
    srand(time(NULL));
    for(int i = 0 ; i < 30 ; i++)
    {
        int ch = rand();
        ch %= 75;
        ch += 48;
        if(ch == 58 || ch == 60 || ch == 62 || ch == 63 || ch == 92)
        {
            i--;
            continue;
        }
        thisToken[i] = ch;
    }
    thisToken[31] = "\0";
    return thisToken;
}
//-------------------------------
char* checkMessage(char* message) {
    char code[20],username[80],password[80],tmp[2],token[35],newMessage[300];
    /*puts("\n\tWHOLE MESSAGE: \n\t");
    puts(message);*/
    sscanf(message,"%s",code);
    char* response;
    if(strcmp(code,"register") == 0) {
        sscanf(message,"%s %[^,]%s %[^\n]",code,username,tmp,password);
        if(checkUsername_register(username) == 0) //if no error
        {
            response = createUser(username,password);
        }else
        {
            response = makeMessageJSON("Error","This username is not available");
        }
    }
    else if(strcmp(code, "login") == 0) {
        sscanf(message,"%s %[^,]%s %[^\n]",code,username,tmp,password);
        int flag = checkUsername_login(username,password);
        if(flag == 0)
            response = makeMessageJSON("Error", "Username is not valid");
        else if(flag == 1)
        {
            char* token = generateToken();
            response = makeMessageJSON("AuthToken",token);
            addOnlineUser(username,token);
            free(token);
        }
        else if(flag == -1)
            response = makeMessageJSON("Error","Wrong password");
        else if(flag == 2)
            response = makeMessageJSON("Error","This user is already logged in");

    }
    else if(strcmp(code, "create") == 0) {
        sscanf(message,"%s %s %[^,]%s %[^\n]",code,code,username,tmp,token);
        int flag = checkToken(token);
        if(flag == 0)
        {
            if(checkChannelName_create(username) == 0)
                response = createChannel(username, token);
            else
                response = makeMessageJSON("Error","Channel name is not available");
        }
        else if(flag == 1)
            response = makeMessageJSON("Error", "Token is invalid");
        else if(flag == -1)
            response = makeMessageJSON("Error", "User already joined in a channel");
    }
    else if(strcmp(code, "join") == 0) { /* needs to be cheked... */
        sscanf(message,"%s %s %[^,]%s %[^\n]",code,code,username,tmp,token);
        int flag = checkToken(token);
        if(flag == 0)
        {
            if(checkChannelName_join(username) == 0)
                response = addUserToChannel(username, token);
            else
                response = makeMessageJSON("Error","Channel name is invalid");
        }
        else if(flag == 1)
            response = makeMessageJSON("Error", "Token is invalid");
        else if(flag == -1)
            response = makeMessageJSON("Error", "User already joined in a channel");
    }
    else if(strcmp(code, "send") == 0) {
        sscanf(message,"%s %[^,]%s %[^\n]",code,newMessage,tmp,token);
        int flag = checkToken(token);
        if(flag == 0)
            response = makeMessageJSON("Error", "User is not in a channel");
        else if(flag == 1)
            response = makeMessageJSON("Error", "Token is invalid");
        else if(flag == -1)
            response = refreshChannel(newMessage, findUserIndex(token), 0);
    }
    else if(strcmp(code, "refresh") == 0) {
        sscanf(message,"%s %s",code,token);
        int flag = checkToken(token);
        if(flag == 0)
            response = makeMessageJSON("Error", "User is not in a channel");
        else if(flag == 1)
            response = makeMessageJSON("Error", "Token is invalid");
        else
        {
            int index = findUserIndex(token);
            response = refreshMessages(index);
        }
    }
    else if(strcmp(code, "channel") == 0) {
        sscanf(message,"%s %s %s",code,code,token);
        int flag = checkToken(token);
        if(flag == 0)
            response = makeMessageJSON("Error", "User is not in a channel");
        else if(flag == 1)
            response = makeMessageJSON("Error", "Token is invalid");
        else
        {
            int index = findUserIndex(token);
            response = findChannelMembers(index);
        }
    }
    else if(strcmp(code, "leave") == 0) {
        sscanf(message,"%s %s",code,token);
        int flag = checkToken(token);
        if(flag == 0)
            response = makeMessageJSON("Error", "User is not in a channel");
        else if(flag == 1)
            response = makeMessageJSON("Error", "Token is invalid");
        else
        {
            int index = findUserIndex(token);
            response = removeUserFromChannel(index);
        }
    }
    else if(strcmp(code, "logout") == 0) {
        sscanf(message,"%s %s",code,token);
        int index = findUserIndex(token);
        response = logout(index);
        index = findUserIndex(token);
        printf("Index = %d\n",index);
    }
    return response;
}
//-------------------------------
