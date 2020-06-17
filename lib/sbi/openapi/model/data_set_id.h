/*
 * data_set_id.h
 *
 * Types of data sets stored in UDR
 */

#ifndef _OpenAPI_data_set_id_H_
#define _OpenAPI_data_set_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_data_set_id_s OpenAPI_data_set_id_t;
typedef struct OpenAPI_data_set_id_s {
} OpenAPI_data_set_id_t;

OpenAPI_data_set_id_t *OpenAPI_data_set_id_create(
    );
void OpenAPI_data_set_id_free(OpenAPI_data_set_id_t *data_set_id);
OpenAPI_data_set_id_t *OpenAPI_data_set_id_parseFromJSON(cJSON *data_set_idJSON);
cJSON *OpenAPI_data_set_id_convertToJSON(OpenAPI_data_set_id_t *data_set_id);
OpenAPI_data_set_id_t *OpenAPI_data_set_id_copy(OpenAPI_data_set_id_t *dst, OpenAPI_data_set_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_set_id_H_ */

