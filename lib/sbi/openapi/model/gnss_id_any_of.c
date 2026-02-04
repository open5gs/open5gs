
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gnss_id_any_of.h"

char* OpenAPI_gnss_id_any_of_ToString(OpenAPI_gnss_id_any_of_e gnss_id_any_of)
{
    const char *gnss_id_any_ofArray[] =  { "NULL", "GPS", "GALILEO", "SBAS", "MODERNIZED_GPS", "QZSS", "GLONASS", "BDS", "NAVIC" };
    size_t sizeofArray = sizeof(gnss_id_any_ofArray) / sizeof(gnss_id_any_ofArray[0]);
    if (gnss_id_any_of < sizeofArray)
        return (char *)gnss_id_any_ofArray[gnss_id_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_gnss_id_any_of_e OpenAPI_gnss_id_any_of_FromString(char* gnss_id_any_of)
{
    int stringToReturn = 0;
    const char *gnss_id_any_ofArray[] =  { "NULL", "GPS", "GALILEO", "SBAS", "MODERNIZED_GPS", "QZSS", "GLONASS", "BDS", "NAVIC" };
    size_t sizeofArray = sizeof(gnss_id_any_ofArray) / sizeof(gnss_id_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(gnss_id_any_of, gnss_id_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

