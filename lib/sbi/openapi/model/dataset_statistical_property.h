/*
 * dataset_statistical_property.h
 *
 * 
 */

#ifndef _OpenAPI_dataset_statistical_property_H_
#define _OpenAPI_dataset_statistical_property_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dataset_statistical_property_NULL = 0, OpenAPI_dataset_statistical_property_UNIFORM_DIST_DATA, OpenAPI_dataset_statistical_property_NO_OUTLIERS } OpenAPI_dataset_statistical_property_e;

char* OpenAPI_dataset_statistical_property_ToString(OpenAPI_dataset_statistical_property_e dataset_statistical_property);

OpenAPI_dataset_statistical_property_e OpenAPI_dataset_statistical_property_FromString(char* dataset_statistical_property);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dataset_statistical_property_H_ */

