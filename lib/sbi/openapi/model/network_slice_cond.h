/*
 * network_slice_cond.h
 *
 * Subscription to a set of NFs, based on the slices (S-NSSAI and NSI) they support
 */

#ifndef _OpenAPI_network_slice_cond_H_
#define _OpenAPI_network_slice_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_network_slice_cond_s OpenAPI_network_slice_cond_t;
typedef struct OpenAPI_network_slice_cond_s {
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *nsi_list;
} OpenAPI_network_slice_cond_t;

OpenAPI_network_slice_cond_t *OpenAPI_network_slice_cond_create(
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *nsi_list
);
void OpenAPI_network_slice_cond_free(OpenAPI_network_slice_cond_t *network_slice_cond);
OpenAPI_network_slice_cond_t *OpenAPI_network_slice_cond_parseFromJSON(cJSON *network_slice_condJSON);
cJSON *OpenAPI_network_slice_cond_convertToJSON(OpenAPI_network_slice_cond_t *network_slice_cond);
OpenAPI_network_slice_cond_t *OpenAPI_network_slice_cond_copy(OpenAPI_network_slice_cond_t *dst, OpenAPI_network_slice_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_slice_cond_H_ */

