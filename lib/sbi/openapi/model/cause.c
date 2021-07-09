
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cause.h"

char* OpenAPI_cause_ToString(OpenAPI_cause_e cause)
{
    const char *causeArray[] =  { "NULL", "REL_DUE_TO_HO", "EPS_FALLBACK", "REL_DUE_TO_UP_SEC", "DNN_CONGESTION", "S_NSSAI_CONGESTION", "REL_DUE_TO_REACTIVATION", "_5G_AN_NOT_RESPONDING", "REL_DUE_TO_SLICE_NOT_AVAILABLE", "REL_DUE_TO_DUPLICATE_SESSION_ID", "PDU_SESSION_STATUS_MISMATCH", "HO_FAILURE", "INSUFFICIENT_UP_RESOURCES", "PDU_SESSION_HANDED_OVER", "PDU_SESSION_RESUMED", "CN_ASSISTED_RAN_PARAMETER_TUNING", "ISMF_CONTEXT_TRANSFER", "SMF_CONTEXT_TRANSFER", "REL_DUE_TO_PS_TO_CS_HO", "REL_DUE_TO_SUBSCRIPTION_CHANGE", "HO_CANCEL", "REL_DUE_TO_SLICE_NOT_AUTHORIZED", "PDU_SESSION_HAND_OVER_FAILURE", "DDN_FAILURE_STATUS", "REL_DUE_TO_CP_ONLY_NOT_APPLICABLE", "NOT_SUPPORTED_WITH_ISMF", "CHANGED_ANCHOR_SMF", "CHANGED_INTERMEDIATE_SMF" };
    size_t sizeofArray = sizeof(causeArray) / sizeof(causeArray[0]);
    if (cause < sizeofArray)
        return (char *)causeArray[cause];
    else
        return (char *)"Unknown";
}

OpenAPI_cause_e OpenAPI_cause_FromString(char* cause)
{
    int stringToReturn = 0;
    const char *causeArray[] =  { "NULL", "REL_DUE_TO_HO", "EPS_FALLBACK", "REL_DUE_TO_UP_SEC", "DNN_CONGESTION", "S_NSSAI_CONGESTION", "REL_DUE_TO_REACTIVATION", "_5G_AN_NOT_RESPONDING", "REL_DUE_TO_SLICE_NOT_AVAILABLE", "REL_DUE_TO_DUPLICATE_SESSION_ID", "PDU_SESSION_STATUS_MISMATCH", "HO_FAILURE", "INSUFFICIENT_UP_RESOURCES", "PDU_SESSION_HANDED_OVER", "PDU_SESSION_RESUMED", "CN_ASSISTED_RAN_PARAMETER_TUNING", "ISMF_CONTEXT_TRANSFER", "SMF_CONTEXT_TRANSFER", "REL_DUE_TO_PS_TO_CS_HO", "REL_DUE_TO_SUBSCRIPTION_CHANGE", "HO_CANCEL", "REL_DUE_TO_SLICE_NOT_AUTHORIZED", "PDU_SESSION_HAND_OVER_FAILURE", "DDN_FAILURE_STATUS", "REL_DUE_TO_CP_ONLY_NOT_APPLICABLE", "NOT_SUPPORTED_WITH_ISMF", "CHANGED_ANCHOR_SMF", "CHANGED_INTERMEDIATE_SMF" };
    size_t sizeofArray = sizeof(causeArray) / sizeof(causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(cause, causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

