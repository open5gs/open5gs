/*
 * additional_snssai_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_additional_snssai_data_1_H_
#define _OpenAPI_additional_snssai_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "slice_mbr_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_additional_snssai_data_1_s OpenAPI_additional_snssai_data_1_t;
typedef struct OpenAPI_additional_snssai_data_1_s {
    bool is_required_authn_authz;
    int required_authn_authz;
    struct OpenAPI_slice_mbr_rm_s *subscribed_ue_slice_mbr;
    OpenAPI_list_t *subscribed_ns_srg_list;
} OpenAPI_additional_snssai_data_1_t;

OpenAPI_additional_snssai_data_1_t *OpenAPI_additional_snssai_data_1_create(
    bool is_required_authn_authz,
    int required_authn_authz,
    OpenAPI_slice_mbr_rm_t *subscribed_ue_slice_mbr,
    OpenAPI_list_t *subscribed_ns_srg_list
);
void OpenAPI_additional_snssai_data_1_free(OpenAPI_additional_snssai_data_1_t *additional_snssai_data_1);
OpenAPI_additional_snssai_data_1_t *OpenAPI_additional_snssai_data_1_parseFromJSON(cJSON *additional_snssai_data_1JSON);
cJSON *OpenAPI_additional_snssai_data_1_convertToJSON(OpenAPI_additional_snssai_data_1_t *additional_snssai_data_1);
OpenAPI_additional_snssai_data_1_t *OpenAPI_additional_snssai_data_1_copy(OpenAPI_additional_snssai_data_1_t *dst, OpenAPI_additional_snssai_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_snssai_data_1_H_ */

