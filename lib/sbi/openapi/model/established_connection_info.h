/*
 * established_connection_info.h
 *
 * 
 */

#ifndef _OpenAPI_established_connection_info_H_
#define _OpenAPI_established_connection_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_established_connection_info_s OpenAPI_established_connection_info_t;
#include "global_ran_node_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_established_connection_info_s {
    OpenAPI_list_t *u_pfids;
    OpenAPI_list_t *ran_node_ids;
};

OpenAPI_established_connection_info_t *OpenAPI_established_connection_info_create(
    OpenAPI_list_t *u_pfids,
    OpenAPI_list_t *ran_node_ids
);
void OpenAPI_established_connection_info_free(OpenAPI_established_connection_info_t *established_connection_info);
OpenAPI_established_connection_info_t *OpenAPI_established_connection_info_parseFromJSON(cJSON *established_connection_infoJSON);
cJSON *OpenAPI_established_connection_info_convertToJSON(OpenAPI_established_connection_info_t *established_connection_info);
OpenAPI_established_connection_info_t *OpenAPI_established_connection_info_copy(OpenAPI_established_connection_info_t *dst, OpenAPI_established_connection_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_established_connection_info_H_ */

