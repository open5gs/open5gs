
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trigger_type_any_of.h"

char* OpenAPI_trigger_type_any_of_ToString(OpenAPI_trigger_type_any_of_e trigger_type_any_of)
{
    const char *trigger_type_any_ofArray[] =  { "NULL", "QUOTA_THRESHOLD", "QHT", "FINAL", "QUOTA_EXHAUSTED", "VALIDITY_TIME", "OTHER_QUOTA_TYPE", "FORCED_REAUTHORISATION", "UNUSED_QUOTA_TIMER", "UNIT_COUNT_INACTIVITY_TIMER", "ABNORMAL_RELEASE", "QOS_CHANGE", "VOLUME_LIMIT", "TIME_LIMIT", "EVENT_LIMIT", "PLMN_CHANGE", "USER_LOCATION_CHANGE", "RAT_CHANGE", "SESSION_AMBR_CHANGE", "UE_TIMEZONE_CHANGE", "TARIFF_TIME_CHANGE", "MAX_NUMBER_OF_CHANGES_IN_CHARGING_CONDITIONS", "MANAGEMENT_INTERVENTION", "CHANGE_OF_UE_PRESENCE_IN_PRESENCE_REPORTING_AREA", "CHANGE_OF_3GPP_PS_DATA_OFF_STATUS", "SERVING_NODE_CHANGE", "REMOVAL_OF_UPF", "ADDITION_OF_UPF", "INSERTION_OF_ISMF", "REMOVAL_OF_ISMF", "CHANGE_OF_ISMF", "START_OF_SERVICE_DATA_FLOW", "ECGI_CHANGE", "TAI_CHANGE", "HANDOVER_CANCEL", "HANDOVER_START", "HANDOVER_COMPLETE", "GFBR_GUARANTEED_STATUS_CHANGE", "ADDITION_OF_ACCESS", "REMOVAL_OF_ACCESS", "START_OF_SDF_ADDITIONAL_ACCESS", "REDUNDANT_TRANSMISSION_CHANGE", "CGI_SAI_CHANGE", "RAI_CHANGE" };
    size_t sizeofArray = sizeof(trigger_type_any_ofArray) / sizeof(trigger_type_any_ofArray[0]);
    if (trigger_type_any_of < sizeofArray)
        return (char *)trigger_type_any_ofArray[trigger_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_trigger_type_any_of_e OpenAPI_trigger_type_any_of_FromString(char* trigger_type_any_of)
{
    int stringToReturn = 0;
    const char *trigger_type_any_ofArray[] =  { "NULL", "QUOTA_THRESHOLD", "QHT", "FINAL", "QUOTA_EXHAUSTED", "VALIDITY_TIME", "OTHER_QUOTA_TYPE", "FORCED_REAUTHORISATION", "UNUSED_QUOTA_TIMER", "UNIT_COUNT_INACTIVITY_TIMER", "ABNORMAL_RELEASE", "QOS_CHANGE", "VOLUME_LIMIT", "TIME_LIMIT", "EVENT_LIMIT", "PLMN_CHANGE", "USER_LOCATION_CHANGE", "RAT_CHANGE", "SESSION_AMBR_CHANGE", "UE_TIMEZONE_CHANGE", "TARIFF_TIME_CHANGE", "MAX_NUMBER_OF_CHANGES_IN_CHARGING_CONDITIONS", "MANAGEMENT_INTERVENTION", "CHANGE_OF_UE_PRESENCE_IN_PRESENCE_REPORTING_AREA", "CHANGE_OF_3GPP_PS_DATA_OFF_STATUS", "SERVING_NODE_CHANGE", "REMOVAL_OF_UPF", "ADDITION_OF_UPF", "INSERTION_OF_ISMF", "REMOVAL_OF_ISMF", "CHANGE_OF_ISMF", "START_OF_SERVICE_DATA_FLOW", "ECGI_CHANGE", "TAI_CHANGE", "HANDOVER_CANCEL", "HANDOVER_START", "HANDOVER_COMPLETE", "GFBR_GUARANTEED_STATUS_CHANGE", "ADDITION_OF_ACCESS", "REMOVAL_OF_ACCESS", "START_OF_SDF_ADDITIONAL_ACCESS", "REDUNDANT_TRANSMISSION_CHANGE", "CGI_SAI_CHANGE", "RAI_CHANGE" };
    size_t sizeofArray = sizeof(trigger_type_any_ofArray) / sizeof(trigger_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(trigger_type_any_of, trigger_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

