/*
 * user_plane_address.h
 *
 * 
 */

#ifndef _OpenAPI_user_plane_address_H_
#define _OpenAPI_user_plane_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_user_plane_address_s OpenAPI_user_plane_address_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_user_plane_address_s {
    char *value;
};

OpenAPI_user_plane_address_t *OpenAPI_user_plane_address_create(
char *value
);
void OpenAPI_user_plane_address_free(OpenAPI_user_plane_address_t *user_plane_address);
OpenAPI_user_plane_address_t *OpenAPI_user_plane_address_parseFromJSON(cJSON *user_plane_addressJSON);
cJSON *OpenAPI_user_plane_address_convertToJSON(OpenAPI_user_plane_address_t *user_plane_address);
OpenAPI_user_plane_address_t *OpenAPI_user_plane_address_copy(OpenAPI_user_plane_address_t *dst, OpenAPI_user_plane_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_user_plane_address_H_ */

