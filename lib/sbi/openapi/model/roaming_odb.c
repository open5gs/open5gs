
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_odb.h"

char* OpenAPI_roaming_odb_ToString(OpenAPI_roaming_odb_e roaming_odb)
{
    const char *roaming_odbArray[] =  { "NULL", "OUTSIDE_HOME_PLMN", "OUTSIDE_HOME_PLMN_COUNTRY" };
    size_t sizeofArray = sizeof(roaming_odbArray) / sizeof(roaming_odbArray[0]);
    if (roaming_odb < sizeofArray)
        return (char *)roaming_odbArray[roaming_odb];
    else
        return (char *)"Unknown";
}

OpenAPI_roaming_odb_e OpenAPI_roaming_odb_FromString(char* roaming_odb)
{
    int stringToReturn = 0;
    const char *roaming_odbArray[] =  { "NULL", "OUTSIDE_HOME_PLMN", "OUTSIDE_HOME_PLMN_COUNTRY" };
    size_t sizeofArray = sizeof(roaming_odbArray) / sizeof(roaming_odbArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(roaming_odb, roaming_odbArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

