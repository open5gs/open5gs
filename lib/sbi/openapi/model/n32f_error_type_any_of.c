
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32f_error_type_any_of.h"

char* OpenAPI_n32f_error_type_any_of_ToString(OpenAPI_n32f_error_type_any_of_e n32f_error_type_any_of)
{
    const char *n32f_error_type_any_ofArray[] =  { "NULL", "INTEGRITY_CHECK_FAILED", "INTEGRITY_CHECK_ON_MODIFICATIONS_FAILED", "MODIFICATIONS_INSTRUCTIONS_FAILED", "DECIPHERING_FAILED", "MESSAGE_RECONSTRUCTION_FAILED", "CONTEXT_NOT_FOUND", "INTEGRITY_KEY_EXPIRED", "ENCRYPTION_KEY_EXPIRED", "POLICY_MISMATCH" };
    size_t sizeofArray = sizeof(n32f_error_type_any_ofArray) / sizeof(n32f_error_type_any_ofArray[0]);
    if (n32f_error_type_any_of < sizeofArray)
        return (char *)n32f_error_type_any_ofArray[n32f_error_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_n32f_error_type_any_of_e OpenAPI_n32f_error_type_any_of_FromString(char* n32f_error_type_any_of)
{
    int stringToReturn = 0;
    const char *n32f_error_type_any_ofArray[] =  { "NULL", "INTEGRITY_CHECK_FAILED", "INTEGRITY_CHECK_ON_MODIFICATIONS_FAILED", "MODIFICATIONS_INSTRUCTIONS_FAILED", "DECIPHERING_FAILED", "MESSAGE_RECONSTRUCTION_FAILED", "CONTEXT_NOT_FOUND", "INTEGRITY_KEY_EXPIRED", "ENCRYPTION_KEY_EXPIRED", "POLICY_MISMATCH" };
    size_t sizeofArray = sizeof(n32f_error_type_any_ofArray) / sizeof(n32f_error_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n32f_error_type_any_of, n32f_error_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

