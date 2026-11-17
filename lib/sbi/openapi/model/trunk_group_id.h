/*
 * trunk_group_id.h
 *
 * 
 */

#ifndef _OpenAPI_trunk_group_id_H_
#define _OpenAPI_trunk_group_id_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_trunk_group_id_s OpenAPI_trunk_group_id_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_trunk_group_id_s {
    char *incoming_trunk_group_id;
    char *outgoing_trunk_group_id;
};

OpenAPI_trunk_group_id_t *OpenAPI_trunk_group_id_create(
    char *incoming_trunk_group_id,
    char *outgoing_trunk_group_id
);
void OpenAPI_trunk_group_id_free(OpenAPI_trunk_group_id_t *trunk_group_id);
OpenAPI_trunk_group_id_t *OpenAPI_trunk_group_id_parseFromJSON(cJSON *trunk_group_idJSON);
cJSON *OpenAPI_trunk_group_id_convertToJSON(OpenAPI_trunk_group_id_t *trunk_group_id);
OpenAPI_trunk_group_id_t *OpenAPI_trunk_group_id_copy(OpenAPI_trunk_group_id_t *dst, OpenAPI_trunk_group_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trunk_group_id_H_ */

