/*
 * authorization_data.h
 *
 * NIDD Authorization Information
 */

#ifndef _OpenAPI_authorization_data_H_
#define _OpenAPI_authorization_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mtc_provider.h"
#include "set.h"
#include "snssai.h"
#include "user_identifier.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authorization_data_s OpenAPI_authorization_data_t;
typedef struct OpenAPI_authorization_data_s {
    OpenAPI_set_t *authorization_data;
    OpenAPI_list_t *allowed_dnn_list;
    OpenAPI_list_t *allowed_snssai_list;
    OpenAPI_list_t *allowed_mtc_providers;
    char *validity_time;
} OpenAPI_authorization_data_t;

OpenAPI_authorization_data_t *OpenAPI_authorization_data_create(
    OpenAPI_set_t *authorization_data,
    OpenAPI_list_t *allowed_dnn_list,
    OpenAPI_list_t *allowed_snssai_list,
    OpenAPI_list_t *allowed_mtc_providers,
    char *validity_time
);
void OpenAPI_authorization_data_free(OpenAPI_authorization_data_t *authorization_data);
OpenAPI_authorization_data_t *OpenAPI_authorization_data_parseFromJSON(cJSON *authorization_dataJSON);
cJSON *OpenAPI_authorization_data_convertToJSON(OpenAPI_authorization_data_t *authorization_data);
OpenAPI_authorization_data_t *OpenAPI_authorization_data_copy(OpenAPI_authorization_data_t *dst, OpenAPI_authorization_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authorization_data_H_ */

