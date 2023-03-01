/*
 * dataset_statistical_property_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_dataset_statistical_property_any_of_H_
#define _OpenAPI_dataset_statistical_property_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dataset_statistical_property_any_of_NULL = 0, OpenAPI_dataset_statistical_property_any_of_UNIFORM_DIST_DATA, OpenAPI_dataset_statistical_property_any_of_NO_OUTLIERS } OpenAPI_dataset_statistical_property_any_of_e;

char* OpenAPI_dataset_statistical_property_any_of_ToString(OpenAPI_dataset_statistical_property_any_of_e dataset_statistical_property_any_of);

OpenAPI_dataset_statistical_property_any_of_e OpenAPI_dataset_statistical_property_any_of_FromString(char* dataset_statistical_property_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dataset_statistical_property_any_of_H_ */

