/*
 * smsf_info.h
 *
 * Specific Data for SMSF
 */

#ifndef _OpenAPI_smsf_info_H_
#define _OpenAPI_smsf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_smsf_info_s OpenAPI_smsf_info_t;
#include "plmn_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_smsf_info_s {
    bool is_roaming_ue_ind;
    int roaming_ue_ind;
    OpenAPI_list_t *remote_plmn_range_list;
};

OpenAPI_smsf_info_t *OpenAPI_smsf_info_create(
    bool is_roaming_ue_ind,
    int roaming_ue_ind,
    OpenAPI_list_t *remote_plmn_range_list
);
void OpenAPI_smsf_info_free(OpenAPI_smsf_info_t *smsf_info);
OpenAPI_smsf_info_t *OpenAPI_smsf_info_parseFromJSON(cJSON *smsf_infoJSON);
cJSON *OpenAPI_smsf_info_convertToJSON(OpenAPI_smsf_info_t *smsf_info);
OpenAPI_smsf_info_t *OpenAPI_smsf_info_copy(OpenAPI_smsf_info_t *dst, OpenAPI_smsf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smsf_info_H_ */

