
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_set_handling_info.h"

char* OpenAPI_pdu_set_handling_info_ToString(OpenAPI_pdu_set_handling_info_e pdu_set_handling_info)
{
    const char *pdu_set_handling_infoArray[] =  { "NULL", "ALL_PDUS_NEEDED", "ALL_PDUS_NOT_NEEDED" };
    size_t sizeofArray = sizeof(pdu_set_handling_infoArray) / sizeof(pdu_set_handling_infoArray[0]);
    if (pdu_set_handling_info < sizeofArray)
        return (char *)pdu_set_handling_infoArray[pdu_set_handling_info];
    else
        return (char *)"Unknown";
}

OpenAPI_pdu_set_handling_info_e OpenAPI_pdu_set_handling_info_FromString(char* pdu_set_handling_info)
{
    int stringToReturn = 0;
    const char *pdu_set_handling_infoArray[] =  { "NULL", "ALL_PDUS_NEEDED", "ALL_PDUS_NOT_NEEDED" };
    size_t sizeofArray = sizeof(pdu_set_handling_infoArray) / sizeof(pdu_set_handling_infoArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pdu_set_handling_info, pdu_set_handling_infoArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

