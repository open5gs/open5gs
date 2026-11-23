/*
 * serving_network_function_id.h
 *
 * 
 */

#ifndef _OpenAPI_serving_network_function_id_H_
#define _OpenAPI_serving_network_function_id_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_serving_network_function_id_s OpenAPI_serving_network_function_id_t;
#include "nf_identification.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_serving_network_function_id_s {
    struct OpenAPI_nf_identification_s *serving_network_function_information;
    char *a_mfid;
};

OpenAPI_serving_network_function_id_t *OpenAPI_serving_network_function_id_create(
    OpenAPI_nf_identification_t *serving_network_function_information,
    char *a_mfid
);
void OpenAPI_serving_network_function_id_free(OpenAPI_serving_network_function_id_t *serving_network_function_id);
OpenAPI_serving_network_function_id_t *OpenAPI_serving_network_function_id_parseFromJSON(cJSON *serving_network_function_idJSON);
cJSON *OpenAPI_serving_network_function_id_convertToJSON(OpenAPI_serving_network_function_id_t *serving_network_function_id);
OpenAPI_serving_network_function_id_t *OpenAPI_serving_network_function_id_copy(OpenAPI_serving_network_function_id_t *dst, OpenAPI_serving_network_function_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_serving_network_function_id_H_ */

