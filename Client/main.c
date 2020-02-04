#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <winsock2.h>
#include "cJSON.h"
#include "cJSON.c"
#define MAX 80
#define PORT 12345
#define IP "127.0.0.1"
//127.0.0.1
#define SA struct sockaddr
void delay(int);
void setColor(int);
int checkError(cJSON* , char);
void showAccountMenu();
//void getPass(int, char pass[20]);
void loginMenu();
void registerMenu();
void socketInitialize();
void userMenu();
void sendMenu();
void joinChannel();
void quit();
void sendMessage();
//char* fetch();
void leaveChannel();
void viewMembers();
void printItems(cJSON*,int);
void parse_object2(cJSON*);
//-----------------------
int client_socket, server_socket;
char option; //r: Register *** l: Login *** m: Main *** u: user menu *** c: create Channel *** j: join *** q: logout *** s: Channel
char token[MAX+1];
char username[23];
char password[21];
//-------------
int main(){
    //socketInitialize();
    option = 'm'; //main menu
    while(1)
    {
        switch (option)
        {
        case 'c':
            createChannel();
            break;
        case 'l':
            loginMenu();
            break;
        case 'u':
            userMenu();
            break;
        case 'q':
            quit();
            break;
        case 's':
            sendMenu();
            break;
        case 'j':
            joinChannel();
            break;
        case 'm':
            showAccountMenu();
            break;
        case 'r':
            registerMenu();
            break;
        case '1':
            sendMessage();
            break;
        case '2':
            viewMessages();
            break;
        case '3':
            viewMembers();
            break;
        case '4':
            leaveChannel();
            break;
        //default:
            //printf("Wrong Choice, Try Again");
            //system("cls");
        }
        system("cls");
    }
    return 0;
}
void socketInitialize(){
    //int client_socket, server_socket;
	struct sockaddr_in servaddr, cli;

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
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	//else
		//printf("Socket successfully created..\n");

	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	//servaddr.sin_addr.s_addr = inet_addr("192.168.1.7");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	//else
		//printf("Successfully connected to the server..\n");
}
void setColor(int num){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}
void delay(int number_of_seconds){
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds);
}
int checkError(cJSON* json, char operation){
    setColor(10);
    cJSON* Type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON* Content;
    char* content;
    if(operation != '2' && operation != '3')
    {
        Content = cJSON_GetObjectItemCaseSensitive(json, "content");
        content = Content -> valuestring;
    }
    char* type = Type -> valuestring;
    if(strcmp(type ,"Error") == 0)
    {
        printf("%s",content);
        return 0;
    }else
    {
        if(operation == 'l')
        {
            memset(token ,0, sizeof(token));
            strcat(token, content);
            //token = content;
            printf("Logged on successfully");
            return 1;
        }else if(operation == 'r')
        {
            printf("Welcome To Our App!");
            return 1;
        }else if(operation == 'c')
        {
            printf("Channel Created Successfully!");
            return 1;
        }else if(operation == 'j')
        {
            printf("Successfully Joined Channel!");
            return 1;
        }else if(operation =='4')
        {
            printf("Successfully Left Channel");
            return 1;
        }else if(operation =='3')
        {
            return 1;
        }else if(operation =='2')
        {
            return 1;
        }
    }
    setColor(7);
}
void loginMenu(){
    setColor(27);
    printf("***LOGIN MENU***\n");
    setColor(15);
    memset(username, 0,sizeof(username));
    memset(password, 0,sizeof(password));
    printf("Enter Your Username And Password:\n");
    scanf("%s",username);
    scanf("%s",password);
    //getPass(20 , password);
    //creating request
    char code[MAX]="login ";
    strcat(username,", ");
    strcat(code,username);
    strcat(code,password);
    strcat(code,"\n");
    //creating socket and sending message
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    //receiving response
    char buffer[MAX];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    if(checkError(response, 'l'))
        option = 'u';
    else
        option = 'm';
    delay(3);
    setColor(7);
    return;
}
void registerMenu(){
    //printf("\n\t");
    setColor(27);
    printf("***REGISTERATION MENU***\n");
    setColor(15);
    printf("Enter Username: ");
    scanf("%s",username);
    printf("\nEnter Password: ");
    scanf("%s",password);
    //getPass(20 , password);
    //creating message
    char code[MAX];
    strcat(username,", ");
    strcat(code,"register ");
    strcat(code,username);
    strcat(code,password);
    strcat(code,"\n");
    //sending request
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    //receiving response
    char buffer[MAX];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    checkError(response, 'r');
    //printf("\n\tUser Successfully Created!\n");
    option = 'm';
    delay(3);
    setColor(7);
    return;
}
void showAccountMenu(){
    setColor(124);
    printf("***MAIN MENU MENU***\n");
    setColor(7);
    printf("Account Menu:\n");
    printf("1. Register: (r)\n");
    printf("2. Login: (l)\n");
    printf("Enter Your Option (r/l): ");
    while(scanf(" %[rl]c",&option) == 0) {
        fflush(stdin);
    }
    return;
}
void userMenu(){
    setColor(59);
    printf("***USER MENU***\n");
    setColor(7);
    printf("1. Create Channel (c)\n");
    printf("2. Join Channel (j)\n");
    printf("3. Quit (q)\n");
    printf("Enter Your Option:(c/j/q) ");
    while(scanf(" %[cjq]c",&option) == 0) {
        fflush(stdin);
    }
    //scanf(" %c",&option);
    return;
}
void createChannel(){
    setColor(236);
    printf("***Create Channel***\n");
    setColor(7);
    char name[MAX];
    printf("Enter The Name Of Your Channel: ");
    scanf("%s",name);
    char code[MAX];
    memset(code, 0, sizeof(code));
    strcat(code,"create channel ");
    strcat(name,", ");
    strcat(code,name);
    strcat(code,token);
    strcat(code,"\n");
    //sending request
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    //receiving response
    char buffer[MAX];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    if(checkError(response, 'c'))
        option = 's';
    else
        option = 'u';
    delay(3);
    //checkError(response, 'r');
    return;
}
void joinChannel(){
    setColor(236);
    char name[MAX];
    memset(name, 0, sizeof(name));
    printf("***Join Channel***\n");
    setColor(7);
    printf("Enter The Name Of Channel: ");
    scanf(" %s",name);
    char code[MAX];
    memset(code, 0, sizeof(code));
    strcat(code,"join channel ");
    strcat(name,", ");
    strcat(code,name);
    strcat(code,token);
    strcat(code,"\n");
    //sending request
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    //receiving response
    char buffer[MAX];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    if(checkError(response, 'j'))
        option = 's';
    else
        option = 'u';
    delay(3);
    return;
}
void sendMenu(){
    setColor(79);
    printf("***Channel Menu***\n");
    setColor(7);
    printf("1. Send Message (1)\n");
    printf("2. Refresh (2)\n");
    printf("3. Members (3)\n");
    printf("4. Leave (4)\n");
    printf("Enter Your Option: (1/2/3/4) ");
    while(scanf(" %[1234]c",&option) == 0) {
        fflush(stdin);
    }
    //scanf(" %c",&option);
    return;
}
void quit(){
    char code[MAX];
    memset(code, 0, sizeof(code));
    strcat(code,"logout ");
    strcat(code,token);
    strcat(code,"\n");
    //sending request
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    option = 'm';
    return;
}
void sendMessage(){
    setColor(243);
    printf("***SEND MESSAGE***\n");
    setColor(7);
    printf("Enter Your Message: ");
    char message[MAX];
    //gets(message);
    scanf(" %[^\n]",message);
    char code[MAX];
    memset(code, 0, sizeof(code));
    strcat(message,", ");
    strcat(code,"send ");
    strcat(code,message);
    strcat(code,token);
    strcat(code,"\n");
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);

    char buffer[MAX];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    cJSON* Type = cJSON_GetObjectItemCaseSensitive(response, "type");
    option = 's';
    return;
}
void leaveChannel(){
    char code[MAX];
    memset(code, 0, sizeof(code));
    strcat(code, "leave ");
    strcat(code, token);
    strcat(code ,"\n");
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    char buffer[MAX];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    if(checkError(response, '4'))
        option = 'u';
    delay(3);
    return;
}
void viewMembers(){
    char code[MAX];
    memset(code, 0, sizeof(code));
    strcat(code, "channel members ");
    strcat(code, token);
    strcat(code ,"\n");
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    char buffer[1000];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    setColor(27);
    printf("***CHANNEL MEMBERS***\n");
    if(checkError(response,'3'))
    {
        cJSON* members = cJSON_GetObjectItemCaseSensitive(response, "content");
        printItems(members,1);
    }
    printf("\nPress Any Key To Continue...");
    getch();
    option = 's';
    return;
}
void viewMessages(){
    char code[MAX];
    memset(code, 0, sizeof(code));
    strcat(code, "refresh ");
    strcat(code, token);
    strcat(code ,"\n");
    socketInitialize();
    send(client_socket, code, sizeof(code), 0);
    char buffer[1000];
    memset(buffer, 0,sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* response = cJSON_Parse(buffer);
    setColor(27);
    printf("***NEW MESSAGES***\n");
    //if(checkError(response,'2'))
    //{
        //cJSON* messages = (cJSON*)malloc(sizeof(cJSON_GetObjectItemCaseSensitive(response, "content")));
        parse_object2(response);
        //cJSON* messages =cJSON_GetObjectItem(response, "content");
        //printItems(messages,2);
    //}
    setColor(7);
    printf("\nPress Any Key To Continue...");
    getch();
    option = 's';
    return;
}
void printItems(cJSON* items,int option){
    setColor(7);
    if(option == 1)
    {
        for (int i = 0 ; i < cJSON_GetArraySize(items) ; i++)
        {
            cJSON * thisItem = cJSON_GetArrayItem(items, i);
            printf("%s\n",thisItem->valuestring);
        }
    }else if(option == 2)
    {
        /*for (int i = 0 ; i < cJSON_GetArraySize(items) ; i++)
        {
            printf("%d ",cJSON_GetArraySize(items));
            cJSON * thisItem = cJSON_GetArrayItem(items, i);
            cJSON* sender = cJSON_GetObjectItem(thisItem, "sender");
            cJSON* message = cJSON_GetObjectItem(thisItem, "content");
            printf("%s : %s\n",sender->valuestring,message->valuestring);
        }*/
        cJSON* sender = NULL;
        cJSON* message = NULL;
        int i;
        for (i = 0 ; i < cJSON_GetArraySize(items) ; i++)
        {
            cJSON * subitem = cJSON_GetArrayItem(items, i);
            sender = cJSON_GetObjectItem(subitem, "sender");
            message = cJSON_GetObjectItem(subitem, "content");
            printf("%s : %s\n",sender->valuestring,message->valuestring);
        }
    }
    return;
}
void parse_object2(cJSON *root){
    setColor(7);
  cJSON* name = NULL;
  cJSON* index = NULL;

  int i;

  cJSON *item = cJSON_GetObjectItem(root,"content");
  for (i = 0 ; i < cJSON_GetArraySize(item) ; i++)
  {
     cJSON * subitem = cJSON_GetArrayItem(item, i);
     name = cJSON_GetObjectItem(subitem, "sender");
     index = cJSON_GetObjectItem(subitem, "content");
     if(strcasecmp(name->valuestring,"server") == 0)
        setColor(250);
    else
        setColor(7);
     printf("%s : %s\n",name->valuestring,index->valuestring);
  }
}
