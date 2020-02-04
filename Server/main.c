#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>
#include "connection.h"
#include "messages.h"
#include "files.h"
#define PORT 12345
#define SA struct sockaddr
//-------Variables---------------
int server_socket, client_socket;
struct sockaddr_in server, client;
struct user{
    char token[35];
    char username[80];
    char channel[80];
    int unread;
    int joined;
};
struct user allUsers[100];
int onlineUsers = 0;
//-------Functions---------------
int main() {
    while(1)
    {
        createSocket();
        char* buffer = receive(client_socket);
        puts("\tReceived:\n\t");
        puts(buffer);
        char* response = checkMessage(buffer);
        puts("\tSent:\n\t");
        puts(response);
        sendResponse(response);
        //sendMessage(respons);
        free(buffer);
        free(response);
        closesocket(server_socket);
        //checkMessage(buffer);
    }
    return 0;
}
//-------------------------------
