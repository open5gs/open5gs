
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ciphering_algorithm.h"

char* OpenAPI_ciphering_algorithm_ToString(OpenAPI_ciphering_algorithm_e ciphering_algorithm)
{
    const char *ciphering_algorithmArray[] =  { "NULL", "NEA0", "NEA1", "NEA2", "NEA3" };
    size_t sizeofArray = sizeof(ciphering_algorithmArray) / sizeof(ciphering_algorithmArray[0]);
    if (ciphering_algorithm < sizeofArray)
        return (char *)ciphering_algorithmArray[ciphering_algorithm];
    else
        return (char *)"Unknown";
}

OpenAPI_ciphering_algorithm_e OpenAPI_ciphering_algorithm_FromString(char* ciphering_algorithm)
{
    int stringToReturn = 0;
    const char *ciphering_algorithmArray[] =  { "NULL", "NEA0", "NEA1", "NEA2", "NEA3" };
    size_t sizeofArray = sizeof(ciphering_algorithmArray) / sizeof(ciphering_algorithmArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ciphering_algorithm, ciphering_algorithmArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

