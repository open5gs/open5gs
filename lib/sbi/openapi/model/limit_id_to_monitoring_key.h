/*
 * limit_id_to_monitoring_key.h
 *
 * Contains the limit identifier and the corresponding monitoring key for a given S-NSSAI and DNN. 
 */

#ifndef _OpenAPI_limit_id_to_monitoring_key_H_
#define _OpenAPI_limit_id_to_monitoring_key_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_limit_id_to_monitoring_key_s OpenAPI_limit_id_to_monitoring_key_t;
typedef struct OpenAPI_limit_id_to_monitoring_key_s {
    char *limit_id;
    OpenAPI_list_t *monkey;
} OpenAPI_limit_id_to_monitoring_key_t;

OpenAPI_limit_id_to_monitoring_key_t *OpenAPI_limit_id_to_monitoring_key_create(
    char *limit_id,
    OpenAPI_list_t *monkey
);
void OpenAPI_limit_id_to_monitoring_key_free(OpenAPI_limit_id_to_monitoring_key_t *limit_id_to_monitoring_key);
OpenAPI_limit_id_to_monitoring_key_t *OpenAPI_limit_id_to_monitoring_key_parseFromJSON(cJSON *limit_id_to_monitoring_keyJSON);
cJSON *OpenAPI_limit_id_to_monitoring_key_convertToJSON(OpenAPI_limit_id_to_monitoring_key_t *limit_id_to_monitoring_key);
OpenAPI_limit_id_to_monitoring_key_t *OpenAPI_limit_id_to_monitoring_key_copy(OpenAPI_limit_id_to_monitoring_key_t *dst, OpenAPI_limit_id_to_monitoring_key_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_limit_id_to_monitoring_key_H_ */

