/*
 * requested_rule_data.h
 *
 * Contains rule data requested by the PCF to receive information associated with PCC rule(s).
 */

#ifndef _OpenAPI_requested_rule_data_H_
#define _OpenAPI_requested_rule_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "requested_rule_data_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_requested_rule_data_s OpenAPI_requested_rule_data_t;
typedef struct OpenAPI_requested_rule_data_s {
    OpenAPI_list_t *ref_pcc_rule_ids;
    OpenAPI_list_t *req_data;
} OpenAPI_requested_rule_data_t;

OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_list_t *req_data
);
void OpenAPI_requested_rule_data_free(OpenAPI_requested_rule_data_t *requested_rule_data);
OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_parseFromJSON(cJSON *requested_rule_dataJSON);
cJSON *OpenAPI_requested_rule_data_convertToJSON(OpenAPI_requested_rule_data_t *requested_rule_data);
OpenAPI_requested_rule_data_t *OpenAPI_requested_rule_data_copy(OpenAPI_requested_rule_data_t *dst, OpenAPI_requested_rule_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_rule_data_H_ */

