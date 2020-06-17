/*
 * parameter_over_uu.h
 *
 *
 */

#ifndef _OpenAPI_parameter_over_uu_H_
#define _OpenAPI_parameter_over_uu_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "service_application_server_address.h"
#include "service_to_pdu_session.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_parameter_over_uu_s OpenAPI_parameter_over_uu_t;
typedef struct OpenAPI_parameter_over_uu_s {
    char *expiry;
    OpenAPI_list_t *ser_to_pdu_sess;
    OpenAPI_list_t *ser_to_app_addr;
} OpenAPI_parameter_over_uu_t;

OpenAPI_parameter_over_uu_t *OpenAPI_parameter_over_uu_create(
    char *expiry,
    OpenAPI_list_t *ser_to_pdu_sess,
    OpenAPI_list_t *ser_to_app_addr
    );
void OpenAPI_parameter_over_uu_free(OpenAPI_parameter_over_uu_t *parameter_over_uu);
OpenAPI_parameter_over_uu_t *OpenAPI_parameter_over_uu_parseFromJSON(cJSON *parameter_over_uuJSON);
cJSON *OpenAPI_parameter_over_uu_convertToJSON(OpenAPI_parameter_over_uu_t *parameter_over_uu);
OpenAPI_parameter_over_uu_t *OpenAPI_parameter_over_uu_copy(OpenAPI_parameter_over_uu_t *dst, OpenAPI_parameter_over_uu_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_parameter_over_uu_H_ */

