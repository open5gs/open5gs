
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mdt_user_consent.h"

char* OpenAPI_mdt_user_consent_ToString(OpenAPI_mdt_user_consent_e mdt_user_consent)
{
    const char *mdt_user_consentArray[] =  { "NULL", "CONSENT_NOT_GIVEN", "CONSENT_GIVEN" };
    size_t sizeofArray = sizeof(mdt_user_consentArray) / sizeof(mdt_user_consentArray[0]);
    if (mdt_user_consent < sizeofArray)
        return (char *)mdt_user_consentArray[mdt_user_consent];
    else
        return (char *)"Unknown";
}

OpenAPI_mdt_user_consent_e OpenAPI_mdt_user_consent_FromString(char* mdt_user_consent)
{
    int stringToReturn = 0;
    const char *mdt_user_consentArray[] =  { "NULL", "CONSENT_NOT_GIVEN", "CONSENT_GIVEN" };
    size_t sizeofArray = sizeof(mdt_user_consentArray) / sizeof(mdt_user_consentArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mdt_user_consent, mdt_user_consentArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

