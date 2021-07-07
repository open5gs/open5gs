
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_nas_ciphering_algorithm.h"

char* OpenAPI_eps_nas_ciphering_algorithm_ToString(OpenAPI_eps_nas_ciphering_algorithm_e eps_nas_ciphering_algorithm)
{
    const char *eps_nas_ciphering_algorithmArray[] =  { "NULL", "EEA0", "EEA1", "EEA2", "EEA3" };
    size_t sizeofArray = sizeof(eps_nas_ciphering_algorithmArray) / sizeof(eps_nas_ciphering_algorithmArray[0]);
    if (eps_nas_ciphering_algorithm < sizeofArray)
        return (char *)eps_nas_ciphering_algorithmArray[eps_nas_ciphering_algorithm];
    else
        return (char *)"Unknown";
}

OpenAPI_eps_nas_ciphering_algorithm_e OpenAPI_eps_nas_ciphering_algorithm_FromString(char* eps_nas_ciphering_algorithm)
{
    int stringToReturn = 0;
    const char *eps_nas_ciphering_algorithmArray[] =  { "NULL", "EEA0", "EEA1", "EEA2", "EEA3" };
    size_t sizeofArray = sizeof(eps_nas_ciphering_algorithmArray) / sizeof(eps_nas_ciphering_algorithmArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(eps_nas_ciphering_algorithm, eps_nas_ciphering_algorithmArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

