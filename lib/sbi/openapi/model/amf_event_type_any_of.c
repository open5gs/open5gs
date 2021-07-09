
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_type_any_of.h"

char* OpenAPI_amf_event_type_any_of_ToString(OpenAPI_amf_event_type_any_of_e amf_event_type_any_of)
{
    const char *amf_event_type_any_ofArray[] =  { "NULL", "LOCATION_REPORT", "PRESENCE_IN_AOI_REPORT", "TIMEZONE_REPORT", "ACCESS_TYPE_REPORT", "REGISTRATION_STATE_REPORT", "CONNECTIVITY_STATE_REPORT", "REACHABILITY_REPORT", "COMMUNICATION_FAILURE_REPORT", "UES_IN_AREA_REPORT", "SUBSCRIPTION_ID_CHANGE", "SUBSCRIPTION_ID_ADDITION", "LOSS_OF_CONNECTIVITY", "_5GS_USER_STATE_REPORT", "AVAILABILITY_AFTER_DDN_FAILURE", "TYPE_ALLOCATION_CODE_REPORT", "FREQUENT_MOBILITY_REGISTRATION_REPORT" };
    size_t sizeofArray = sizeof(amf_event_type_any_ofArray) / sizeof(amf_event_type_any_ofArray[0]);
    if (amf_event_type_any_of < sizeofArray)
        return (char *)amf_event_type_any_ofArray[amf_event_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_amf_event_type_any_of_e OpenAPI_amf_event_type_any_of_FromString(char* amf_event_type_any_of)
{
    int stringToReturn = 0;
    const char *amf_event_type_any_ofArray[] =  { "NULL", "LOCATION_REPORT", "PRESENCE_IN_AOI_REPORT", "TIMEZONE_REPORT", "ACCESS_TYPE_REPORT", "REGISTRATION_STATE_REPORT", "CONNECTIVITY_STATE_REPORT", "REACHABILITY_REPORT", "COMMUNICATION_FAILURE_REPORT", "UES_IN_AREA_REPORT", "SUBSCRIPTION_ID_CHANGE", "SUBSCRIPTION_ID_ADDITION", "LOSS_OF_CONNECTIVITY", "_5GS_USER_STATE_REPORT", "AVAILABILITY_AFTER_DDN_FAILURE", "TYPE_ALLOCATION_CODE_REPORT", "FREQUENT_MOBILITY_REGISTRATION_REPORT" };
    size_t sizeofArray = sizeof(amf_event_type_any_ofArray) / sizeof(amf_event_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(amf_event_type_any_of, amf_event_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

