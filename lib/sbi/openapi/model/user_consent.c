
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "user_consent.h"

char* OpenAPI_user_consent_ToString(OpenAPI_user_consent_e user_consent)
{
    const char *user_consentArray[] =  { "NULL", "CONSENT_NOT_GIVEN", "CONSENT_GIVEN" };
    size_t sizeofArray = sizeof(user_consentArray) / sizeof(user_consentArray[0]);
    if (user_consent < sizeofArray)
        return (char *)user_consentArray[user_consent];
    else
        return (char *)"Unknown";
}

OpenAPI_user_consent_e OpenAPI_user_consent_FromString(char* user_consent)
{
    int stringToReturn = 0;
    const char *user_consentArray[] =  { "NULL", "CONSENT_NOT_GIVEN", "CONSENT_GIVEN" };
    size_t sizeofArray = sizeof(user_consentArray) / sizeof(user_consentArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(user_consent, user_consentArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

