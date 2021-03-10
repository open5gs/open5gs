
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_indication.h"

char* OpenAPI_roaming_indication_ToString(OpenAPI_roaming_indication_e roaming_indication)
{
    const char *roaming_indicationArray[] =  { "NULL", "NON_ROAMING", "LOCAL_BREAKOUT", "HOME_ROUTED_ROAMING" };
    size_t sizeofArray = sizeof(roaming_indicationArray) / sizeof(roaming_indicationArray[0]);
    if (roaming_indication < sizeofArray)
        return (char *)roaming_indicationArray[roaming_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_roaming_indication_e OpenAPI_roaming_indication_FromString(char* roaming_indication)
{
    int stringToReturn = 0;
    const char *roaming_indicationArray[] =  { "NULL", "NON_ROAMING", "LOCAL_BREAKOUT", "HOME_ROUTED_ROAMING" };
    size_t sizeofArray = sizeof(roaming_indicationArray) / sizeof(roaming_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(roaming_indication, roaming_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

