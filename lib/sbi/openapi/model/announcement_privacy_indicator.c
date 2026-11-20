
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "announcement_privacy_indicator.h"

char* OpenAPI_announcement_privacy_indicator_ToString(OpenAPI_announcement_privacy_indicator_e announcement_privacy_indicator)
{
    const char *announcement_privacy_indicatorArray[] =  { "NULL", "NOT_PRIVATE", "PRIVATE" };
    size_t sizeofArray = sizeof(announcement_privacy_indicatorArray) / sizeof(announcement_privacy_indicatorArray[0]);
    if (announcement_privacy_indicator < sizeofArray)
        return (char *)announcement_privacy_indicatorArray[announcement_privacy_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_announcement_privacy_indicator_e OpenAPI_announcement_privacy_indicator_FromString(char* announcement_privacy_indicator)
{
    int stringToReturn = 0;
    const char *announcement_privacy_indicatorArray[] =  { "NULL", "NOT_PRIVATE", "PRIVATE" };
    size_t sizeofArray = sizeof(announcement_privacy_indicatorArray) / sizeof(announcement_privacy_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(announcement_privacy_indicator, announcement_privacy_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

