
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rtp_payload_format.h"

char* OpenAPI_rtp_payload_format_ToString(OpenAPI_rtp_payload_format_e rtp_payload_format)
{
    const char *rtp_payload_formatArray[] =  { "NULL", "H264", "H265" };
    size_t sizeofArray = sizeof(rtp_payload_formatArray) / sizeof(rtp_payload_formatArray[0]);
    if (rtp_payload_format < sizeofArray)
        return (char *)rtp_payload_formatArray[rtp_payload_format];
    else
        return (char *)"Unknown";
}

OpenAPI_rtp_payload_format_e OpenAPI_rtp_payload_format_FromString(char* rtp_payload_format)
{
    int stringToReturn = 0;
    const char *rtp_payload_formatArray[] =  { "NULL", "H264", "H265" };
    size_t sizeofArray = sizeof(rtp_payload_formatArray) / sizeof(rtp_payload_formatArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rtp_payload_format, rtp_payload_formatArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

