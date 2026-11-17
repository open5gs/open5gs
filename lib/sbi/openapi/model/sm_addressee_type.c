
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_addressee_type.h"

char* OpenAPI_sm_addressee_type_ToString(OpenAPI_sm_addressee_type_e sm_addressee_type)
{
    const char *sm_addressee_typeArray[] =  { "NULL", "TO", "CC", "BCC" };
    size_t sizeofArray = sizeof(sm_addressee_typeArray) / sizeof(sm_addressee_typeArray[0]);
    if (sm_addressee_type < sizeofArray)
        return (char *)sm_addressee_typeArray[sm_addressee_type];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_addressee_type_e OpenAPI_sm_addressee_type_FromString(char* sm_addressee_type)
{
    int stringToReturn = 0;
    const char *sm_addressee_typeArray[] =  { "NULL", "TO", "CC", "BCC" };
    size_t sizeofArray = sizeof(sm_addressee_typeArray) / sizeof(sm_addressee_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_addressee_type, sm_addressee_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

