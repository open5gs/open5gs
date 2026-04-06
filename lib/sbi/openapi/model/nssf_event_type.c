
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssf_event_type.h"

char* OpenAPI_nssf_event_type_ToString(OpenAPI_nssf_event_type_e nssf_event_type)
{
    const char *nssf_event_typeArray[] =  { "NULL", "SNSSAI_STATUS_CHANGE_REPORT", "SNSSAI_REPLACEMENT_REPORT", "NSI_UNAVAILABILITY_REPORT", "SNSSAI_VALIDITY_TIME_REPORT" };
    size_t sizeofArray = sizeof(nssf_event_typeArray) / sizeof(nssf_event_typeArray[0]);
    if (nssf_event_type < sizeofArray)
        return (char *)nssf_event_typeArray[nssf_event_type];
    else
        return (char *)"Unknown";
}

OpenAPI_nssf_event_type_e OpenAPI_nssf_event_type_FromString(char* nssf_event_type)
{
    int stringToReturn = 0;
    const char *nssf_event_typeArray[] =  { "NULL", "SNSSAI_STATUS_CHANGE_REPORT", "SNSSAI_REPLACEMENT_REPORT", "NSI_UNAVAILABILITY_REPORT", "SNSSAI_VALIDITY_TIME_REPORT" };
    size_t sizeofArray = sizeof(nssf_event_typeArray) / sizeof(nssf_event_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nssf_event_type, nssf_event_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

