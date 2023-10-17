/*
 * shared_data_id_range.h
 *
 * A range of SharedDataIds based on regular-expression matching
 */

#ifndef _OpenAPI_shared_data_id_range_H_
#define _OpenAPI_shared_data_id_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_shared_data_id_range_s OpenAPI_shared_data_id_range_t;
typedef struct OpenAPI_shared_data_id_range_s {
    char *pattern;
} OpenAPI_shared_data_id_range_t;

OpenAPI_shared_data_id_range_t *OpenAPI_shared_data_id_range_create(
    char *pattern
);
void OpenAPI_shared_data_id_range_free(OpenAPI_shared_data_id_range_t *shared_data_id_range);
OpenAPI_shared_data_id_range_t *OpenAPI_shared_data_id_range_parseFromJSON(cJSON *shared_data_id_rangeJSON);
cJSON *OpenAPI_shared_data_id_range_convertToJSON(OpenAPI_shared_data_id_range_t *shared_data_id_range);
OpenAPI_shared_data_id_range_t *OpenAPI_shared_data_id_range_copy(OpenAPI_shared_data_id_range_t *dst, OpenAPI_shared_data_id_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_shared_data_id_range_H_ */

