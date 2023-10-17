/*
 * eas_ip_replacement_info.h
 *
 * Contains EAS IP replacement information for a Source and a Target EAS.
 */

#ifndef _OpenAPI_eas_ip_replacement_info_H_
#define _OpenAPI_eas_ip_replacement_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eas_server_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eas_ip_replacement_info_s OpenAPI_eas_ip_replacement_info_t;
typedef struct OpenAPI_eas_ip_replacement_info_s {
    struct OpenAPI_eas_server_address_s *source;
    struct OpenAPI_eas_server_address_s *target;
} OpenAPI_eas_ip_replacement_info_t;

OpenAPI_eas_ip_replacement_info_t *OpenAPI_eas_ip_replacement_info_create(
    OpenAPI_eas_server_address_t *source,
    OpenAPI_eas_server_address_t *target
);
void OpenAPI_eas_ip_replacement_info_free(OpenAPI_eas_ip_replacement_info_t *eas_ip_replacement_info);
OpenAPI_eas_ip_replacement_info_t *OpenAPI_eas_ip_replacement_info_parseFromJSON(cJSON *eas_ip_replacement_infoJSON);
cJSON *OpenAPI_eas_ip_replacement_info_convertToJSON(OpenAPI_eas_ip_replacement_info_t *eas_ip_replacement_info);
OpenAPI_eas_ip_replacement_info_t *OpenAPI_eas_ip_replacement_info_copy(OpenAPI_eas_ip_replacement_info_t *dst, OpenAPI_eas_ip_replacement_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eas_ip_replacement_info_H_ */

