/*
 * shared_scope.h
 *
 * Authorized Scope for a Shared Data
 */

#ifndef _OpenAPI_shared_scope_H_
#define _OpenAPI_shared_scope_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_shared_scope_s OpenAPI_shared_scope_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_shared_scope_s {
    OpenAPI_list_t *nf_set_id_list;
};

OpenAPI_shared_scope_t *OpenAPI_shared_scope_create(
    OpenAPI_list_t *nf_set_id_list
);
void OpenAPI_shared_scope_free(OpenAPI_shared_scope_t *shared_scope);
OpenAPI_shared_scope_t *OpenAPI_shared_scope_parseFromJSON(cJSON *shared_scopeJSON);
cJSON *OpenAPI_shared_scope_convertToJSON(OpenAPI_shared_scope_t *shared_scope);
OpenAPI_shared_scope_t *OpenAPI_shared_scope_copy(OpenAPI_shared_scope_t *dst, OpenAPI_shared_scope_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_shared_scope_H_ */

