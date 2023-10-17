
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_type_any_of.h"

char* OpenAPI_event_type_any_of_ToString(OpenAPI_event_type_any_of_e event_type_any_of)
{
    const char *event_type_any_ofArray[] =  { "NULL", "LOSS_OF_CONNECTIVITY", "UE_REACHABILITY_FOR_DATA", "UE_REACHABILITY_FOR_SMS", "LOCATION_REPORTING", "CHANGE_OF_SUPI_PEI_ASSOCIATION", "ROAMING_STATUS", "COMMUNICATION_FAILURE", "AVAILABILITY_AFTER_DDN_FAILURE", "CN_TYPE_CHANGE", "DL_DATA_DELIVERY_STATUS", "PDN_CONNECTIVITY_STATUS", "UE_CONNECTION_MANAGEMENT_STATE", "ACCESS_TYPE_REPORT", "REGISTRATION_STATE_REPORT", "CONNECTIVITY_STATE_REPORT", "TYPE_ALLOCATION_CODE_REPORT", "FREQUENT_MOBILITY_REGISTRATION_REPORT", "PDU_SES_REL", "PDU_SES_EST", "UE_MEMORY_AVAILABLE_FOR_SMS" };
    size_t sizeofArray = sizeof(event_type_any_ofArray) / sizeof(event_type_any_ofArray[0]);
    if (event_type_any_of < sizeofArray)
        return (char *)event_type_any_ofArray[event_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_event_type_any_of_e OpenAPI_event_type_any_of_FromString(char* event_type_any_of)
{
    int stringToReturn = 0;
    const char *event_type_any_ofArray[] =  { "NULL", "LOSS_OF_CONNECTIVITY", "UE_REACHABILITY_FOR_DATA", "UE_REACHABILITY_FOR_SMS", "LOCATION_REPORTING", "CHANGE_OF_SUPI_PEI_ASSOCIATION", "ROAMING_STATUS", "COMMUNICATION_FAILURE", "AVAILABILITY_AFTER_DDN_FAILURE", "CN_TYPE_CHANGE", "DL_DATA_DELIVERY_STATUS", "PDN_CONNECTIVITY_STATUS", "UE_CONNECTION_MANAGEMENT_STATE", "ACCESS_TYPE_REPORT", "REGISTRATION_STATE_REPORT", "CONNECTIVITY_STATE_REPORT", "TYPE_ALLOCATION_CODE_REPORT", "FREQUENT_MOBILITY_REGISTRATION_REPORT", "PDU_SES_REL", "PDU_SES_EST", "UE_MEMORY_AVAILABLE_FOR_SMS" };
    size_t sizeofArray = sizeof(event_type_any_ofArray) / sizeof(event_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_type_any_of, event_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

