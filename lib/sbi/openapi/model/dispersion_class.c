
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_class.h"

char* OpenAPI_dispersion_class_ToString(OpenAPI_dispersion_class_e dispersion_class)
{
    const char *dispersion_classArray[] =  { "NULL", "FIXED", "CAMPER", "TRAVELLER", "TOP_HEAVY" };
    size_t sizeofArray = sizeof(dispersion_classArray) / sizeof(dispersion_classArray[0]);
    if (dispersion_class < sizeofArray)
        return (char *)dispersion_classArray[dispersion_class];
    else
        return (char *)"Unknown";
}

OpenAPI_dispersion_class_e OpenAPI_dispersion_class_FromString(char* dispersion_class)
{
    int stringToReturn = 0;
    const char *dispersion_classArray[] =  { "NULL", "FIXED", "CAMPER", "TRAVELLER", "TOP_HEAVY" };
    size_t sizeofArray = sizeof(dispersion_classArray) / sizeof(dispersion_classArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dispersion_class, dispersion_classArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

