/*
 * slice_scope_per_plmn.h
 *
 * contains the list of network slices for MDT per PLMN.
 */

#ifndef _OpenAPI_slice_scope_per_plmn_H_
#define _OpenAPI_slice_scope_per_plmn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_scope_per_plmn_s OpenAPI_slice_scope_per_plmn_t;
#include "plmn_id.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_scope_per_plmn_s {
    struct OpenAPI_plmn_id_s *plmn;
    OpenAPI_list_t *slice_scope;
};

OpenAPI_slice_scope_per_plmn_t *OpenAPI_slice_scope_per_plmn_create(
    OpenAPI_plmn_id_t *plmn,
    OpenAPI_list_t *slice_scope
);
void OpenAPI_slice_scope_per_plmn_free(OpenAPI_slice_scope_per_plmn_t *slice_scope_per_plmn);
OpenAPI_slice_scope_per_plmn_t *OpenAPI_slice_scope_per_plmn_parseFromJSON(cJSON *slice_scope_per_plmnJSON);
cJSON *OpenAPI_slice_scope_per_plmn_convertToJSON(OpenAPI_slice_scope_per_plmn_t *slice_scope_per_plmn);
OpenAPI_slice_scope_per_plmn_t *OpenAPI_slice_scope_per_plmn_copy(OpenAPI_slice_scope_per_plmn_t *dst, OpenAPI_slice_scope_per_plmn_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_scope_per_plmn_H_ */

