/*
 * iptv_config_data.h
 *
 * Represents IPTV configuration data information.
 */

#ifndef _OpenAPI_iptv_config_data_H_
#define _OpenAPI_iptv_config_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "any_type.h"
#include "multicast_access_control.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_iptv_config_data_s OpenAPI_iptv_config_data_t;
typedef struct OpenAPI_iptv_config_data_s {
    char *supi;
    bool is_inter_group_id_null;
    OpenAPI_any_type_t *inter_group_id;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *af_app_id;
    OpenAPI_list_t* multi_acc_ctrls;
    char *supp_feat;
    char *res_uri;
    OpenAPI_list_t *reset_ids;
} OpenAPI_iptv_config_data_t;

OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_create(
    char *supi,
    bool is_inter_group_id_null,
    OpenAPI_any_type_t *inter_group_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *af_app_id,
    OpenAPI_list_t* multi_acc_ctrls,
    char *supp_feat,
    char *res_uri,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_iptv_config_data_free(OpenAPI_iptv_config_data_t *iptv_config_data);
OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_parseFromJSON(cJSON *iptv_config_dataJSON);
cJSON *OpenAPI_iptv_config_data_convertToJSON(OpenAPI_iptv_config_data_t *iptv_config_data);
OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_copy(OpenAPI_iptv_config_data_t *dst, OpenAPI_iptv_config_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_iptv_config_data_H_ */

