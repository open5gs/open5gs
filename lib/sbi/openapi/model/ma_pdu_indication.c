
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ma_pdu_indication.h"

char* OpenAPI_ma_pdu_indication_ToString(OpenAPI_ma_pdu_indication_e ma_pdu_indication)
{
    const char *ma_pdu_indicationArray[] =  { "NULL", "MA_PDU_REQUEST", "MA_PDU_NETWORK_UPGRADE_ALLOWED" };
    size_t sizeofArray = sizeof(ma_pdu_indicationArray) / sizeof(ma_pdu_indicationArray[0]);
    if (ma_pdu_indication < sizeofArray)
        return (char *)ma_pdu_indicationArray[ma_pdu_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_ma_pdu_indication_e OpenAPI_ma_pdu_indication_FromString(char* ma_pdu_indication)
{
    int stringToReturn = 0;
    const char *ma_pdu_indicationArray[] =  { "NULL", "MA_PDU_REQUEST", "MA_PDU_NETWORK_UPGRADE_ALLOWED" };
    size_t sizeofArray = sizeof(ma_pdu_indicationArray) / sizeof(ma_pdu_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ma_pdu_indication, ma_pdu_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

