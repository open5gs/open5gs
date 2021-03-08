/*
 * registration_data_set_name.h
 *
 *
 */

#ifndef _OpenAPI_registration_data_set_name_H_
#define _OpenAPI_registration_data_set_name_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_registration_data_set_name_s OpenAPI_registration_data_set_name_t;
typedef struct OpenAPI_registration_data_set_name_s {
} OpenAPI_registration_data_set_name_t;

OpenAPI_registration_data_set_name_t *OpenAPI_registration_data_set_name_create(
    );
void OpenAPI_registration_data_set_name_free(OpenAPI_registration_data_set_name_t *registration_data_set_name);
OpenAPI_registration_data_set_name_t *OpenAPI_registration_data_set_name_parseFromJSON(cJSON *registration_data_set_nameJSON);
cJSON *OpenAPI_registration_data_set_name_convertToJSON(OpenAPI_registration_data_set_name_t *registration_data_set_name);
OpenAPI_registration_data_set_name_t *OpenAPI_registration_data_set_name_copy(OpenAPI_registration_data_set_name_t *dst, OpenAPI_registration_data_set_name_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_data_set_name_H_ */

