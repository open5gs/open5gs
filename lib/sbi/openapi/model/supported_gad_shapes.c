
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supported_gad_shapes.h"

char* OpenAPI_supported_gad_shapes_ToString(OpenAPI_supported_gad_shapes_e supported_gad_shapes)
{
    const char *supported_gad_shapesArray[] =  { "NULL", "POINT", "POINT_UNCERTAINTY_CIRCLE", "POINT_UNCERTAINTY_ELLIPSE", "POLYGON", "POINT_ALTITUDE", "POINT_ALTITUDE_UNCERTAINTY", "ELLIPSOID_ARC", "LOCAL_2D_POINT_UNCERTAINTY_ELLIPSE", "LOCAL_3D_POINT_UNCERTAINTY_ELLIPSOID", "DISTANCE_DIRECTION", "RELATIVE_2D_LOCATION_UNCERTAINTY_ELLIPSE", "RELATIVE_3D_LOCATION_UNCERTAINTY_ELLIPSOID" };
    size_t sizeofArray = sizeof(supported_gad_shapesArray) / sizeof(supported_gad_shapesArray[0]);
    if (supported_gad_shapes < sizeofArray)
        return (char *)supported_gad_shapesArray[supported_gad_shapes];
    else
        return (char *)"Unknown";
}

OpenAPI_supported_gad_shapes_e OpenAPI_supported_gad_shapes_FromString(char* supported_gad_shapes)
{
    int stringToReturn = 0;
    const char *supported_gad_shapesArray[] =  { "NULL", "POINT", "POINT_UNCERTAINTY_CIRCLE", "POINT_UNCERTAINTY_ELLIPSE", "POLYGON", "POINT_ALTITUDE", "POINT_ALTITUDE_UNCERTAINTY", "ELLIPSOID_ARC", "LOCAL_2D_POINT_UNCERTAINTY_ELLIPSE", "LOCAL_3D_POINT_UNCERTAINTY_ELLIPSOID", "DISTANCE_DIRECTION", "RELATIVE_2D_LOCATION_UNCERTAINTY_ELLIPSE", "RELATIVE_3D_LOCATION_UNCERTAINTY_ELLIPSOID" };
    size_t sizeofArray = sizeof(supported_gad_shapesArray) / sizeof(supported_gad_shapesArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(supported_gad_shapes, supported_gad_shapesArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

