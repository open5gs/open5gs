
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pru_ind.h"

char* OpenAPI_pru_ind_ToString(OpenAPI_pru_ind_e pru_ind)
{
    const char *pru_indArray[] =  { "NULL", "NON_PRU", "STATIONARY_PRU", "NON_STATIONARY_PRU" };
    size_t sizeofArray = sizeof(pru_indArray) / sizeof(pru_indArray[0]);
    if (pru_ind < sizeofArray)
        return (char *)pru_indArray[pru_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_pru_ind_e OpenAPI_pru_ind_FromString(char* pru_ind)
{
    int stringToReturn = 0;
    const char *pru_indArray[] =  { "NULL", "NON_PRU", "STATIONARY_PRU", "NON_STATIONARY_PRU" };
    size_t sizeofArray = sizeof(pru_indArray) / sizeof(pru_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pru_ind, pru_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

