#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON.c"
#include <dirent.h>
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
void readFile(FILE*,char buffer[]);
//-------Functions---------------
char* makeMessageJSON(char* type, char* content) {
    cJSON *messageType = NULL;
    cJSON *messageContent = NULL;
    cJSON *thisMessage = cJSON_CreateObject();
    messageType = cJSON_CreateString(type);
    messageContent = cJSON_CreateString(content);
    cJSON_AddItemToObject(thisMessage, "type", messageType);
    cJSON_AddItemToObject(thisMessage, "content", messageContent);
    char* buffer = calloc(500,1);
    buffer = cJSON_Print(thisMessage);
    //cJSON_Delete(thisMessage);
    free(thisMessage);
    free(messageContent);
    free(messageType);
    return buffer;
}
//-------------------------------
void insertMessageInChannel(char* Sender, char* Content, cJSON* messages) {
    cJSON* thisMessage = cJSON_CreateObject();
    cJSON* sender = NULL;
    cJSON* content = NULL;
    sender = cJSON_CreateString(Sender);
    content = cJSON_CreateString(Content);
    cJSON_AddItemToObject(thisMessage, "sender", sender);
    cJSON_AddItemToObject(thisMessage, "content", content);
    cJSON_AddItemToArray(messages, thisMessage);
}
//-------------------------------
char* makeChannelJSON(char* Name) {
    cJSON *name = NULL;
    cJSON* messages = NULL;
    cJSON* thisMessage = NULL;
    cJSON *thisChannel = cJSON_CreateObject();
    name = cJSON_CreateString(Name);
    messages = cJSON_CreateArray();
    messages = cJSON_AddArrayToObject(thisChannel, "messages");
    insertMessageInChannel("Server", "Channel Created", messages);
    cJSON_AddItemToObject(thisChannel, "name", name);
    char* buffer = calloc(500,1);
    buffer = cJSON_Print(thisChannel);
    //Freeing memory
    free(thisChannel);
    free(name);
    free(messages);
    free(thisMessage);
    return buffer;
}
//-------------------------------
char* makeUserJSON(char* username, char* password) {
    cJSON *name = NULL;
    cJSON *pass = NULL;
    cJSON *thisUser = cJSON_CreateObject();
    name = cJSON_CreateString(username);
    pass = cJSON_CreateString(password);
    cJSON_AddItemToObject(thisUser, "username", name);
    cJSON_AddItemToObject(thisUser, "password", pass);
    char* buffer = calloc(300,1);
    buffer = cJSON_Print(thisUser);
    free(name);
    free(pass);
    free(thisUser);
    return buffer;
}
//-------------------------------
int checkUsername_register(char* username) {
    int flag = 0;
    struct dirent *thisFile;
    DIR *dir = opendir("Resources/Users");
    strcat(username,".user.json");
    while ((thisFile = readdir(dir)) != NULL && flag == 0)
    {
        if(strcmp(username, thisFile->d_name) == 0)
            flag = 1;
    }
    closedir(dir);
    free(thisFile);
    return flag; //flag = 1 => Error , flag = 0 => OK
}
//-------------------------------
int checkUsername_login(char* username, char* password) {
    FILE* fptr;
    char address[120];
    int flag;
    memset(address,0,sizeof(address));
    strcat(address, "Resources/Users/");
    strcat(address, username);
    strcat(address, ".user.json");
    if((fptr = fopen(address,"r")) == NULL)
        flag = 0; //username is not valid
    else
    {
        char buffer[300];
        readFile(fptr,buffer);
        fclose(fptr);
        cJSON* thisUser = cJSON_Parse(buffer);
        cJSON* thisPass = cJSON_GetObjectItemCaseSensitive(thisUser,"password");
        if(strcasecmp(password, thisPass->valuestring) == 0) //check if the password is correct
        {
            //check if the user is already looged in
            for(int i = 0 ; i < onlineUsers ; i++)
                if(strcasecmp(allUsers[i].username, username) == 0)
                {
                    flag = 2; //online!
                    free(thisUser);
                    free(thisPass);
                    return flag;
                }
            flag = 1; //successful
        }
        else
            flag = -1; //wrong password
        free(thisUser);
        free(thisPass);
    }
    return flag;
}
//-------------------------------
char* createUser(char* username, char* password) { //Creates the user file and sends successful message
    FILE* fptr;
    char address[120];
    char* thisMessage;
    memset(address,0,sizeof(address));
    strcat(address, "Resources/Users/");
    strcat(address, username);
    if((fptr = fopen(address,"a")) == NULL)
    {
        thisMessage = makeMessageJSON("Error", "User Registration Failed");
    }else
    {
        char* thisUser = makeUserJSON(username,password);
        fprintf(fptr,"%s",thisUser);
        free(thisUser);
        fclose(fptr);
        thisMessage = makeMessageJSON("Successful","");
    }
    return thisMessage;
}
//-------------------------------
void readFile(FILE* ptr, char buffer[]) {
    char ch = fgetc(ptr);
    int i = 0;
    while(ch != EOF)
    {
        buffer[i] = ch;
        ch = fgetc(ptr);
        i++;
    }
}
//-------------------------------
void addOnlineUser(char* username, char* token) {
    allUsers[onlineUsers].read = 0;
    allUsers[onlineUsers].joined = 0;
    strcpy(allUsers[onlineUsers].channel,"");
    strcpy(allUsers[onlineUsers].username, username);
    strcpy(allUsers[onlineUsers].token, token);
    onlineUsers ++;
}
//-------------------------------
int findUserIndex(char* token) {
    int i;
    for(i = 0 ; i <onlineUsers ; i++)
    {
        if(strcasecmp(allUsers[i].token, token) == 0)
            return i;
    }
    return -1;
}
//-------------------------------
int checkToken(char* token) {
    int flag = 1;
    for(int i = 0 ; i < onlineUsers ; i++)
    {
        if(strcasecmp(allUsers[i].token, token) == 0)
        {
            if(allUsers[i].joined == 0)
                flag = 0;
            else
                flag = -1;
        }
    }
    return flag; //flag = 1 || -1 => Error , flag = 0 => OK
}
//-------------------------------
int checkChannelName_create(char* name) {
    int flag = 0;
    struct dirent *thisFile;
    char fullName[95]={};
    strcpy(fullName, name);
    DIR *dir = opendir("Resources/Channels");
    strcat(fullName,".channel.json");
    while ((thisFile = readdir(dir)) != NULL && flag == 0)
    {
        if(strcmp(fullName, thisFile->d_name) == 0)
            flag = 1;
    }
    closedir(dir);
    //free(thisFile);
    return flag; //flag = 1 => Error , flag = 0 => OK
}
//-------------------------------
int checkChannelName_join(char* name) {
    int flag = 1;
    struct dirent *thisFile;
    char fullName[95]={};
    strcpy(fullName, name);
    DIR *dir = opendir("Resources/Channels");
    strcat(fullName,".channel.json");
    while ((thisFile = readdir(dir)) != NULL && flag == 1)
    {
        if(strcmp(fullName, thisFile->d_name) == 0)
            flag = 0;
    }
    closedir(dir);
    //free(thisFile);
    return flag; //flag = 1 => Error , flag = 0 => OK
}
//-------------------------------
char* createChannel(char* name, char* token) {
    FILE* fptr;
    char address[120];
    char* thisMessage;
    memset(address,0,sizeof(address));
    strcat(address, "Resources/Channels/");
    strcat(address, name);
    strcat(address, ".channel.json");
    if((fptr = fopen(address,"a")) == NULL)
    {
        thisMessage = makeMessageJSON("Error", "Channel creation failed");
    }else
    {
        char* thisChannel = makeChannelJSON(name);
        fprintf(fptr,"%s",thisChannel);
        free(thisChannel);
        fclose(fptr);
        //changing user properties
        int index = findUserIndex(token);
        allUsers[index].joined = 1;
        strcpy(allUsers[index].channel, name);
        allUsers[index].read = 0;
        //sending message
        thisMessage = makeMessageJSON("Successful","");
    }
    return thisMessage;
}
//-------------------------------
int getFileSize(FILE* file) {
    int count = 1;
    char ch = fgetc(file);
    while(ch != EOF)
    {
        count ++;
        ch = fgetc(file);
    }
    rewind(file);
    return count;
}
//-------------------------------
char* getChannel(char* name) {
    char address[120]={};
    strcat(address, "Resources/Channels/");
    strcat(address, name);
    strcat(address, ".channel.json");
    FILE* fptr;
    if((fptr = fopen(address, "r")) == NULL)
        printf("Error");
    int size = getFileSize(fptr);
    printf("Size = %d\n", size);
    char* buffer= calloc(size,1);
    //memset(buffer, 0, sizeof(buffer));
    readFile(fptr, buffer);
    fclose(fptr);
    return buffer;
}
//-------------------------------
char* refreshChannel(char* newMessage, int senderIndex, int server) {
    /*char address[120]={};
    strcat(address, "Resources/Channels/");
    strcat(address, allUsers[senderIndex].channel);
    strcat(address, ".channel.json");
    FILE* fptr;
    if((fptr = fopen(address, "r")) == NULL)
        printf("Error");
    int size = getFileSize(fptr);
    printf("Size = %d\n", size);
    char buffer[size];
    memset(buffer, 0, sizeof(buffer));
    readFile(fptr, buffer);
    fclose(fptr);*/
    char address[120]={};
    strcat(address, "Resources/Channels/");
    strcat(address, allUsers[senderIndex].channel);
    strcat(address, ".channel.json");
    FILE* fptr;
    if((fptr = fopen(address, "r")) == NULL)
        printf("Error");
    int size = getFileSize(fptr);
    char* buffer;
    buffer = getChannel(allUsers[senderIndex].channel);
    cJSON* channel = cJSON_Parse(buffer);
    cJSON* messages = cJSON_GetObjectItem(channel, "messages");
    if(server)
        insertMessageInChannel("Server",newMessage,messages);
    else
        insertMessageInChannel(allUsers[senderIndex].username, newMessage, messages);
    size += 90;
    char* newBuffer = calloc(size, 1);
    newBuffer = cJSON_Print(channel);
    fptr = fopen(address, "w");
    fputs(newBuffer, fptr);
    fclose(fptr);
    free(newBuffer);
    free(buffer);
    free(channel);
    free(messages);
    char* message = makeMessageJSON("Successful","");
    return message;
}
//-------------------------------
char* refreshMessages(int userIndex) {
    //int size = getFileSize(fptr);
    char* buffer;
    buffer = getChannel(allUsers[userIndex].channel);
    cJSON* channel = cJSON_Parse(buffer);
    cJSON* messages = cJSON_GetObjectItem(channel, "messages");
    cJSON* newList = cJSON_CreateObject();
    cJSON* unreadMessages = cJSON_CreateArray();
    cJSON* type = cJSON_CreateString("List");
    cJSON_AddItemToObject(newList,"type",type);
    unreadMessages = cJSON_AddArrayToObject(newList, "content");
    int numMessages = cJSON_GetArraySize(messages);
    free(buffer);
    for(int i = allUsers[userIndex].read ; i < numMessages ; i++)
        cJSON_AddItemReferenceToArray(unreadMessages, cJSON_GetArrayItem(messages,i));
    allUsers[userIndex].read = numMessages;
    buffer = cJSON_Print(newList);
    //freeing memory
    free(channel);
    free(messages);
    free(unreadMessages);
    free(newList);
    free(type);
    return buffer;
}
//-------------------------------
char* addUserToChannel(char* channel, char* token) {
    int index = findUserIndex(token);
    char* message;
    //change user properties
    allUsers[index].joined = 1;
    strcpy(allUsers[index].channel, channel);
    allUsers[index].read = 0;
    message = makeMessageJSON("Successful","");
    //send message in channel
    char joinMessage[100]={};
    strcat(joinMessage, allUsers[index].username);
    strcat(joinMessage, " joined channel!");
    refreshChannel(joinMessage, index, 1);
    return message;
}
//-------------------------------
char* removeUserFromChannel(int userIndex) {
    //sending left message
    char leftMessage[95]={};
    strcat(leftMessage, allUsers[userIndex].username);
    strcat(leftMessage, " left channel");
    refreshChannel(leftMessage, userIndex, 1);
    //setting user properties
    memset(allUsers[userIndex].channel, 0,80);
    allUsers[userIndex].read = 0;
    allUsers[userIndex].joined = 0;
    char* buffer = makeMessageJSON("Successful","");
    return buffer;
}
//-------------------------------
char* findChannelMembers(int userIndex) {
    char channelName[80]={};
    strcpy(channelName, allUsers[userIndex].channel);
    cJSON* newList = cJSON_CreateObject();
    cJSON* type = cJSON_CreateString("List");
    cJSON* members = cJSON_CreateArray();
    cJSON_AddItemToObject(newList, "type", type);
    members = cJSON_AddArrayToObject(newList, "content");
    for(int i = 0 ; i < onlineUsers ; i++)
    {
        if(strcasecmp(allUsers[i].channel, channelName) == 0)
        {
            cJSON* thisMember = cJSON_CreateString(allUsers[i].username);
            cJSON_AddItemToArray(members,thisMember);
        }
    }
    char* buffer = cJSON_Print(newList);
    return buffer;
}
//-------------------------------
char* logout(int index) {
    /*for(int i = index; i < onlineUsers ; i++)
    {
        printf("i: %d, onlineUsers: %d",i,onlineUsers);
        if(i == onlineUsers-1)
        {
            printf("begin if\n");
            memset(allUsers[i].token, 0, 35);
            memset(allUsers[i].channel, 0, 80);
            memset(allUsers[i].username, 0, 80);
            allUsers[i].read = 0;
            allUsers[i].joined = 0;
            printf("end if\n");
        }else
        {
            printf("begin else\n");
            strcpy(allUsers[i].token, allUsers[i+1].token);
            strcpy(allUsers[i].channel, allUsers[i+1].channel);
            strcpy(allUsers[i].username, allUsers[i+1].username);
            allUsers[i].read = allUsers[i+1].read;
            allUsers[i].joined = allUsers[i+1].joined;
            printf("end else\n");
        }
    }*/
    allUsers[index] = allUsers[onlineUsers-1];
    memset(allUsers[onlineUsers-1].token, 0, 35);
    memset(allUsers[onlineUsers-1].channel, 0, 80);
    memset(allUsers[onlineUsers-1].username, 0, 80);
    allUsers[onlineUsers-1].read = 0;
    allUsers[onlineUsers-1].joined = 0;
    onlineUsers --;
    /*printf("Online Users:\n");
    for(int i = 0 ; i <onlineUsers ; i++)
        printf("%s\n",allUsers[i].username);*/
    char* buffer = makeMessageJSON("Successful","");
    return buffer;
}
//-------------------------------
