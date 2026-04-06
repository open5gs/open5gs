
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rtp_header_ext_type.h"

char* OpenAPI_rtp_header_ext_type_ToString(OpenAPI_rtp_header_ext_type_e rtp_header_ext_type)
{
    const char *rtp_header_ext_typeArray[] =  { "NULL", "PDU_SET_MARKING", "DYN_CHANGING_TRAFFIC_CHAR", "EXPEDITED_TRANSFER_IND" };
    size_t sizeofArray = sizeof(rtp_header_ext_typeArray) / sizeof(rtp_header_ext_typeArray[0]);
    if (rtp_header_ext_type < sizeofArray)
        return (char *)rtp_header_ext_typeArray[rtp_header_ext_type];
    else
        return (char *)"Unknown";
}

OpenAPI_rtp_header_ext_type_e OpenAPI_rtp_header_ext_type_FromString(char* rtp_header_ext_type)
{
    int stringToReturn = 0;
    const char *rtp_header_ext_typeArray[] =  { "NULL", "PDU_SET_MARKING", "DYN_CHANGING_TRAFFIC_CHAR", "EXPEDITED_TRANSFER_IND" };
    size_t sizeofArray = sizeof(rtp_header_ext_typeArray) / sizeof(rtp_header_ext_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rtp_header_ext_type, rtp_header_ext_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

