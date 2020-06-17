/*
 * af_external.h
 *
 *
 */

#ifndef _OpenAPI_af_external_H_
#define _OpenAPI_af_external_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geographic_area.h"
#include "privacy_check_related_action.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_external_s OpenAPI_af_external_t;
typedef struct OpenAPI_af_external_s {
    char *af_id;
    OpenAPI_list_t *allowed_geographic_area;
    OpenAPI_privacy_check_related_action_e privacy_check_related_action;
    struct OpenAPI_valid_time_period_s *valid_time_period;
} OpenAPI_af_external_t;

OpenAPI_af_external_t *OpenAPI_af_external_create(
    char *af_id,
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_privacy_check_related_action_e privacy_check_related_action,
    OpenAPI_valid_time_period_t *valid_time_period
    );
void OpenAPI_af_external_free(OpenAPI_af_external_t *af_external);
OpenAPI_af_external_t *OpenAPI_af_external_parseFromJSON(cJSON *af_externalJSON);
cJSON *OpenAPI_af_external_convertToJSON(OpenAPI_af_external_t *af_external);
OpenAPI_af_external_t *OpenAPI_af_external_copy(OpenAPI_af_external_t *dst, OpenAPI_af_external_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_external_H_ */

