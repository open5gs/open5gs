
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "integrity_algorithm.h"

char* OpenAPI_integrity_algorithm_ToString(OpenAPI_integrity_algorithm_e integrity_algorithm)
{
    const char *integrity_algorithmArray[] =  { "NULL", "NIA0", "NIA1", "NIA2", "NIA3" };
    size_t sizeofArray = sizeof(integrity_algorithmArray) / sizeof(integrity_algorithmArray[0]);
    if (integrity_algorithm < sizeofArray)
        return (char *)integrity_algorithmArray[integrity_algorithm];
    else
        return (char *)"Unknown";
}

OpenAPI_integrity_algorithm_e OpenAPI_integrity_algorithm_FromString(char* integrity_algorithm)
{
    int stringToReturn = 0;
    const char *integrity_algorithmArray[] =  { "NULL", "NIA0", "NIA1", "NIA2", "NIA3" };
    size_t sizeofArray = sizeof(integrity_algorithmArray) / sizeof(integrity_algorithmArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(integrity_algorithm, integrity_algorithmArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

