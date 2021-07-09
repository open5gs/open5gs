/*
 * hss_authentication_info_request.h
 *
 * 
 */

#ifndef _OpenAPI_hss_authentication_info_request_H_
#define _OpenAPI_hss_authentication_info_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_network_id.h"
#include "hss_auth_type.h"
#include "node_type.h"
#include "plmn_id.h"
#include "resynchronization_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hss_authentication_info_request_s OpenAPI_hss_authentication_info_request_t;
typedef struct OpenAPI_hss_authentication_info_request_s {
    char *supported_features;
    OpenAPI_hss_auth_type_e hss_auth_type;
    int num_of_requested_vectors;
    OpenAPI_node_type_e requesting_node_type;
    struct OpenAPI_plmn_id_s *serving_network_id;
    struct OpenAPI_resynchronization_info_s *resynchronization_info;
    OpenAPI_access_network_id_e an_id;
} OpenAPI_hss_authentication_info_request_t;

OpenAPI_hss_authentication_info_request_t *OpenAPI_hss_authentication_info_request_create(
    char *supported_features,
    OpenAPI_hss_auth_type_e hss_auth_type,
    int num_of_requested_vectors,
    OpenAPI_node_type_e requesting_node_type,
    OpenAPI_plmn_id_t *serving_network_id,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    OpenAPI_access_network_id_e an_id
);
void OpenAPI_hss_authentication_info_request_free(OpenAPI_hss_authentication_info_request_t *hss_authentication_info_request);
OpenAPI_hss_authentication_info_request_t *OpenAPI_hss_authentication_info_request_parseFromJSON(cJSON *hss_authentication_info_requestJSON);
cJSON *OpenAPI_hss_authentication_info_request_convertToJSON(OpenAPI_hss_authentication_info_request_t *hss_authentication_info_request);
OpenAPI_hss_authentication_info_request_t *OpenAPI_hss_authentication_info_request_copy(OpenAPI_hss_authentication_info_request_t *dst, OpenAPI_hss_authentication_info_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_authentication_info_request_H_ */

