
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n4_message_type.h"

char* OpenAPI_n4_message_type_ToString(OpenAPI_n4_message_type_e n4_message_type)
{
    const char *n4_message_typeArray[] =  { "NULL", "PFCP_SES_EST_REQ", "PFCP_SES_EST_RSP", "PFCP_SES_MOD_REQ", "PFCP_SES_MOD_RSP", "PFCP_SES_DEL_REQ", "PFCP_SES_DEL_RSP", "PFCP_SES_REP_REQ", "PFCP_SES_REP_RSP" };
    size_t sizeofArray = sizeof(n4_message_typeArray) / sizeof(n4_message_typeArray[0]);
    if (n4_message_type < sizeofArray)
        return (char *)n4_message_typeArray[n4_message_type];
    else
        return (char *)"Unknown";
}

OpenAPI_n4_message_type_e OpenAPI_n4_message_type_FromString(char* n4_message_type)
{
    int stringToReturn = 0;
    const char *n4_message_typeArray[] =  { "NULL", "PFCP_SES_EST_REQ", "PFCP_SES_EST_RSP", "PFCP_SES_MOD_REQ", "PFCP_SES_MOD_RSP", "PFCP_SES_DEL_REQ", "PFCP_SES_DEL_RSP", "PFCP_SES_REP_REQ", "PFCP_SES_REP_RSP" };
    size_t sizeofArray = sizeof(n4_message_typeArray) / sizeof(n4_message_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n4_message_type, n4_message_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

