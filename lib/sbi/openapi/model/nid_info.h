/*
 * nid_info.h
 *
 * contains NIDs.
 */

#ifndef _OpenAPI_nid_info_H_
#define _OpenAPI_nid_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nid_info_s OpenAPI_nid_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nid_info_s {
    OpenAPI_list_t *nid_list;
};

OpenAPI_nid_info_t *OpenAPI_nid_info_create(
    OpenAPI_list_t *nid_list
);
void OpenAPI_nid_info_free(OpenAPI_nid_info_t *nid_info);
OpenAPI_nid_info_t *OpenAPI_nid_info_parseFromJSON(cJSON *nid_infoJSON);
cJSON *OpenAPI_nid_info_convertToJSON(OpenAPI_nid_info_t *nid_info);
OpenAPI_nid_info_t *OpenAPI_nid_info_copy(OpenAPI_nid_info_t *dst, OpenAPI_nid_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nid_info_H_ */

