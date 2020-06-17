/*
 * ref_to_binary_data.h
 *
 *
 */

#ifndef _OpenAPI_ref_to_binary_data_H_
#define _OpenAPI_ref_to_binary_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ref_to_binary_data_s OpenAPI_ref_to_binary_data_t;
typedef struct OpenAPI_ref_to_binary_data_s {
    char *content_id;
} OpenAPI_ref_to_binary_data_t;

OpenAPI_ref_to_binary_data_t *OpenAPI_ref_to_binary_data_create(
    char *content_id
    );
void OpenAPI_ref_to_binary_data_free(OpenAPI_ref_to_binary_data_t *ref_to_binary_data);
OpenAPI_ref_to_binary_data_t *OpenAPI_ref_to_binary_data_parseFromJSON(cJSON *ref_to_binary_dataJSON);
cJSON *OpenAPI_ref_to_binary_data_convertToJSON(OpenAPI_ref_to_binary_data_t *ref_to_binary_data);
OpenAPI_ref_to_binary_data_t *OpenAPI_ref_to_binary_data_copy(OpenAPI_ref_to_binary_data_t *dst, OpenAPI_ref_to_binary_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ref_to_binary_data_H_ */

