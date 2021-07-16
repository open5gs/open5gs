/*
 * redirect_information.h
 *
 * 
 */

#ifndef _OpenAPI_redirect_information_H_
#define _OpenAPI_redirect_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "redirect_address_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_redirect_information_s OpenAPI_redirect_information_t;
typedef struct OpenAPI_redirect_information_s {
    bool is_redirect_enabled;
    int redirect_enabled;
    OpenAPI_redirect_address_type_e redirect_address_type;
    char *redirect_server_address;
} OpenAPI_redirect_information_t;

OpenAPI_redirect_information_t *OpenAPI_redirect_information_create(
    bool is_redirect_enabled,
    int redirect_enabled,
    OpenAPI_redirect_address_type_e redirect_address_type,
    char *redirect_server_address
);
void OpenAPI_redirect_information_free(OpenAPI_redirect_information_t *redirect_information);
OpenAPI_redirect_information_t *OpenAPI_redirect_information_parseFromJSON(cJSON *redirect_informationJSON);
cJSON *OpenAPI_redirect_information_convertToJSON(OpenAPI_redirect_information_t *redirect_information);
OpenAPI_redirect_information_t *OpenAPI_redirect_information_copy(OpenAPI_redirect_information_t *dst, OpenAPI_redirect_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redirect_information_H_ */

