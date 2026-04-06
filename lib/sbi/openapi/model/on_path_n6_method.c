
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "on_path_n6_method.h"

char* OpenAPI_on_path_n6_method_ToString(OpenAPI_on_path_n6_method_e on_path_n6_method)
{
    const char *on_path_n6_methodArray[] =  { "NULL", "CONNECT_UDP" };
    size_t sizeofArray = sizeof(on_path_n6_methodArray) / sizeof(on_path_n6_methodArray[0]);
    if (on_path_n6_method < sizeofArray)
        return (char *)on_path_n6_methodArray[on_path_n6_method];
    else
        return (char *)"Unknown";
}

OpenAPI_on_path_n6_method_e OpenAPI_on_path_n6_method_FromString(char* on_path_n6_method)
{
    int stringToReturn = 0;
    const char *on_path_n6_methodArray[] =  { "NULL", "CONNECT_UDP" };
    size_t sizeofArray = sizeof(on_path_n6_methodArray) / sizeof(on_path_n6_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(on_path_n6_method, on_path_n6_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

