
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "failure_code.h"

char* OpenAPI_failure_code_ToString(OpenAPI_failure_code_e failure_code)
{
    const char *failure_codeArray[] =  { "NULL", "UNK_RULE_ID", "RA_GR_ERR", "SER_ID_ERR", "NF_MAL", "RES_LIM", "MAX_NR_QoS_FLOW", "MISS_FLOW_INFO", "RES_ALLO_FAIL", "UNSUCC_QOS_VAL", "INCOR_FLOW_INFO", "PS_TO_CS_HAN", "APP_ID_ERR", "NO_QOS_FLOW_BOUND", "FILTER_RES", "MISS_REDI_SER_ADDR", "CM_END_USER_SER_DENIED", "CM_CREDIT_CON_NOT_APP", "CM_AUTH_REJ", "CM_USER_UNK", "CM_RAT_FAILED", "UE_STA_SUSP" };
    size_t sizeofArray = sizeof(failure_codeArray) / sizeof(failure_codeArray[0]);
    if (failure_code < sizeofArray)
        return (char *)failure_codeArray[failure_code];
    else
        return (char *)"Unknown";
}

OpenAPI_failure_code_e OpenAPI_failure_code_FromString(char* failure_code)
{
    int stringToReturn = 0;
    const char *failure_codeArray[] =  { "NULL", "UNK_RULE_ID", "RA_GR_ERR", "SER_ID_ERR", "NF_MAL", "RES_LIM", "MAX_NR_QoS_FLOW", "MISS_FLOW_INFO", "RES_ALLO_FAIL", "UNSUCC_QOS_VAL", "INCOR_FLOW_INFO", "PS_TO_CS_HAN", "APP_ID_ERR", "NO_QOS_FLOW_BOUND", "FILTER_RES", "MISS_REDI_SER_ADDR", "CM_END_USER_SER_DENIED", "CM_CREDIT_CON_NOT_APP", "CM_AUTH_REJ", "CM_USER_UNK", "CM_RAT_FAILED", "UE_STA_SUSP" };
    size_t sizeofArray = sizeof(failure_codeArray) / sizeof(failure_codeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(failure_code, failure_codeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

