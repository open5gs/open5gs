/*
 * sm_policy_snssai_data.h
 *
 * Contains the SM policy data for a given subscriber and S-NSSAI.
 */

#ifndef _OpenAPI_sm_policy_snssai_data_H_
#define _OpenAPI_sm_policy_snssai_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "slice_mbr_1.h"
#include "sm_policy_dnn_data.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_snssai_data_s OpenAPI_sm_policy_snssai_data_t;
typedef struct OpenAPI_sm_policy_snssai_data_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t* sm_policy_dnn_data;
    struct OpenAPI_slice_mbr_1_s *ue_slice_mbr;
} OpenAPI_sm_policy_snssai_data_t;

OpenAPI_sm_policy_snssai_data_t *OpenAPI_sm_policy_snssai_data_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t* sm_policy_dnn_data,
    OpenAPI_slice_mbr_1_t *ue_slice_mbr
);
void OpenAPI_sm_policy_snssai_data_free(OpenAPI_sm_policy_snssai_data_t *sm_policy_snssai_data);
OpenAPI_sm_policy_snssai_data_t *OpenAPI_sm_policy_snssai_data_parseFromJSON(cJSON *sm_policy_snssai_dataJSON);
cJSON *OpenAPI_sm_policy_snssai_data_convertToJSON(OpenAPI_sm_policy_snssai_data_t *sm_policy_snssai_data);
OpenAPI_sm_policy_snssai_data_t *OpenAPI_sm_policy_snssai_data_copy(OpenAPI_sm_policy_snssai_data_t *dst, OpenAPI_sm_policy_snssai_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_snssai_data_H_ */

