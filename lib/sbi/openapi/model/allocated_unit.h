/*
 * allocated_unit.h
 *
 * 
 */

#ifndef _OpenAPI_allocated_unit_H_
#define _OpenAPI_allocated_unit_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_allocated_unit_s OpenAPI_allocated_unit_t;
#include "charging_trigger.h"
#include "nsac_container_information.h"
#include "quota_management_indicator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_allocated_unit_s {
    OpenAPI_quota_management_indicator_e quota_management_indicator;
    OpenAPI_list_t *triggers;
    char *trigger_timestamp;
    int local_sequence_number;
    struct OpenAPI_nsac_container_information_s *n_sac_container_information;
};

OpenAPI_allocated_unit_t *OpenAPI_allocated_unit_create(
    OpenAPI_quota_management_indicator_e quota_management_indicator,
    OpenAPI_list_t *triggers,
    char *trigger_timestamp,
    int local_sequence_number,
    OpenAPI_nsac_container_information_t *n_sac_container_information
);
void OpenAPI_allocated_unit_free(OpenAPI_allocated_unit_t *allocated_unit);
OpenAPI_allocated_unit_t *OpenAPI_allocated_unit_parseFromJSON(cJSON *allocated_unitJSON);
cJSON *OpenAPI_allocated_unit_convertToJSON(OpenAPI_allocated_unit_t *allocated_unit);
OpenAPI_allocated_unit_t *OpenAPI_allocated_unit_copy(OpenAPI_allocated_unit_t *dst, OpenAPI_allocated_unit_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allocated_unit_H_ */

