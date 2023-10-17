
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32f_error_type.h"

char* OpenAPI_n32f_error_type_ToString(OpenAPI_n32f_error_type_e n32f_error_type)
{
    const char *n32f_error_typeArray[] =  { "NULL", "INTEGRITY_CHECK_FAILED", "INTEGRITY_CHECK_ON_MODIFICATIONS_FAILED", "MODIFICATIONS_INSTRUCTIONS_FAILED", "DECIPHERING_FAILED", "MESSAGE_RECONSTRUCTION_FAILED", "CONTEXT_NOT_FOUND", "INTEGRITY_KEY_EXPIRED", "ENCRYPTION_KEY_EXPIRED", "POLICY_MISMATCH" };
    size_t sizeofArray = sizeof(n32f_error_typeArray) / sizeof(n32f_error_typeArray[0]);
    if (n32f_error_type < sizeofArray)
        return (char *)n32f_error_typeArray[n32f_error_type];
    else
        return (char *)"Unknown";
}

OpenAPI_n32f_error_type_e OpenAPI_n32f_error_type_FromString(char* n32f_error_type)
{
    int stringToReturn = 0;
    const char *n32f_error_typeArray[] =  { "NULL", "INTEGRITY_CHECK_FAILED", "INTEGRITY_CHECK_ON_MODIFICATIONS_FAILED", "MODIFICATIONS_INSTRUCTIONS_FAILED", "DECIPHERING_FAILED", "MESSAGE_RECONSTRUCTION_FAILED", "CONTEXT_NOT_FOUND", "INTEGRITY_KEY_EXPIRED", "ENCRYPTION_KEY_EXPIRED", "POLICY_MISMATCH" };
    size_t sizeofArray = sizeof(n32f_error_typeArray) / sizeof(n32f_error_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n32f_error_type, n32f_error_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

