/*
 * pcscf_restoration_notification.h
 *
 * 
 */

#ifndef _OpenAPI_pcscf_restoration_notification_H_
#define _OpenAPI_pcscf_restoration_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcscf_restoration_notification_s OpenAPI_pcscf_restoration_notification_t;
typedef struct OpenAPI_pcscf_restoration_notification_s {
    char *supi;
} OpenAPI_pcscf_restoration_notification_t;

OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_create(
    char *supi
);
void OpenAPI_pcscf_restoration_notification_free(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification);
OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_parseFromJSON(cJSON *pcscf_restoration_notificationJSON);
cJSON *OpenAPI_pcscf_restoration_notification_convertToJSON(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification);
OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_copy(OpenAPI_pcscf_restoration_notification_t *dst, OpenAPI_pcscf_restoration_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcscf_restoration_notification_H_ */

