
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "release_cause.h"

char* OpenAPI_release_cause_ToString(OpenAPI_release_cause_e release_cause)
{
    const char *release_causeArray[] =  { "NULL", "SNPN_SNPN_MOBILITY", "NO_HR_AGREEMENT", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(release_causeArray) / sizeof(release_causeArray[0]);
    if (release_cause < sizeofArray)
        return (char *)release_causeArray[release_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_release_cause_e OpenAPI_release_cause_FromString(char* release_cause)
{
    int stringToReturn = 0;
    const char *release_causeArray[] =  { "NULL", "SNPN_SNPN_MOBILITY", "NO_HR_AGREEMENT", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(release_causeArray) / sizeof(release_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(release_cause, release_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

