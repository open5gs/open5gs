
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ngap_ie_type.h"

char* OpenAPI_ngap_ie_type_ToString(OpenAPI_ngap_ie_type_e ngap_ie_type)
{
    const char *ngap_ie_typeArray[] =  { "NULL", "PDU_RES_SETUP_REQ", "PDU_RES_REL_CMD", "PDU_RES_MOD_REQ", "HANDOVER_CMD", "HANDOVER_REQUIRED", "HANDOVER_PREP_FAIL", "SRC_TO_TAR_CONTAINER", "TAR_TO_SRC_CONTAINER", "RAN_STATUS_TRANS_CONTAINER", "SON_CONFIG_TRANSFER", "NRPPA_PDU", "UE_RADIO_CAPABILITY", "RIM_INFO_TRANSFER", "SECONDARY_RAT_USAGE" };
    size_t sizeofArray = sizeof(ngap_ie_typeArray) / sizeof(ngap_ie_typeArray[0]);
    if (ngap_ie_type < sizeofArray)
        return (char *)ngap_ie_typeArray[ngap_ie_type];
    else
        return (char *)"Unknown";
}

OpenAPI_ngap_ie_type_e OpenAPI_ngap_ie_type_FromString(char* ngap_ie_type)
{
    int stringToReturn = 0;
    const char *ngap_ie_typeArray[] =  { "NULL", "PDU_RES_SETUP_REQ", "PDU_RES_REL_CMD", "PDU_RES_MOD_REQ", "HANDOVER_CMD", "HANDOVER_REQUIRED", "HANDOVER_PREP_FAIL", "SRC_TO_TAR_CONTAINER", "TAR_TO_SRC_CONTAINER", "RAN_STATUS_TRANS_CONTAINER", "SON_CONFIG_TRANSFER", "NRPPA_PDU", "UE_RADIO_CAPABILITY", "RIM_INFO_TRANSFER", "SECONDARY_RAT_USAGE" };
    size_t sizeofArray = sizeof(ngap_ie_typeArray) / sizeof(ngap_ie_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ngap_ie_type, ngap_ie_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

