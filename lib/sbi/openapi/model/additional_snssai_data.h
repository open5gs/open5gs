/*
 * additional_snssai_data.h
 *
 * Contains additional S-NSSAI Data
 */

#ifndef _OpenAPI_additional_snssai_data_H_
#define _OpenAPI_additional_snssai_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_additional_snssai_data_s OpenAPI_additional_snssai_data_t;
#include "nsac_admission_mode.h"
#include "recur_time.h"
#include "slice_mbr_rm.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_additional_snssai_data_s {
    bool is_required_authn_authz;
    int required_authn_authz;
    bool is_subscribed_ue_slice_mbr_null;
    struct OpenAPI_slice_mbr_rm_s *subscribed_ue_slice_mbr;
    OpenAPI_list_t *subscribed_ns_srg_list;
    OpenAPI_nsac_admission_mode_e nsac_mode;
    struct OpenAPI_valid_time_period_s *valid_time_period;
    OpenAPI_list_t *valid_time_period_list;
    bool is_dereg_inact_timer;
    int dereg_inact_timer;
    bool is_on_demand;
    int on_demand;
};

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_create(
    bool is_required_authn_authz,
    int required_authn_authz,
    bool is_subscribed_ue_slice_mbr_null,
    OpenAPI_slice_mbr_rm_t *subscribed_ue_slice_mbr,
    OpenAPI_list_t *subscribed_ns_srg_list,
    OpenAPI_nsac_admission_mode_e nsac_mode,
    OpenAPI_valid_time_period_t *valid_time_period,
    OpenAPI_list_t *valid_time_period_list,
    bool is_dereg_inact_timer,
    int dereg_inact_timer,
    bool is_on_demand,
    int on_demand
);
void OpenAPI_additional_snssai_data_free(OpenAPI_additional_snssai_data_t *additional_snssai_data);
OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_parseFromJSON(cJSON *additional_snssai_dataJSON);
cJSON *OpenAPI_additional_snssai_data_convertToJSON(OpenAPI_additional_snssai_data_t *additional_snssai_data);
OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_copy(OpenAPI_additional_snssai_data_t *dst, OpenAPI_additional_snssai_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_snssai_data_H_ */

