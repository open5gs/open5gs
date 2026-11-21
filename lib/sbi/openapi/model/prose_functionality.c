
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prose_functionality.h"

char* OpenAPI_prose_functionality_ToString(OpenAPI_prose_functionality_e prose_functionality)
{
    const char *prose_functionalityArray[] =  { "NULL", "DIRECT_DISCOVERY", "DIRECT_COMMUNICATION" };
    size_t sizeofArray = sizeof(prose_functionalityArray) / sizeof(prose_functionalityArray[0]);
    if (prose_functionality < sizeofArray)
        return (char *)prose_functionalityArray[prose_functionality];
    else
        return (char *)"Unknown";
}

OpenAPI_prose_functionality_e OpenAPI_prose_functionality_FromString(char* prose_functionality)
{
    int stringToReturn = 0;
    const char *prose_functionalityArray[] =  { "NULL", "DIRECT_DISCOVERY", "DIRECT_COMMUNICATION" };
    size_t sizeofArray = sizeof(prose_functionalityArray) / sizeof(prose_functionalityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(prose_functionality, prose_functionalityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

