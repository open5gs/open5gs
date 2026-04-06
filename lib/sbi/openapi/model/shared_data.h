/*
 * shared_data.h
 *
 * Shared Data
 */

#ifndef _OpenAPI_shared_data_H_
#define _OpenAPI_shared_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_shared_data_s OpenAPI_shared_data_t;
#include "nf_profile.h"
#include "nf_service.h"
#include "shared_scope.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_shared_data_s {
    char *shared_data_id;
    bool is_shared_profile_data_null;
    struct OpenAPI_nf_profile_s *shared_profile_data;
    struct OpenAPI_nf_service_s *shared_service_data;
    struct OpenAPI_shared_scope_s *authorized_write_scope;
};

OpenAPI_shared_data_t *OpenAPI_shared_data_create(
    char *shared_data_id,
    bool is_shared_profile_data_null,
    OpenAPI_nf_profile_t *shared_profile_data,
    OpenAPI_nf_service_t *shared_service_data,
    OpenAPI_shared_scope_t *authorized_write_scope
);
void OpenAPI_shared_data_free(OpenAPI_shared_data_t *shared_data);
OpenAPI_shared_data_t *OpenAPI_shared_data_parseFromJSON(cJSON *shared_dataJSON);
cJSON *OpenAPI_shared_data_convertToJSON(OpenAPI_shared_data_t *shared_data);
OpenAPI_shared_data_t *OpenAPI_shared_data_copy(OpenAPI_shared_data_t *dst, OpenAPI_shared_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_shared_data_H_ */

