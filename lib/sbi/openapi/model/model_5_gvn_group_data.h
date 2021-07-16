/*
 * model_5_gvn_group_data.h
 *
 * 
 */

#ifndef _OpenAPI_model_5_gvn_group_data_H_
#define _OpenAPI_model_5_gvn_group_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "app_descriptor.h"
#include "ip_address_1.h"
#include "pdu_session_type.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_model_5_gvn_group_data_s OpenAPI_model_5_gvn_group_data_t;
typedef struct OpenAPI_model_5_gvn_group_data_s {
    char *dnn;
    struct OpenAPI_snssai_s *s_nssai;
    OpenAPI_list_t *pdu_session_types;
    OpenAPI_list_t *app_descriptors;
    bool is_secondary_auth;
    int secondary_auth;
    struct OpenAPI_ip_address_1_s *dn_aaa_address;
} OpenAPI_model_5_gvn_group_data_t;

OpenAPI_model_5_gvn_group_data_t *OpenAPI_model_5_gvn_group_data_create(
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_list_t *app_descriptors,
    bool is_secondary_auth,
    int secondary_auth,
    OpenAPI_ip_address_1_t *dn_aaa_address
);
void OpenAPI_model_5_gvn_group_data_free(OpenAPI_model_5_gvn_group_data_t *model_5_gvn_group_data);
OpenAPI_model_5_gvn_group_data_t *OpenAPI_model_5_gvn_group_data_parseFromJSON(cJSON *model_5_gvn_group_dataJSON);
cJSON *OpenAPI_model_5_gvn_group_data_convertToJSON(OpenAPI_model_5_gvn_group_data_t *model_5_gvn_group_data);
OpenAPI_model_5_gvn_group_data_t *OpenAPI_model_5_gvn_group_data_copy(OpenAPI_model_5_gvn_group_data_t *dst, OpenAPI_model_5_gvn_group_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_gvn_group_data_H_ */

