
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tid_type.h"

char* OpenAPI_tid_type_ToString(OpenAPI_tid_type_e tid_type)
{
    const char *tid_typeArray[] =  { "NULL", "STORED", "CONCEALED" };
    size_t sizeofArray = sizeof(tid_typeArray) / sizeof(tid_typeArray[0]);
    if (tid_type < sizeofArray)
        return (char *)tid_typeArray[tid_type];
    else
        return (char *)"Unknown";
}

OpenAPI_tid_type_e OpenAPI_tid_type_FromString(char* tid_type)
{
    int stringToReturn = 0;
    const char *tid_typeArray[] =  { "NULL", "STORED", "CONCEALED" };
    size_t sizeofArray = sizeof(tid_typeArray) / sizeof(tid_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(tid_type, tid_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

