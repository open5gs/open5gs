
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_metadata_any_of.h"

char* OpenAPI_analytics_metadata_any_of_ToString(OpenAPI_analytics_metadata_any_of_e analytics_metadata_any_of)
{
    const char *analytics_metadata_any_ofArray[] =  { "NULL", "NUM_OF_SAMPLES", "DATA_WINDOW", "DATA_STAT_PROPS", "STRATEGY", "ACCURACY" };
    size_t sizeofArray = sizeof(analytics_metadata_any_ofArray) / sizeof(analytics_metadata_any_ofArray[0]);
    if (analytics_metadata_any_of < sizeofArray)
        return (char *)analytics_metadata_any_ofArray[analytics_metadata_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_analytics_metadata_any_of_e OpenAPI_analytics_metadata_any_of_FromString(char* analytics_metadata_any_of)
{
    int stringToReturn = 0;
    const char *analytics_metadata_any_ofArray[] =  { "NULL", "NUM_OF_SAMPLES", "DATA_WINDOW", "DATA_STAT_PROPS", "STRATEGY", "ACCURACY" };
    size_t sizeofArray = sizeof(analytics_metadata_any_ofArray) / sizeof(analytics_metadata_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(analytics_metadata_any_of, analytics_metadata_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

