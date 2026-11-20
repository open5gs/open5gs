
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "range_class.h"

char* OpenAPI_range_class_ToString(OpenAPI_range_class_e range_class)
{
    const char *range_classArray[] =  { "NULL", "RESERVED", "50_METER", "100_METER", "200_METER", "500_METER", "1000_METER", "UNUSED" };
    size_t sizeofArray = sizeof(range_classArray) / sizeof(range_classArray[0]);
    if (range_class < sizeofArray)
        return (char *)range_classArray[range_class];
    else
        return (char *)"Unknown";
}

OpenAPI_range_class_e OpenAPI_range_class_FromString(char* range_class)
{
    int stringToReturn = 0;
    const char *range_classArray[] =  { "NULL", "RESERVED", "50_METER", "100_METER", "200_METER", "500_METER", "1000_METER", "UNUSED" };
    size_t sizeofArray = sizeof(range_classArray) / sizeof(range_classArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(range_class, range_classArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

