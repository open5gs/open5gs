
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_metadata.h"

char* OpenAPI_analytics_metadata_ToString(OpenAPI_analytics_metadata_e analytics_metadata)
{
    const char *analytics_metadataArray[] =  { "NULL", "NUM_OF_SAMPLES", "DATA_WINDOW", "DATA_STAT_PROPS", "STRATEGY", "ACCURACY", "DATA_SOURCES", "USED_PROC_INSTRUCT" };
    size_t sizeofArray = sizeof(analytics_metadataArray) / sizeof(analytics_metadataArray[0]);
    if (analytics_metadata < sizeofArray)
        return (char *)analytics_metadataArray[analytics_metadata];
    else
        return (char *)"Unknown";
}

OpenAPI_analytics_metadata_e OpenAPI_analytics_metadata_FromString(char* analytics_metadata)
{
    int stringToReturn = 0;
    const char *analytics_metadataArray[] =  { "NULL", "NUM_OF_SAMPLES", "DATA_WINDOW", "DATA_STAT_PROPS", "STRATEGY", "ACCURACY", "DATA_SOURCES", "USED_PROC_INSTRUCT" };
    size_t sizeofArray = sizeof(analytics_metadataArray) / sizeof(analytics_metadataArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(analytics_metadata, analytics_metadataArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

