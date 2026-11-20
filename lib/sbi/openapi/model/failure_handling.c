
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "failure_handling.h"

char* OpenAPI_failure_handling_ToString(OpenAPI_failure_handling_e failure_handling)
{
    const char *failure_handlingArray[] =  { "NULL", "TERMINATE", "CONTINUE", "RETRY_AND_TERMINATE" };
    size_t sizeofArray = sizeof(failure_handlingArray) / sizeof(failure_handlingArray[0]);
    if (failure_handling < sizeofArray)
        return (char *)failure_handlingArray[failure_handling];
    else
        return (char *)"Unknown";
}

OpenAPI_failure_handling_e OpenAPI_failure_handling_FromString(char* failure_handling)
{
    int stringToReturn = 0;
    const char *failure_handlingArray[] =  { "NULL", "TERMINATE", "CONTINUE", "RETRY_AND_TERMINATE" };
    size_t sizeofArray = sizeof(failure_handlingArray) / sizeof(failure_handlingArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(failure_handling, failure_handlingArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

