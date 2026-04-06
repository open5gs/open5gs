
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_orientation.h"

char* OpenAPI_location_orientation_ToString(OpenAPI_location_orientation_e location_orientation)
{
    const char *location_orientationArray[] =  { "NULL", "HORIZONTAL", "VERTICAL", "HOR_AND_VER" };
    size_t sizeofArray = sizeof(location_orientationArray) / sizeof(location_orientationArray[0]);
    if (location_orientation < sizeofArray)
        return (char *)location_orientationArray[location_orientation];
    else
        return (char *)"Unknown";
}

OpenAPI_location_orientation_e OpenAPI_location_orientation_FromString(char* location_orientation)
{
    int stringToReturn = 0;
    const char *location_orientationArray[] =  { "NULL", "HORIZONTAL", "VERTICAL", "HOR_AND_VER" };
    size_t sizeofArray = sizeof(location_orientationArray) / sizeof(location_orientationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(location_orientation, location_orientationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

