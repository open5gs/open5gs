/*
 * redirect_server.h
 *
 * 
 */

#ifndef _OpenAPI_redirect_server_H_
#define _OpenAPI_redirect_server_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_redirect_server_s OpenAPI_redirect_server_t;
#include "charging_redirect_address_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_redirect_server_s {
    OpenAPI_charging_redirect_address_type_e redirect_address_type;
    char *redirect_server_address;
};

OpenAPI_redirect_server_t *OpenAPI_redirect_server_create(
    OpenAPI_charging_redirect_address_type_e redirect_address_type,
    char *redirect_server_address
);
void OpenAPI_redirect_server_free(OpenAPI_redirect_server_t *redirect_server);
OpenAPI_redirect_server_t *OpenAPI_redirect_server_parseFromJSON(cJSON *redirect_serverJSON);
cJSON *OpenAPI_redirect_server_convertToJSON(OpenAPI_redirect_server_t *redirect_server);
OpenAPI_redirect_server_t *OpenAPI_redirect_server_copy(OpenAPI_redirect_server_t *dst, OpenAPI_redirect_server_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redirect_server_H_ */

