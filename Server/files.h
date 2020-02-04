#include "cJSON.h"

char* makeMessageJSON(char* type, char* content);
char* makeListJSON(cJSON* list);
void insertMessageInChannel(char* Sender, char* Content, cJSON* messages);
char* makeChannelJSON(char* Name);
char* makeUserJSON(char* username, char* password);
int checkUsername_register(char* username);
int checkUsername_login(char* username, char* password);
char* createUser(char* username, char* password);
void readFile(FILE* ptr, char buffer[]);
void addOnlineUser(char* username, char* token);
int findUserIndex(char* token);
int checkToken(char* token);
int checkChannelName_create(char* name);
int checkChannelName_join(char* name);
char* createChannel(char* name, char* token);
int getFileSize(FILE* file);
char* getChannel(char* name);
char* refreshChannel(char* newMessage, int senderIndex, int server);
char* refreshMessages(int userIndex);
char* addUserToChannel(char* channel, char* token);
char* removeUserFromChannel(int userIndex);
char* findChannelMembers(int userIndex);
char* logout(int index);
