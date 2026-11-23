
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tad_identifier.h"

char* OpenAPI_tad_identifier_ToString(OpenAPI_tad_identifier_e tad_identifier)
{
    const char *tad_identifierArray[] =  { "NULL", "CS", "PS" };
    size_t sizeofArray = sizeof(tad_identifierArray) / sizeof(tad_identifierArray[0]);
    if (tad_identifier < sizeofArray)
        return (char *)tad_identifierArray[tad_identifier];
    else
        return (char *)"Unknown";
}

OpenAPI_tad_identifier_e OpenAPI_tad_identifier_FromString(char* tad_identifier)
{
    int stringToReturn = 0;
    const char *tad_identifierArray[] =  { "NULL", "CS", "PS" };
    size_t sizeofArray = sizeof(tad_identifierArray) / sizeof(tad_identifierArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(tad_identifier, tad_identifierArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

