
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "header_handling_cond.h"

char* OpenAPI_header_handling_cond_ToString(OpenAPI_header_handling_cond_e header_handling_cond)
{
    const char *header_handling_condArray[] =  { "NULL", "EVERY_MATCH", "FIRST_MATCH" };
    size_t sizeofArray = sizeof(header_handling_condArray) / sizeof(header_handling_condArray[0]);
    if (header_handling_cond < sizeofArray)
        return (char *)header_handling_condArray[header_handling_cond];
    else
        return (char *)"Unknown";
}

OpenAPI_header_handling_cond_e OpenAPI_header_handling_cond_FromString(char* header_handling_cond)
{
    int stringToReturn = 0;
    const char *header_handling_condArray[] =  { "NULL", "EVERY_MATCH", "FIRST_MATCH" };
    size_t sizeofArray = sizeof(header_handling_condArray) / sizeof(header_handling_condArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(header_handling_cond, header_handling_condArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

