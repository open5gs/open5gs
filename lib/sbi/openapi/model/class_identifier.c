
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "class_identifier.h"

char* OpenAPI_class_identifier_ToString(OpenAPI_class_identifier_e class_identifier)
{
    const char *class_identifierArray[] =  { "NULL", "PERSONAL", "ADVERTISEMENT", "INFORMATIONAL", "AUTO" };
    size_t sizeofArray = sizeof(class_identifierArray) / sizeof(class_identifierArray[0]);
    if (class_identifier < sizeofArray)
        return (char *)class_identifierArray[class_identifier];
    else
        return (char *)"Unknown";
}

OpenAPI_class_identifier_e OpenAPI_class_identifier_FromString(char* class_identifier)
{
    int stringToReturn = 0;
    const char *class_identifierArray[] =  { "NULL", "PERSONAL", "ADVERTISEMENT", "INFORMATIONAL", "AUTO" };
    size_t sizeofArray = sizeof(class_identifierArray) / sizeof(class_identifierArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(class_identifier, class_identifierArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

