/*
 * pfd_data.h
 *
 * List of Application IDs and/or AF IDs managed by a given NEF Instance
 */

#ifndef _OpenAPI_pfd_data_H_
#define _OpenAPI_pfd_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pfd_data_s OpenAPI_pfd_data_t;
typedef struct OpenAPI_pfd_data_s {
    OpenAPI_list_t *app_ids;
    OpenAPI_list_t *af_ids;
} OpenAPI_pfd_data_t;

OpenAPI_pfd_data_t *OpenAPI_pfd_data_create(
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *af_ids
);
void OpenAPI_pfd_data_free(OpenAPI_pfd_data_t *pfd_data);
OpenAPI_pfd_data_t *OpenAPI_pfd_data_parseFromJSON(cJSON *pfd_dataJSON);
cJSON *OpenAPI_pfd_data_convertToJSON(OpenAPI_pfd_data_t *pfd_data);
OpenAPI_pfd_data_t *OpenAPI_pfd_data_copy(OpenAPI_pfd_data_t *dst, OpenAPI_pfd_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfd_data_H_ */

