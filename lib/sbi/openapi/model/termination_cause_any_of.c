
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "termination_cause_any_of.h"

char* OpenAPI_termination_cause_any_of_ToString(OpenAPI_termination_cause_any_of_e termination_cause_any_of)
{
    const char *termination_cause_any_ofArray[] =  { "NULL", "TERMINATION_BY_UE", "TERMINATION_BY_NETWORK", "NORMAL_TERMINATION" };
    size_t sizeofArray = sizeof(termination_cause_any_ofArray) / sizeof(termination_cause_any_ofArray[0]);
    if (termination_cause_any_of < sizeofArray)
        return (char *)termination_cause_any_ofArray[termination_cause_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_termination_cause_any_of_e OpenAPI_termination_cause_any_of_FromString(char* termination_cause_any_of)
{
    int stringToReturn = 0;
    const char *termination_cause_any_ofArray[] =  { "NULL", "TERMINATION_BY_UE", "TERMINATION_BY_NETWORK", "NORMAL_TERMINATION" };
    size_t sizeofArray = sizeof(termination_cause_any_ofArray) / sizeof(termination_cause_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(termination_cause_any_of, termination_cause_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

