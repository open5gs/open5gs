
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "summarization_attribute.h"

char* OpenAPI_summarization_attribute_ToString(OpenAPI_summarization_attribute_e summarization_attribute)
{
    const char *summarization_attributeArray[] =  { "NULL", "SPACING", "DURATION", "OCCURRENCES", "AVG_VAR", "FREQ_VAL", "MIN_MAX" };
    size_t sizeofArray = sizeof(summarization_attributeArray) / sizeof(summarization_attributeArray[0]);
    if (summarization_attribute < sizeofArray)
        return (char *)summarization_attributeArray[summarization_attribute];
    else
        return (char *)"Unknown";
}

OpenAPI_summarization_attribute_e OpenAPI_summarization_attribute_FromString(char* summarization_attribute)
{
    int stringToReturn = 0;
    const char *summarization_attributeArray[] =  { "NULL", "SPACING", "DURATION", "OCCURRENCES", "AVG_VAR", "FREQ_VAL", "MIN_MAX" };
    size_t sizeofArray = sizeof(summarization_attributeArray) / sizeof(summarization_attributeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(summarization_attribute, summarization_attributeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

