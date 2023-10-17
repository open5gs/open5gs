
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_class_one_of.h"

char* OpenAPI_dispersion_class_one_of_ToString(OpenAPI_dispersion_class_one_of_e dispersion_class_one_of)
{
    const char *dispersion_class_one_ofArray[] =  { "NULL", "FIXED", "CAMPER", "TRAVELLER", "TOP_HEAVY" };
    size_t sizeofArray = sizeof(dispersion_class_one_ofArray) / sizeof(dispersion_class_one_ofArray[0]);
    if (dispersion_class_one_of < sizeofArray)
        return (char *)dispersion_class_one_ofArray[dispersion_class_one_of];
    else
        return (char *)"Unknown";
}

OpenAPI_dispersion_class_one_of_e OpenAPI_dispersion_class_one_of_FromString(char* dispersion_class_one_of)
{
    int stringToReturn = 0;
    const char *dispersion_class_one_ofArray[] =  { "NULL", "FIXED", "CAMPER", "TRAVELLER", "TOP_HEAVY" };
    size_t sizeofArray = sizeof(dispersion_class_one_ofArray) / sizeof(dispersion_class_one_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dispersion_class_one_of, dispersion_class_one_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

