/*
 * supported_plmn.h
 *
 * Contains Information about the PLMN and associated ECSPs
 */

#ifndef _OpenAPI_supported_plmn_H_
#define _OpenAPI_supported_plmn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_supported_plmn_s OpenAPI_supported_plmn_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_supported_plmn_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    OpenAPI_list_t *ecsp_ids;
};

OpenAPI_supported_plmn_t *OpenAPI_supported_plmn_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *ecsp_ids
);
void OpenAPI_supported_plmn_free(OpenAPI_supported_plmn_t *supported_plmn);
OpenAPI_supported_plmn_t *OpenAPI_supported_plmn_parseFromJSON(cJSON *supported_plmnJSON);
cJSON *OpenAPI_supported_plmn_convertToJSON(OpenAPI_supported_plmn_t *supported_plmn);
OpenAPI_supported_plmn_t *OpenAPI_supported_plmn_copy(OpenAPI_supported_plmn_t *dst, OpenAPI_supported_plmn_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supported_plmn_H_ */

