/*
 * service_parameter_data.h
 *
 * 
 */

#ifndef _OpenAPI_service_parameter_data_H_
#define _OpenAPI_service_parameter_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_parameter_data_s OpenAPI_service_parameter_data_t;
typedef struct OpenAPI_service_parameter_data_s {
    char *app_id;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *inter_group_id;
    char *supi;
    char *ue_ipv4;
    char *ue_ipv6;
    char *ue_mac;
    bool is_any_ue_ind;
    int any_ue_ind;
    char *param_over_pc5;
    char *param_over_uu;
    char *supp_feat;
    char *res_uri;
} OpenAPI_service_parameter_data_t;

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_create(
    char *app_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    char *supi,
    char *ue_ipv4,
    char *ue_ipv6,
    char *ue_mac,
    bool is_any_ue_ind,
    int any_ue_ind,
    char *param_over_pc5,
    char *param_over_uu,
    char *supp_feat,
    char *res_uri
);
void OpenAPI_service_parameter_data_free(OpenAPI_service_parameter_data_t *service_parameter_data);
OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_parseFromJSON(cJSON *service_parameter_dataJSON);
cJSON *OpenAPI_service_parameter_data_convertToJSON(OpenAPI_service_parameter_data_t *service_parameter_data);
OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_copy(OpenAPI_service_parameter_data_t *dst, OpenAPI_service_parameter_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_parameter_data_H_ */

