/*
 * iptv_config_data_patch.h
 *
 * Represents the parameters to request the modification of an IPTV Configuration resource. 
 */

#ifndef _OpenAPI_iptv_config_data_patch_H_
#define _OpenAPI_iptv_config_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "multicast_access_control.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_iptv_config_data_patch_s OpenAPI_iptv_config_data_patch_t;
typedef struct OpenAPI_iptv_config_data_patch_s {
    OpenAPI_list_t* multi_acc_ctrls;
} OpenAPI_iptv_config_data_patch_t;

OpenAPI_iptv_config_data_patch_t *OpenAPI_iptv_config_data_patch_create(
    OpenAPI_list_t* multi_acc_ctrls
);
void OpenAPI_iptv_config_data_patch_free(OpenAPI_iptv_config_data_patch_t *iptv_config_data_patch);
OpenAPI_iptv_config_data_patch_t *OpenAPI_iptv_config_data_patch_parseFromJSON(cJSON *iptv_config_data_patchJSON);
cJSON *OpenAPI_iptv_config_data_patch_convertToJSON(OpenAPI_iptv_config_data_patch_t *iptv_config_data_patch);
OpenAPI_iptv_config_data_patch_t *OpenAPI_iptv_config_data_patch_copy(OpenAPI_iptv_config_data_patch_t *dst, OpenAPI_iptv_config_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_iptv_config_data_patch_H_ */

