
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_reason.h"

char* OpenAPI_transfer_reason_ToString(OpenAPI_transfer_reason_e transfer_reason)
{
    const char *transfer_reasonArray[] =  { "NULL", "INIT_REG", "MOBI_REG", "MOBI_REG_UE_VALIDATED" };
    size_t sizeofArray = sizeof(transfer_reasonArray) / sizeof(transfer_reasonArray[0]);
    if (transfer_reason < sizeofArray)
        return (char *)transfer_reasonArray[transfer_reason];
    else
        return (char *)"Unknown";
}

OpenAPI_transfer_reason_e OpenAPI_transfer_reason_FromString(char* transfer_reason)
{
    int stringToReturn = 0;
    const char *transfer_reasonArray[] =  { "NULL", "INIT_REG", "MOBI_REG", "MOBI_REG_UE_VALIDATED" };
    size_t sizeofArray = sizeof(transfer_reasonArray) / sizeof(transfer_reasonArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(transfer_reason, transfer_reasonArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

