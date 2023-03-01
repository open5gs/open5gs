/*
 * suci_info.h
 *
 * SUCI information containing Routing Indicator and Home Network Public Key ID
 */

#ifndef _OpenAPI_suci_info_H_
#define _OpenAPI_suci_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_suci_info_s OpenAPI_suci_info_t;
typedef struct OpenAPI_suci_info_s {
    OpenAPI_list_t *routing_inds;
    OpenAPI_list_t *h_nw_pub_key_ids;
} OpenAPI_suci_info_t;

OpenAPI_suci_info_t *OpenAPI_suci_info_create(
    OpenAPI_list_t *routing_inds,
    OpenAPI_list_t *h_nw_pub_key_ids
);
void OpenAPI_suci_info_free(OpenAPI_suci_info_t *suci_info);
OpenAPI_suci_info_t *OpenAPI_suci_info_parseFromJSON(cJSON *suci_infoJSON);
cJSON *OpenAPI_suci_info_convertToJSON(OpenAPI_suci_info_t *suci_info);
OpenAPI_suci_info_t *OpenAPI_suci_info_copy(OpenAPI_suci_info_t *dst, OpenAPI_suci_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_suci_info_H_ */

