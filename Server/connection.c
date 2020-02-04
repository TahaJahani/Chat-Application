#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define PORT 12345
#define SA struct sockaddr
//-------Variables---------------
extern int server_socket, client_socket;
extern struct sockaddr_in server, client;
//-------Functions---------------
char* receive(int client_socket) {
    //char code[20],username[80],password[80];
    //char tmp[2];
    char buffer[300];
    char* message = calloc(1,sizeof(buffer));
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    for(int i = 0 ; i < sizeof(buffer); i++)
        message[i] = buffer[i];
    //sscanf(buffer,"%s %[^,]%s%s %[^\n]",code,username,tmp,password);
    //printf("%s: %s and %s",code,username,password);
    return message;
}
//-------------------------------
void sendResponse(char* response) {
    int i= 0;
    for(i = 0 ; response[i] != '\0' ; i++);
        //printf("%c",respons[i]);
    //printf("Size: %d\n",i);
    //printf("Check sent: %d",send(client_socket, respons, i, 0));
    send(client_socket, response, i, 0);
}
//-------------------------------
void createSocket() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
    {
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    }else{}
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");

    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else{}
        printf("Server listening..\n");

    // Accept the data packet from client and verify
    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n\n");
        exit(0);
    }
    else{}
        printf("Server acccepted the client..\n\n");
}
//-------------------------------
