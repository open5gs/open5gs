/*
 * ng_ran_target_id.h
 *
 * 
 */

#ifndef _OpenAPI_ng_ran_target_id_H_
#define _OpenAPI_ng_ran_target_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "global_ran_node_id.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ng_ran_target_id_s OpenAPI_ng_ran_target_id_t;
typedef struct OpenAPI_ng_ran_target_id_s {
    struct OpenAPI_global_ran_node_id_s *ran_node_id;
    struct OpenAPI_tai_s *tai;
} OpenAPI_ng_ran_target_id_t;

OpenAPI_ng_ran_target_id_t *OpenAPI_ng_ran_target_id_create(
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_tai_t *tai
);
void OpenAPI_ng_ran_target_id_free(OpenAPI_ng_ran_target_id_t *ng_ran_target_id);
OpenAPI_ng_ran_target_id_t *OpenAPI_ng_ran_target_id_parseFromJSON(cJSON *ng_ran_target_idJSON);
cJSON *OpenAPI_ng_ran_target_id_convertToJSON(OpenAPI_ng_ran_target_id_t *ng_ran_target_id);
OpenAPI_ng_ran_target_id_t *OpenAPI_ng_ran_target_id_copy(OpenAPI_ng_ran_target_id_t *dst, OpenAPI_ng_ran_target_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ng_ran_target_id_H_ */

