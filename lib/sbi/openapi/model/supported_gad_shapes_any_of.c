
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supported_gad_shapes_any_of.h"

char* OpenAPI_supported_gad_shapes_any_of_ToString(OpenAPI_supported_gad_shapes_any_of_e supported_gad_shapes_any_of)
{
    const char *supported_gad_shapes_any_ofArray[] =  { "NULL", "POINT", "POINT_UNCERTAINTY_CIRCLE", "POINT_UNCERTAINTY_ELLIPSE", "POLYGON", "POINT_ALTITUDE", "POINT_ALTITUDE_UNCERTAINTY", "ELLIPSOID_ARC", "LOCAL_2D_POINT_UNCERTAINTY_ELLIPSE", "LOCAL_3D_POINT_UNCERTAINTY_ELLIPSOID" };
    size_t sizeofArray = sizeof(supported_gad_shapes_any_ofArray) / sizeof(supported_gad_shapes_any_ofArray[0]);
    if (supported_gad_shapes_any_of < sizeofArray)
        return (char *)supported_gad_shapes_any_ofArray[supported_gad_shapes_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_supported_gad_shapes_any_of_e OpenAPI_supported_gad_shapes_any_of_FromString(char* supported_gad_shapes_any_of)
{
    int stringToReturn = 0;
    const char *supported_gad_shapes_any_ofArray[] =  { "NULL", "POINT", "POINT_UNCERTAINTY_CIRCLE", "POINT_UNCERTAINTY_ELLIPSE", "POLYGON", "POINT_ALTITUDE", "POINT_ALTITUDE_UNCERTAINTY", "ELLIPSOID_ARC", "LOCAL_2D_POINT_UNCERTAINTY_ELLIPSE", "LOCAL_3D_POINT_UNCERTAINTY_ELLIPSOID" };
    size_t sizeofArray = sizeof(supported_gad_shapes_any_ofArray) / sizeof(supported_gad_shapes_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(supported_gad_shapes_any_of, supported_gad_shapes_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

