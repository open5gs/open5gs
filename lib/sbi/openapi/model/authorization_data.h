/*
 * authorization_data.h
 *
 * 
 */

#ifndef _OpenAPI_authorization_data_H_
#define _OpenAPI_authorization_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "user_identifier.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authorization_data_s OpenAPI_authorization_data_t;
typedef struct OpenAPI_authorization_data_s {
    OpenAPI_list_t *authorization_data;
    char *validity_time;
} OpenAPI_authorization_data_t;

OpenAPI_authorization_data_t *OpenAPI_authorization_data_create(
    OpenAPI_list_t *authorization_data,
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

