
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "connection_capabilities_any_of.h"

char* OpenAPI_connection_capabilities_any_of_ToString(OpenAPI_connection_capabilities_any_of_e connection_capabilities_any_of)
{
    const char *connection_capabilities_any_ofArray[] =  { "NULL", "IMS", "MMS", "SUPL", "INTERNET" };
    size_t sizeofArray = sizeof(connection_capabilities_any_ofArray) / sizeof(connection_capabilities_any_ofArray[0]);
    if (connection_capabilities_any_of < sizeofArray)
        return (char *)connection_capabilities_any_ofArray[connection_capabilities_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_connection_capabilities_any_of_e OpenAPI_connection_capabilities_any_of_FromString(char* connection_capabilities_any_of)
{
    int stringToReturn = 0;
    const char *connection_capabilities_any_ofArray[] =  { "NULL", "IMS", "MMS", "SUPL", "INTERNET" };
    size_t sizeofArray = sizeof(connection_capabilities_any_ofArray) / sizeof(connection_capabilities_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(connection_capabilities_any_of, connection_capabilities_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

