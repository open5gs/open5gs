/*
 * bdt_policy_data.h
 *
 * Represents applied BDT policy data.
 */

#ifndef _OpenAPI_bdt_policy_data_H_
#define _OpenAPI_bdt_policy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bdt_policy_data_s OpenAPI_bdt_policy_data_t;
typedef struct OpenAPI_bdt_policy_data_s {
    char *inter_group_id;
    char *supi;
    char *bdt_ref_id;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *res_uri;
    OpenAPI_list_t *reset_ids;
} OpenAPI_bdt_policy_data_t;

OpenAPI_bdt_policy_data_t *OpenAPI_bdt_policy_data_create(
    char *inter_group_id,
    char *supi,
    char *bdt_ref_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *res_uri,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_bdt_policy_data_free(OpenAPI_bdt_policy_data_t *bdt_policy_data);
OpenAPI_bdt_policy_data_t *OpenAPI_bdt_policy_data_parseFromJSON(cJSON *bdt_policy_dataJSON);
cJSON *OpenAPI_bdt_policy_data_convertToJSON(OpenAPI_bdt_policy_data_t *bdt_policy_data);
OpenAPI_bdt_policy_data_t *OpenAPI_bdt_policy_data_copy(OpenAPI_bdt_policy_data_t *dst, OpenAPI_bdt_policy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_policy_data_H_ */

