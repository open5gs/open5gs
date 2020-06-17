/*
 * pp_data.h
 *
 *
 */

#ifndef _OpenAPI_pp_data_H_
#define _OpenAPI_pp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "acs_info_rm.h"
#include "communication_characteristics.h"
#include "ec_restriction.h"
#include "expected_ue_behaviour.h"
#include "lcs_privacy.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_data_s OpenAPI_pp_data_t;
typedef struct OpenAPI_pp_data_s {
    struct OpenAPI_communication_characteristics_s *communication_characteristics;
    char *supported_features;
    struct OpenAPI_expected_ue_behaviour_s *expected_ue_behaviour_parameters;
    struct OpenAPI_ec_restriction_s *ec_restriction;
    struct OpenAPI_acs_info_rm_s *acs_info;
    char *stn_sr;
    struct OpenAPI_lcs_privacy_s *lcs_privacy;
} OpenAPI_pp_data_t;

OpenAPI_pp_data_t *OpenAPI_pp_data_create(
    OpenAPI_communication_characteristics_t *communication_characteristics,
    char *supported_features,
    OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour_parameters,
    OpenAPI_ec_restriction_t *ec_restriction,
    OpenAPI_acs_info_rm_t *acs_info,
    char *stn_sr,
    OpenAPI_lcs_privacy_t *lcs_privacy
    );
void OpenAPI_pp_data_free(OpenAPI_pp_data_t *pp_data);
OpenAPI_pp_data_t *OpenAPI_pp_data_parseFromJSON(cJSON *pp_dataJSON);
cJSON *OpenAPI_pp_data_convertToJSON(OpenAPI_pp_data_t *pp_data);
OpenAPI_pp_data_t *OpenAPI_pp_data_copy(OpenAPI_pp_data_t *dst, OpenAPI_pp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_data_H_ */

