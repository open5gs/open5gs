
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_source.h"

char* OpenAPI_time_source_ToString(OpenAPI_time_source_e time_source)
{
    const char *time_sourceArray[] =  { "NULL", "SYNC_E", "PTP", "GNSS", "ATOMIC_CLOCK", "TERRESTRIAL_RADIO", "SERIAL_TIME_CODE", "NTP", "HAND_SET", "OTHER" };
    size_t sizeofArray = sizeof(time_sourceArray) / sizeof(time_sourceArray[0]);
    if (time_source < sizeofArray)
        return (char *)time_sourceArray[time_source];
    else
        return (char *)"Unknown";
}

OpenAPI_time_source_e OpenAPI_time_source_FromString(char* time_source)
{
    int stringToReturn = 0;
    const char *time_sourceArray[] =  { "NULL", "SYNC_E", "PTP", "GNSS", "ATOMIC_CLOCK", "TERRESTRIAL_RADIO", "SERIAL_TIME_CODE", "NTP", "HAND_SET", "OTHER" };
    size_t sizeofArray = sizeof(time_sourceArray) / sizeof(time_sourceArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(time_source, time_sourceArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

