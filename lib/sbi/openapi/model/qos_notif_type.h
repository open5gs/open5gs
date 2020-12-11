/*
 * qos_notif_type.h
 *
 *
 */

#ifndef _OpenAPI_qos_notif_type_H_
#define _OpenAPI_qos_notif_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_notif_type_s OpenAPI_qos_notif_type_t;
typedef struct OpenAPI_qos_notif_type_s {
} OpenAPI_qos_notif_type_t;

OpenAPI_qos_notif_type_t *OpenAPI_qos_notif_type_create(
    );
void OpenAPI_qos_notif_type_free(OpenAPI_qos_notif_type_t *qos_notif_type);
OpenAPI_qos_notif_type_t *OpenAPI_qos_notif_type_parseFromJSON(cJSON *qos_notif_typeJSON);
cJSON *OpenAPI_qos_notif_type_convertToJSON(OpenAPI_qos_notif_type_t *qos_notif_type);
OpenAPI_qos_notif_type_t *OpenAPI_qos_notif_type_copy(OpenAPI_qos_notif_type_t *dst, OpenAPI_qos_notif_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_notif_type_H_ */

