/*
 * shared_data_id_list.h
 *
 * Shared Data IDs
 */

#ifndef _OpenAPI_shared_data_id_list_H_
#define _OpenAPI_shared_data_id_list_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_shared_data_id_list_s OpenAPI_shared_data_id_list_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_shared_data_id_list_s {
    OpenAPI_list_t *shared_data_ids;
};

OpenAPI_shared_data_id_list_t *OpenAPI_shared_data_id_list_create(
    OpenAPI_list_t *shared_data_ids
);
void OpenAPI_shared_data_id_list_free(OpenAPI_shared_data_id_list_t *shared_data_id_list);
OpenAPI_shared_data_id_list_t *OpenAPI_shared_data_id_list_parseFromJSON(cJSON *shared_data_id_listJSON);
cJSON *OpenAPI_shared_data_id_list_convertToJSON(OpenAPI_shared_data_id_list_t *shared_data_id_list);
OpenAPI_shared_data_id_list_t *OpenAPI_shared_data_id_list_copy(OpenAPI_shared_data_id_list_t *dst, OpenAPI_shared_data_id_list_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_shared_data_id_list_H_ */

