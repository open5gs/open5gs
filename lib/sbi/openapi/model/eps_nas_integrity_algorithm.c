
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_nas_integrity_algorithm.h"

char* OpenAPI_eps_nas_integrity_algorithm_ToString(OpenAPI_eps_nas_integrity_algorithm_e eps_nas_integrity_algorithm)
{
    const char *eps_nas_integrity_algorithmArray[] =  { "NULL", "EIA0", "EIA1", "EIA2", "EIA3" };
    size_t sizeofArray = sizeof(eps_nas_integrity_algorithmArray) / sizeof(eps_nas_integrity_algorithmArray[0]);
    if (eps_nas_integrity_algorithm < sizeofArray)
        return (char *)eps_nas_integrity_algorithmArray[eps_nas_integrity_algorithm];
    else
        return (char *)"Unknown";
}

OpenAPI_eps_nas_integrity_algorithm_e OpenAPI_eps_nas_integrity_algorithm_FromString(char* eps_nas_integrity_algorithm)
{
    int stringToReturn = 0;
    const char *eps_nas_integrity_algorithmArray[] =  { "NULL", "EIA0", "EIA1", "EIA2", "EIA3" };
    size_t sizeofArray = sizeof(eps_nas_integrity_algorithmArray) / sizeof(eps_nas_integrity_algorithmArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(eps_nas_integrity_algorithm, eps_nas_integrity_algorithmArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

