/*
 * static_ue_ip_address_params.h
 *
 * Static IP address(es), per-DNN, per-S-NSSAI, for a UE (GPSI)
 */

#ifndef _OpenAPI_static_ue_ip_address_params_H_
#define _OpenAPI_static_ue_ip_address_params_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_static_ue_ip_address_params_s OpenAPI_static_ue_ip_address_params_t;
#include "ip_address.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_static_ue_ip_address_params_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *ip_addresses;
};

OpenAPI_static_ue_ip_address_params_t *OpenAPI_static_ue_ip_address_params_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *ip_addresses
);
void OpenAPI_static_ue_ip_address_params_free(OpenAPI_static_ue_ip_address_params_t *static_ue_ip_address_params);
OpenAPI_static_ue_ip_address_params_t *OpenAPI_static_ue_ip_address_params_parseFromJSON(cJSON *static_ue_ip_address_paramsJSON);
cJSON *OpenAPI_static_ue_ip_address_params_convertToJSON(OpenAPI_static_ue_ip_address_params_t *static_ue_ip_address_params);
OpenAPI_static_ue_ip_address_params_t *OpenAPI_static_ue_ip_address_params_copy(OpenAPI_static_ue_ip_address_params_t *dst, OpenAPI_static_ue_ip_address_params_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_static_ue_ip_address_params_H_ */

