/*
 * additional_ee_subs_info.h
 *
 * 
 */

#ifndef _OpenAPI_additional_ee_subs_info_H_
#define _OpenAPI_additional_ee_subs_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_subscription_info.h"
#include "hss_subscription_info.h"
#include "smf_subscription_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_additional_ee_subs_info_s OpenAPI_additional_ee_subs_info_t;
typedef struct OpenAPI_additional_ee_subs_info_s {
    OpenAPI_list_t *amf_subscription_info_list;
    struct OpenAPI_smf_subscription_info_s *smf_subscription_info;
    struct OpenAPI_hss_subscription_info_s *hss_subscription_info;
} OpenAPI_additional_ee_subs_info_t;

OpenAPI_additional_ee_subs_info_t *OpenAPI_additional_ee_subs_info_create(
    OpenAPI_list_t *amf_subscription_info_list,
    OpenAPI_smf_subscription_info_t *smf_subscription_info,
    OpenAPI_hss_subscription_info_t *hss_subscription_info
);
void OpenAPI_additional_ee_subs_info_free(OpenAPI_additional_ee_subs_info_t *additional_ee_subs_info);
OpenAPI_additional_ee_subs_info_t *OpenAPI_additional_ee_subs_info_parseFromJSON(cJSON *additional_ee_subs_infoJSON);
cJSON *OpenAPI_additional_ee_subs_info_convertToJSON(OpenAPI_additional_ee_subs_info_t *additional_ee_subs_info);
OpenAPI_additional_ee_subs_info_t *OpenAPI_additional_ee_subs_info_copy(OpenAPI_additional_ee_subs_info_t *dst, OpenAPI_additional_ee_subs_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_ee_subs_info_H_ */

