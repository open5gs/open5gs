
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "termination_indication.h"

char* OpenAPI_termination_indication_ToString(OpenAPI_termination_indication_e termination_indication)
{
    const char *termination_indicationArray[] =  { "NULL", "NEW_UES_TERMINATION", "ALL_UES_TERMINATION" };
    size_t sizeofArray = sizeof(termination_indicationArray) / sizeof(termination_indicationArray[0]);
    if (termination_indication < sizeofArray)
        return (char *)termination_indicationArray[termination_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_termination_indication_e OpenAPI_termination_indication_FromString(char* termination_indication)
{
    int stringToReturn = 0;
    const char *termination_indicationArray[] =  { "NULL", "NEW_UES_TERMINATION", "ALL_UES_TERMINATION" };
    size_t sizeofArray = sizeof(termination_indicationArray) / sizeof(termination_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(termination_indication, termination_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

