#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct JSONMelli{
    struct JSONMelli* next;
    struct JSONMelli* prev;
    struct JSONMelli* child;
    //-----------
    int type;
    char* valuestring;
    char* string;

}cJSON;

cJSON* cJSON_CreateObject();
cJSON* cJSON_CreateString(char* string);
void cJSON_AddItemToChild(cJSON* Child, cJSON* next);
void cJSON_AddItemToObject(cJSON* parent, char* string, cJSON* child);
void cJSON_AddItemToArray(cJSON* arr, cJSON* obj);
int cJSON_GetArraySize(cJSON* arr);
cJSON* cJSON_GetObjectItem(cJSON* obj, char* string);
cJSON* cJSON_GetArrayItem(cJSON* arr, int i);
char* extractString(char* string, int initPoint, char c1, char c2,int mod);
int findIndexOfChar(char* string, int initPoint, char c);
cJSON* cJSON_Parse(char* json);
