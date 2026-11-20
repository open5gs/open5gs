
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reply_path_requested.h"

char* OpenAPI_reply_path_requested_ToString(OpenAPI_reply_path_requested_e reply_path_requested)
{
    const char *reply_path_requestedArray[] =  { "NULL", "NO_REPLY_PATH_SET", "REPLY_PATH_SET" };
    size_t sizeofArray = sizeof(reply_path_requestedArray) / sizeof(reply_path_requestedArray[0]);
    if (reply_path_requested < sizeofArray)
        return (char *)reply_path_requestedArray[reply_path_requested];
    else
        return (char *)"Unknown";
}

OpenAPI_reply_path_requested_e OpenAPI_reply_path_requested_FromString(char* reply_path_requested)
{
    int stringToReturn = 0;
    const char *reply_path_requestedArray[] =  { "NULL", "NO_REPLY_PATH_SET", "REPLY_PATH_SET" };
    size_t sizeofArray = sizeof(reply_path_requestedArray) / sizeof(reply_path_requestedArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reply_path_requested, reply_path_requestedArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

