
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_client_class.h"

char* OpenAPI_lcs_client_class_ToString(OpenAPI_lcs_client_class_e lcs_client_class)
{
    const char *lcs_client_classArray[] =  { "NULL", "BROADCAST_SERVICE", "OM_IN_HPLMN", "OM_IN_VPLMN", "ANONYMOUS_LOCATION_SERVICE", "SPECIFIC_SERVICE" };
    size_t sizeofArray = sizeof(lcs_client_classArray) / sizeof(lcs_client_classArray[0]);
    if (lcs_client_class < sizeofArray)
        return (char *)lcs_client_classArray[lcs_client_class];
    else
        return (char *)"Unknown";
}

OpenAPI_lcs_client_class_e OpenAPI_lcs_client_class_FromString(char* lcs_client_class)
{
    int stringToReturn = 0;
    const char *lcs_client_classArray[] =  { "NULL", "BROADCAST_SERVICE", "OM_IN_HPLMN", "OM_IN_VPLMN", "ANONYMOUS_LOCATION_SERVICE", "SPECIFIC_SERVICE" };
    size_t sizeofArray = sizeof(lcs_client_classArray) / sizeof(lcs_client_classArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(lcs_client_class, lcs_client_classArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

