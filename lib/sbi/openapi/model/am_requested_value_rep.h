/*
 * am_requested_value_rep.h
 *
 * Represents the current applicable values corresponding to the policy control request triggers. 
 */

#ifndef _OpenAPI_am_requested_value_rep_H_
#define _OpenAPI_am_requested_value_rep_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "presence_info.h"
#include "rat_type.h"
#include "snssai.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_am_requested_value_rep_s OpenAPI_am_requested_value_rep_t;
typedef struct OpenAPI_am_requested_value_rep_s {
    struct OpenAPI_user_location_s *user_loc;
    OpenAPI_list_t* pra_statuses;
    OpenAPI_list_t *access_types;
    OpenAPI_list_t *rat_types;
    OpenAPI_list_t *allowed_snssais;
    OpenAPI_list_t *n3g_allowed_snssais;
} OpenAPI_am_requested_value_rep_t;

OpenAPI_am_requested_value_rep_t *OpenAPI_am_requested_value_rep_create(
    OpenAPI_user_location_t *user_loc,
    OpenAPI_list_t* pra_statuses,
    OpenAPI_list_t *access_types,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *allowed_snssais,
    OpenAPI_list_t *n3g_allowed_snssais
);
void OpenAPI_am_requested_value_rep_free(OpenAPI_am_requested_value_rep_t *am_requested_value_rep);
OpenAPI_am_requested_value_rep_t *OpenAPI_am_requested_value_rep_parseFromJSON(cJSON *am_requested_value_repJSON);
cJSON *OpenAPI_am_requested_value_rep_convertToJSON(OpenAPI_am_requested_value_rep_t *am_requested_value_rep);
OpenAPI_am_requested_value_rep_t *OpenAPI_am_requested_value_rep_copy(OpenAPI_am_requested_value_rep_t *dst, OpenAPI_am_requested_value_rep_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_requested_value_rep_H_ */

