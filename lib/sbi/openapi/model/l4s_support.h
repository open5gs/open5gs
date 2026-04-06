/*
 * l4s_support.h
 *
 * Indicates whether the ECN marking for L4S support is not available or available again in 5GS. 
 */

#ifndef _OpenAPI_l4s_support_H_
#define _OpenAPI_l4s_support_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_l4s_support_s OpenAPI_l4s_support_t;
#include "flows.h"
#include "l4s_notif_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_l4s_support_s {
    OpenAPI_l4s_notif_type_e notif_type;
    OpenAPI_list_t *flows;
};

OpenAPI_l4s_support_t *OpenAPI_l4s_support_create(
    OpenAPI_l4s_notif_type_e notif_type,
    OpenAPI_list_t *flows
);
void OpenAPI_l4s_support_free(OpenAPI_l4s_support_t *l4s_support);
OpenAPI_l4s_support_t *OpenAPI_l4s_support_parseFromJSON(cJSON *l4s_supportJSON);
cJSON *OpenAPI_l4s_support_convertToJSON(OpenAPI_l4s_support_t *l4s_support);
OpenAPI_l4s_support_t *OpenAPI_l4s_support_copy(OpenAPI_l4s_support_t *dst, OpenAPI_l4s_support_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_l4s_support_H_ */

