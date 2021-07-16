/*
 * dnn_info.h
 *
 * 
 */

#ifndef _OpenAPI_dnn_info_H_
#define _OpenAPI_dnn_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_info_s OpenAPI_dnn_info_t;
typedef struct OpenAPI_dnn_info_s {
    char *dnn;
    bool is_default_dnn_indicator;
    int default_dnn_indicator;
    bool is_lbo_roaming_allowed;
    int lbo_roaming_allowed;
    bool is_iwk_eps_ind;
    int iwk_eps_ind;
    bool is_dnn_barred;
    int dnn_barred;
    bool is_invoke_nef_ind;
    int invoke_nef_ind;
    OpenAPI_list_t *smf_list;
    bool is_same_smf_ind;
    int same_smf_ind;
} OpenAPI_dnn_info_t;

OpenAPI_dnn_info_t *OpenAPI_dnn_info_create(
    char *dnn,
    bool is_default_dnn_indicator,
    int default_dnn_indicator,
    bool is_lbo_roaming_allowed,
    int lbo_roaming_allowed,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    bool is_dnn_barred,
    int dnn_barred,
    bool is_invoke_nef_ind,
    int invoke_nef_ind,
    OpenAPI_list_t *smf_list,
    bool is_same_smf_ind,
    int same_smf_ind
);
void OpenAPI_dnn_info_free(OpenAPI_dnn_info_t *dnn_info);
OpenAPI_dnn_info_t *OpenAPI_dnn_info_parseFromJSON(cJSON *dnn_infoJSON);
cJSON *OpenAPI_dnn_info_convertToJSON(OpenAPI_dnn_info_t *dnn_info);
OpenAPI_dnn_info_t *OpenAPI_dnn_info_copy(OpenAPI_dnn_info_t *dst, OpenAPI_dnn_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_info_H_ */

