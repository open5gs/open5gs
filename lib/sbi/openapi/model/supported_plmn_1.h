/*
 * supported_plmn_1.h
 *
 * Contains Information about the PLMN and associated ECSPs
 */

#ifndef _OpenAPI_supported_plmn_1_H_
#define _OpenAPI_supported_plmn_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_supported_plmn_1_s OpenAPI_supported_plmn_1_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_supported_plmn_1_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    OpenAPI_list_t *ecsp_ids;
};

OpenAPI_supported_plmn_1_t *OpenAPI_supported_plmn_1_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *ecsp_ids
);
void OpenAPI_supported_plmn_1_free(OpenAPI_supported_plmn_1_t *supported_plmn_1);
OpenAPI_supported_plmn_1_t *OpenAPI_supported_plmn_1_parseFromJSON(cJSON *supported_plmn_1JSON);
cJSON *OpenAPI_supported_plmn_1_convertToJSON(OpenAPI_supported_plmn_1_t *supported_plmn_1);
OpenAPI_supported_plmn_1_t *OpenAPI_supported_plmn_1_copy(OpenAPI_supported_plmn_1_t *dst, OpenAPI_supported_plmn_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supported_plmn_1_H_ */

