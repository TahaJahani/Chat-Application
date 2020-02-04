#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
//types: 0=object   1=string    2=array
cJSON* cJSON_CreateObject() {
    cJSON* newObject = calloc(1,sizeof(cJSON));
    //cJSON* ptr = &newObject;
    newObject->child = NULL;
    newObject->next = NULL;
    newObject->prev = NULL;
    newObject->type = 0;
    return newObject;
}
//--------------------------------
cJSON* cJSON_CreateString(char* string) {
    cJSON* newString = calloc(1,sizeof(cJSON));
    //cJSON* ptr = &newString;
    newString->child = NULL;
    newString->next = NULL;
    newString->prev = NULL;
    newString->type = 1;
    newString->valuestring = calloc(strlen(string)+1,1);
    strcpy(newString->valuestring, string);
    return newString;
}
//--------------------------------
cJSON* cJSON_CreateArray() {//Not OK****************
    cJSON* newArray = calloc(1,sizeof(cJSON));
    newArray->type = 2;
    return newArray;
}
//--------------------------------
void cJSON_AddItemToChild(cJSON* Child, cJSON* Next) {
    if(Child->next != NULL)
    {
        cJSON_AddItemToChild(Child->next, Next);
    }
    else
    {
        Child->next = Next;
    }
}
//--------------------------------
void cJSON_AddItemToObject(cJSON* parent, char* string, cJSON* Child) {
    Child->string = calloc(strlen(string)+1, 1);
    strcpy(Child->string, string);
    if(parent->child != NULL)
        cJSON_AddItemToChild(parent->child, Child);
    else
        parent->child = Child;
}
//--------------------------------
void cJSON_AddItemToArray(cJSON* arr, cJSON* obj) {
    if(arr->child != NULL)
        cJSON_AddItemToChild(arr->child, obj);
    else
        arr->child = obj;
}
//--------------------------------
cJSON* cJSON_AddArrayToObject(cJSON* object, char* string) {
    cJSON* newArr = calloc(1, sizeof(cJSON));
    newArr->type = 2;
    cJSON_AddItemToObject(object, string, newArr);
    return newArr;
}
//--------------------------------
char* cJSON_Print(cJSON* obj) {
    char* printed = calloc(1,10000);
    int size = 10000;
    if(obj->type == 0)
    {
        int size = 0;
        cJSON* thisChild = obj->child;
        //printf("Type Child: %d\n",thisChild->type);
        sprintf(printed, "{");
        size ++;
        char* add = cJSON_Print(thisChild);
        //printf("child1: %s\n",add);
        if(strlen(add) > (size - 100))
        {
            printed = realloc(printed, size+500);
            size += 500;
        }
        strcat(printed, add);
        free(add);
        thisChild = thisChild->next;
        //printf("Type Next: %d\n",thisChild->type);
        if(thisChild != NULL)
        {
            strcat(printed,",");
            add = cJSON_Print(thisChild);
            if(strlen(add) > (size - 100))
            {
                printed = realloc(printed, size+500);
                size += 500;
            }
            strcat(printed, add);
            //printf("Next: %s\n",add);
            free(add);
        }
        //system("pause");
        strcat(printed, "}");
        return printed;
    }else if(obj->type == 1)
    {
        if(obj->string != NULL)
        {
            strcat(printed, "\"");
            strcat(printed, obj->string);
            strcat(printed, "\":");
        }
        strcat(printed, "\"");
        strcat(printed, obj->valuestring);
        strcat(printed,"\"");
        /*if(obj->next != NULL)
        {
            strcat(printed, ",");
            char* add = cJSON_Print(obj->next);
            printf("ADD: %s\n",add);
            strcat(printed, add);
            free(add);
        }*/
        return printed;
    }else if(obj->type == 2)
    {
        strcat(printed, "\"");
        strcat(printed, obj->string);
        strcat(printed, "\":");
        strcat(printed, "[");
        cJSON* thisChild = obj->child;
        while(thisChild != NULL)
        {
            char* add = cJSON_Print(thisChild);
            strcat(printed, add);
            //printf("Item Added: %s\n",add);
            free(add);
            thisChild = thisChild->next;
            //printf("type: %s\n",thisChild);
            if(thisChild != NULL)
                strcat(printed, ",");
        }
        strcat(printed,"]");
        //printf("Arr Printed\n");
        return printed;
    }
    return printed;
}
//--------------------------------
int cJSON_GetArraySize(cJSON* arr) {
    int num = 0;
    cJSON* thisChild = arr->child;
    while(thisChild != NULL)
    {
        //printf("this: %s\n",thisChild->child->next->valuestring);
        num ++;
        thisChild = thisChild->next;
    }
    return num;
}
//--------------------------------
cJSON* cJSON_GetObjectItem(cJSON* obj, char* String) {
    cJSON* thisChild = obj->child;
    while(strcasecmp(thisChild->string, String) != 0)
    {
        thisChild = thisChild->next;
        if(thisChild == NULL)
            break;
    }
    return thisChild;
}
//--------------------------------
cJSON* cJSON_GetArrayItem(cJSON* arr, int i) {
    cJSON* Child = arr->child;
    for(int j = 0; j < i ; j ++)
        Child = Child->next;
    return Child;
}
//--------------------------------
int findIndexOfChar(char* string, int initPoint, char c) {
    while(1)
    {
        if(string[initPoint] == c)
            break;
        initPoint ++;
        if(string[initPoint] == '\0')
            return -1;
    }
    return initPoint;
}
//--------------------------------
char* extractString(char* string, int initPoint, char c1, char c2, int mod) {
    int index1 = findIndexOfChar(string, initPoint, c1);
    int index2 = findIndexOfChar(string, index1+1, c2);
    char* dest;
    if(mod == 1)
    {
        dest = calloc((index2-index1),1);
        for(int i = index1+1 ; i < index2 ; i++)
            dest[i-(index1+1)] = string[i];
    }else if(mod == 2)
    {
        dest = calloc((index2-index1)+2,1);
        for(int i = index1 ; i <= index2 ; i++)
            dest[i-index1] = string[i];
    }
    return dest;
}
//--------------------------------
cJSON* cJSON_Parse(char* json) {
    //puts(json);
    if(json[0] == '[')
    {
        //printf("Arr Parsing\n");
        //cJSON* arr = cJSON_CreateArray();
        cJSON* arr = calloc(1, sizeof(cJSON));
        arr->type = 2;
        int index = 0;
        if(findIndexOfChar(json, 0,'{') != -1)
        {
            while(index != -1)
            {
                char* string = extractString(json, index, '{', '}',2);
                cJSON* newObj = cJSON_Parse(string);
                index = findIndexOfChar(json, index, '}');
                cJSON_AddItemToArray(arr, newObj);
                index = findIndexOfChar(json, index+1, ',');
            }
        }else
        {
            while(index != -1)
            {
                char* string = extractString(json, index, '\"', '\"',2);
                index = findIndexOfChar(json, index+1, ',');
                cJSON* newStr = cJSON_CreateString(string);
                cJSON_AddItemToArray(arr, newStr);
            }
        }
        //printf("Arr Finished successfully\n");
        //system("pause");
        return arr;
    }else if(findIndexOfChar(json, 0, '[') != -1)
    {
        //printf("Object Parsing\n");
        cJSON* obj = cJSON_CreateObject();
        int index = 0;
        while(index != -1)
        {
            char* string = extractString(json, index, '\"', '\"',1);
            index = findIndexOfChar(json, index+1, ':');
            if(index == -1)
                break;
            if(json[index+1] == '[')
            {
                char* arr = extractString(json, index, '[', ']',2);
                cJSON* thisArr = cJSON_Parse(arr);
                cJSON_AddItemToObject(obj, string, thisArr);
                index = findIndexOfChar(json, index, ']');
            }else
            {
                //printf("\tname adding\n");
                char* valuestring = extractString(json, index, '\"', '\"',1);
                cJSON* child = cJSON_CreateString(valuestring);
                cJSON_AddItemToObject(obj, string, child);
                index = findIndexOfChar(json, index, ',');
                free(valuestring);
                free(string);
            }
        }
        //printf("Object Finished\n");
        //system("pause");
        return obj;
    }else if(json[0] == '{')
    {
        //printf("String Parsing\n");
        cJSON* obj = cJSON_CreateObject();
        int index = 0;
        while(index != -1)
        {
            //printf("\tsubSting adding\n");
            char* string = extractString(json, index, '\"', '\"',1);
            index = findIndexOfChar(json, index+1, ':');
            char* valuestring = extractString(json, index, '\"', '\"',1);
            cJSON* child1 = cJSON_CreateString(valuestring);
            cJSON_AddItemToObject(obj, string, child1);
            index = findIndexOfChar(json, index, ',');
            free(valuestring);
            free(string);
        }
        //printf("String Finished\n");
        //system("pause");
        return obj;
    }
    //return obj;
}

