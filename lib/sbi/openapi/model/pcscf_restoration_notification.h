/*
 * pcscf_restoration_notification.h
 *
 * This datatype contains the SUPI optionally addresses the failed P-CSCF. It is supported by the POST HTTP method. 
 */

#ifndef _OpenAPI_pcscf_restoration_notification_H_
#define _OpenAPI_pcscf_restoration_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pcscf_restoration_notification_s OpenAPI_pcscf_restoration_notification_t;
#include "guami.h"
#include "pcscf_address.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pcscf_restoration_notification_s {
    char *supi;
    bool is_failed_pcscf_null;
    struct OpenAPI_pcscf_address_s *failed_pcscf;
    struct OpenAPI_guami_s *old_guami;
};

OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_create(
    char *supi,
    bool is_failed_pcscf_null,
    OpenAPI_pcscf_address_t *failed_pcscf,
    OpenAPI_guami_t *old_guami
);
void OpenAPI_pcscf_restoration_notification_free(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification);
OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_parseFromJSON(cJSON *pcscf_restoration_notificationJSON);
cJSON *OpenAPI_pcscf_restoration_notification_convertToJSON(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification);
OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_copy(OpenAPI_pcscf_restoration_notification_t *dst, OpenAPI_pcscf_restoration_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcscf_restoration_notification_H_ */

