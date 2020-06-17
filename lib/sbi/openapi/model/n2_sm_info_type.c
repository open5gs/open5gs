
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_sm_info_type.h"

char* OpenAPI_n2_sm_info_type_ToString(OpenAPI_n2_sm_info_type_e n2_sm_info_type)
{
    const char *n2_sm_info_typeArray[] =  { "NULL", "PDU_RES_SETUP_REQ", "PDU_RES_SETUP_RSP", "PDU_RES_SETUP_FAIL", "PDU_RES_REL_CMD", "PDU_RES_REL_RSP", "PDU_RES_MOD_REQ", "PDU_RES_MOD_RSP", "PDU_RES_MOD_FAIL", "PDU_RES_NTY", "PDU_RES_NTY_REL", "PDU_RES_MOD_IND", "PDU_RES_MOD_CFM", "PATH_SWITCH_REQ", "PATH_SWITCH_SETUP_FAIL", "PATH_SWITCH_REQ_ACK", "PATH_SWITCH_REQ_FAIL", "HANDOVER_REQUIRED", "HANDOVER_CMD", "HANDOVER_PREP_FAIL", "HANDOVER_REQ_ACK", "HANDOVER_RES_ALLOC_FAIL", "SECONDARY_RAT_USAGE" };
    size_t sizeofArray = sizeof(n2_sm_info_typeArray) / sizeof(n2_sm_info_typeArray[0]);
    if (n2_sm_info_type < sizeofArray)
        return (char *)n2_sm_info_typeArray[n2_sm_info_type];
    else
        return (char *)"Unknown";
}

OpenAPI_n2_sm_info_type_e OpenAPI_n2_sm_info_type_FromString(char* n2_sm_info_type)
{
    int stringToReturn = 0;
    const char *n2_sm_info_typeArray[] =  { "NULL", "PDU_RES_SETUP_REQ", "PDU_RES_SETUP_RSP", "PDU_RES_SETUP_FAIL", "PDU_RES_REL_CMD", "PDU_RES_REL_RSP", "PDU_RES_MOD_REQ", "PDU_RES_MOD_RSP", "PDU_RES_MOD_FAIL", "PDU_RES_NTY", "PDU_RES_NTY_REL", "PDU_RES_MOD_IND", "PDU_RES_MOD_CFM", "PATH_SWITCH_REQ", "PATH_SWITCH_SETUP_FAIL", "PATH_SWITCH_REQ_ACK", "PATH_SWITCH_REQ_FAIL", "HANDOVER_REQUIRED", "HANDOVER_CMD", "HANDOVER_PREP_FAIL", "HANDOVER_REQ_ACK", "HANDOVER_RES_ALLOC_FAIL", "SECONDARY_RAT_USAGE" };
    size_t sizeofArray = sizeof(n2_sm_info_typeArray) / sizeof(n2_sm_info_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n2_sm_info_type, n2_sm_info_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

