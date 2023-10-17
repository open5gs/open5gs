
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "satellite_backhaul_category.h"

char* OpenAPI_satellite_backhaul_category_ToString(OpenAPI_satellite_backhaul_category_e satellite_backhaul_category)
{
    const char *satellite_backhaul_categoryArray[] =  { "NULL", "GEO", "MEO", "LEO", "OTHER_SAT", "NON_SATELLITE" };
    size_t sizeofArray = sizeof(satellite_backhaul_categoryArray) / sizeof(satellite_backhaul_categoryArray[0]);
    if (satellite_backhaul_category < sizeofArray)
        return (char *)satellite_backhaul_categoryArray[satellite_backhaul_category];
    else
        return (char *)"Unknown";
}

OpenAPI_satellite_backhaul_category_e OpenAPI_satellite_backhaul_category_FromString(char* satellite_backhaul_category)
{
    int stringToReturn = 0;
    const char *satellite_backhaul_categoryArray[] =  { "NULL", "GEO", "MEO", "LEO", "OTHER_SAT", "NON_SATELLITE" };
    size_t sizeofArray = sizeof(satellite_backhaul_categoryArray) / sizeof(satellite_backhaul_categoryArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(satellite_backhaul_category, satellite_backhaul_categoryArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

