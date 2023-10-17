/*
 * confirmation_data_response.h
 *
 * Contains the result of the authentication
 */

#ifndef _OpenAPI_confirmation_data_response_H_
#define _OpenAPI_confirmation_data_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_result.h"
#include "server_addressing_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_confirmation_data_response_s OpenAPI_confirmation_data_response_t;
typedef struct OpenAPI_confirmation_data_response_s {
    OpenAPI_auth_result_e auth_result;
    char *supi;
    char *kseaf;
    OpenAPI_list_t *pvs_info;
} OpenAPI_confirmation_data_response_t;

OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_create(
    OpenAPI_auth_result_e auth_result,
    char *supi,
    char *kseaf,
    OpenAPI_list_t *pvs_info
);
void OpenAPI_confirmation_data_response_free(OpenAPI_confirmation_data_response_t *confirmation_data_response);
OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_parseFromJSON(cJSON *confirmation_data_responseJSON);
cJSON *OpenAPI_confirmation_data_response_convertToJSON(OpenAPI_confirmation_data_response_t *confirmation_data_response);
OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_copy(OpenAPI_confirmation_data_response_t *dst, OpenAPI_confirmation_data_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_confirmation_data_response_H_ */

