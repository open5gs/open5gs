/*
 * slice_scope_per_plmn_1.h
 *
 * contains the list of network slices for MDT per PLMN.
 */

#ifndef _OpenAPI_slice_scope_per_plmn_1_H_
#define _OpenAPI_slice_scope_per_plmn_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_scope_per_plmn_1_s OpenAPI_slice_scope_per_plmn_1_t;
#include "plmn_id.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_scope_per_plmn_1_s {
    struct OpenAPI_plmn_id_s *plmn;
    OpenAPI_list_t *slice_scope;
};

OpenAPI_slice_scope_per_plmn_1_t *OpenAPI_slice_scope_per_plmn_1_create(
    OpenAPI_plmn_id_t *plmn,
    OpenAPI_list_t *slice_scope
);
void OpenAPI_slice_scope_per_plmn_1_free(OpenAPI_slice_scope_per_plmn_1_t *slice_scope_per_plmn_1);
OpenAPI_slice_scope_per_plmn_1_t *OpenAPI_slice_scope_per_plmn_1_parseFromJSON(cJSON *slice_scope_per_plmn_1JSON);
cJSON *OpenAPI_slice_scope_per_plmn_1_convertToJSON(OpenAPI_slice_scope_per_plmn_1_t *slice_scope_per_plmn_1);
OpenAPI_slice_scope_per_plmn_1_t *OpenAPI_slice_scope_per_plmn_1_copy(OpenAPI_slice_scope_per_plmn_1_t *dst, OpenAPI_slice_scope_per_plmn_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_scope_per_plmn_1_H_ */

