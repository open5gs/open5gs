/*
 * amf_info.h
 *
 * Information of an AMF NF Instance
 */

#ifndef _OpenAPI_amf_info_H_
#define _OpenAPI_amf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_amf_info_s OpenAPI_amf_info_t;
#include "amf_event_type.h"
#include "guami.h"
#include "n2_interface_amf_info.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_amf_info_s {
    char *amf_set_id;
    char *amf_region_id;
    OpenAPI_list_t *guami_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *backup_info_amf_failure;
    OpenAPI_list_t *backup_info_amf_removal;
    bool is_n2_interface_amf_info_null;
    struct OpenAPI_n2_interface_amf_info_s *n2_interface_amf_info;
    bool is_amf_onboarding_capability;
    int amf_onboarding_capability;
    bool is_high_latency_com;
    int high_latency_com;
    OpenAPI_list_t *amf_events;
    OpenAPI_list_t *pra_id_list;
    bool is_mobile_iab_ind_null;
    bool is_mobile_iab_ind;
    int mobile_iab_ind;
};

OpenAPI_amf_info_t *OpenAPI_amf_info_create(
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *backup_info_amf_failure,
    OpenAPI_list_t *backup_info_amf_removal,
    bool is_n2_interface_amf_info_null,
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info,
    bool is_amf_onboarding_capability,
    int amf_onboarding_capability,
    bool is_high_latency_com,
    int high_latency_com,
    OpenAPI_list_t *amf_events,
    OpenAPI_list_t *pra_id_list,
    bool is_mobile_iab_ind_null,
    bool is_mobile_iab_ind,
    int mobile_iab_ind
);
void OpenAPI_amf_info_free(OpenAPI_amf_info_t *amf_info);
OpenAPI_amf_info_t *OpenAPI_amf_info_parseFromJSON(cJSON *amf_infoJSON);
cJSON *OpenAPI_amf_info_convertToJSON(OpenAPI_amf_info_t *amf_info);
OpenAPI_amf_info_t *OpenAPI_amf_info_copy(OpenAPI_amf_info_t *dst, OpenAPI_amf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_info_H_ */

