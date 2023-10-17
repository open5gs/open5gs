
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_information_class.h"

char* OpenAPI_n2_information_class_ToString(OpenAPI_n2_information_class_e n2_information_class)
{
    const char *n2_information_classArray[] =  { "NULL", "SM", "NRPPa", "PWS", "PWS-BCAL", "PWS-RF", "RAN", "V2X", "PROSE" };
    size_t sizeofArray = sizeof(n2_information_classArray) / sizeof(n2_information_classArray[0]);
    if (n2_information_class < sizeofArray)
        return (char *)n2_information_classArray[n2_information_class];
    else
        return (char *)"Unknown";
}

OpenAPI_n2_information_class_e OpenAPI_n2_information_class_FromString(char* n2_information_class)
{
    int stringToReturn = 0;
    const char *n2_information_classArray[] =  { "NULL", "SM", "NRPPa", "PWS", "PWS-BCAL", "PWS-RF", "RAN", "V2X", "PROSE" };
    size_t sizeofArray = sizeof(n2_information_classArray) / sizeof(n2_information_classArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n2_information_class, n2_information_classArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

