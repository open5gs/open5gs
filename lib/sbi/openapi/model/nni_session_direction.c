
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nni_session_direction.h"

char* OpenAPI_nni_session_direction_ToString(OpenAPI_nni_session_direction_e nni_session_direction)
{
    const char *nni_session_directionArray[] =  { "NULL", "INBOUND", "OUTBOUND" };
    size_t sizeofArray = sizeof(nni_session_directionArray) / sizeof(nni_session_directionArray[0]);
    if (nni_session_direction < sizeofArray)
        return (char *)nni_session_directionArray[nni_session_direction];
    else
        return (char *)"Unknown";
}

OpenAPI_nni_session_direction_e OpenAPI_nni_session_direction_FromString(char* nni_session_direction)
{
    int stringToReturn = 0;
    const char *nni_session_directionArray[] =  { "NULL", "INBOUND", "OUTBOUND" };
    size_t sizeofArray = sizeof(nni_session_directionArray) / sizeof(nni_session_directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nni_session_direction, nni_session_directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

