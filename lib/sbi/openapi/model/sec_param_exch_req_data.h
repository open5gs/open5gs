/*
 * sec_param_exch_req_data.h
 *
 * Request data structure for parameter exchange
 */

#ifndef _OpenAPI_sec_param_exch_req_data_H_
#define _OpenAPI_sec_param_exch_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ipx_provider_sec_info.h"
#include "protection_policy.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sec_param_exch_req_data_s OpenAPI_sec_param_exch_req_data_t;
typedef struct OpenAPI_sec_param_exch_req_data_s {
    char *n32f_context_id;
    OpenAPI_list_t *jwe_cipher_suite_list;
    OpenAPI_list_t *jws_cipher_suite_list;
    struct OpenAPI_protection_policy_s *protection_policy_info;
    OpenAPI_list_t *ipx_provider_sec_info_list;
    char *sender;
} OpenAPI_sec_param_exch_req_data_t;

OpenAPI_sec_param_exch_req_data_t *OpenAPI_sec_param_exch_req_data_create(
    char *n32f_context_id,
    OpenAPI_list_t *jwe_cipher_suite_list,
    OpenAPI_list_t *jws_cipher_suite_list,
    OpenAPI_protection_policy_t *protection_policy_info,
    OpenAPI_list_t *ipx_provider_sec_info_list,
    char *sender
);
void OpenAPI_sec_param_exch_req_data_free(OpenAPI_sec_param_exch_req_data_t *sec_param_exch_req_data);
OpenAPI_sec_param_exch_req_data_t *OpenAPI_sec_param_exch_req_data_parseFromJSON(cJSON *sec_param_exch_req_dataJSON);
cJSON *OpenAPI_sec_param_exch_req_data_convertToJSON(OpenAPI_sec_param_exch_req_data_t *sec_param_exch_req_data);
OpenAPI_sec_param_exch_req_data_t *OpenAPI_sec_param_exch_req_data_copy(OpenAPI_sec_param_exch_req_data_t *dst, OpenAPI_sec_param_exch_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sec_param_exch_req_data_H_ */

