
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "loc_info_granularity.h"

char* OpenAPI_loc_info_granularity_ToString(OpenAPI_loc_info_granularity_e loc_info_granularity)
{
    const char *loc_info_granularityArray[] =  { "NULL", "TA_LEVEL", "CELL_LEVEL", "LON_AND_LAT_LEVEL", "LON_LAT_ALT_LEVEL" };
    size_t sizeofArray = sizeof(loc_info_granularityArray) / sizeof(loc_info_granularityArray[0]);
    if (loc_info_granularity < sizeofArray)
        return (char *)loc_info_granularityArray[loc_info_granularity];
    else
        return (char *)"Unknown";
}

OpenAPI_loc_info_granularity_e OpenAPI_loc_info_granularity_FromString(char* loc_info_granularity)
{
    int stringToReturn = 0;
    const char *loc_info_granularityArray[] =  { "NULL", "TA_LEVEL", "CELL_LEVEL", "LON_AND_LAT_LEVEL", "LON_LAT_ALT_LEVEL" };
    size_t sizeofArray = sizeof(loc_info_granularityArray) / sizeof(loc_info_granularityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(loc_info_granularity, loc_info_granularityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

