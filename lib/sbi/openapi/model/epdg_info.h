/*
 * epdg_info.h
 *
 * Information of the ePDG end-points
 */

#ifndef _OpenAPI_epdg_info_H_
#define _OpenAPI_epdg_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_epdg_info_s OpenAPI_epdg_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_epdg_info_s {
    OpenAPI_list_t *ipv4_endpoint_addresses;
    OpenAPI_list_t *ipv6_endpoint_addresses;
};

OpenAPI_epdg_info_t *OpenAPI_epdg_info_create(
    OpenAPI_list_t *ipv4_endpoint_addresses,
    OpenAPI_list_t *ipv6_endpoint_addresses
);
void OpenAPI_epdg_info_free(OpenAPI_epdg_info_t *epdg_info);
OpenAPI_epdg_info_t *OpenAPI_epdg_info_parseFromJSON(cJSON *epdg_infoJSON);
cJSON *OpenAPI_epdg_info_convertToJSON(OpenAPI_epdg_info_t *epdg_info);
OpenAPI_epdg_info_t *OpenAPI_epdg_info_copy(OpenAPI_epdg_info_t *dst, OpenAPI_epdg_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_epdg_info_H_ */

