/*
 * non3gpp_device_info.h
 *
 * Represents the non-3gpp device information.
 */

#ifndef _OpenAPI_non3gpp_device_info_H_
#define _OpenAPI_non3gpp_device_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_non3gpp_device_info_s OpenAPI_non3gpp_device_info_t;
#include "user_plane_address.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_non3gpp_device_info_s {
    char *n3g_dev_id;
    struct OpenAPI_user_plane_address_s *n3g_dev_addr;
};

OpenAPI_non3gpp_device_info_t *OpenAPI_non3gpp_device_info_create(
    char *n3g_dev_id,
    OpenAPI_user_plane_address_t *n3g_dev_addr
);
void OpenAPI_non3gpp_device_info_free(OpenAPI_non3gpp_device_info_t *non3gpp_device_info);
OpenAPI_non3gpp_device_info_t *OpenAPI_non3gpp_device_info_parseFromJSON(cJSON *non3gpp_device_infoJSON);
cJSON *OpenAPI_non3gpp_device_info_convertToJSON(OpenAPI_non3gpp_device_info_t *non3gpp_device_info);
OpenAPI_non3gpp_device_info_t *OpenAPI_non3gpp_device_info_copy(OpenAPI_non3gpp_device_info_t *dst, OpenAPI_non3gpp_device_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non3gpp_device_info_H_ */

