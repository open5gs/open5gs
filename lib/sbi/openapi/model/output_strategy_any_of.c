
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "output_strategy_any_of.h"

char* OpenAPI_output_strategy_any_of_ToString(OpenAPI_output_strategy_any_of_e output_strategy_any_of)
{
    const char *output_strategy_any_ofArray[] =  { "NULL", "BINARY", "GRADIENT" };
    size_t sizeofArray = sizeof(output_strategy_any_ofArray) / sizeof(output_strategy_any_ofArray[0]);
    if (output_strategy_any_of < sizeofArray)
        return (char *)output_strategy_any_ofArray[output_strategy_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_output_strategy_any_of_e OpenAPI_output_strategy_any_of_FromString(char* output_strategy_any_of)
{
    int stringToReturn = 0;
    const char *output_strategy_any_ofArray[] =  { "NULL", "BINARY", "GRADIENT" };
    size_t sizeofArray = sizeof(output_strategy_any_ofArray) / sizeof(output_strategy_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(output_strategy_any_of, output_strategy_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

