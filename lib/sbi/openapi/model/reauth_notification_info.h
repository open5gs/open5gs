/*
 * reauth_notification_info.h
 *
 * Contains the SUPI to identify the UE that is subject to reauthentication
 */

#ifndef _OpenAPI_reauth_notification_info_H_
#define _OpenAPI_reauth_notification_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_reauth_notification_info_s OpenAPI_reauth_notification_info_t;
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_reauth_notification_info_s {
    char *supi;
    struct OpenAPI_guami_s *old_guami;
    bool is_ausf_rediscovery_ind;
    int ausf_rediscovery_ind;
};

OpenAPI_reauth_notification_info_t *OpenAPI_reauth_notification_info_create(
    char *supi,
    OpenAPI_guami_t *old_guami,
    bool is_ausf_rediscovery_ind,
    int ausf_rediscovery_ind
);
void OpenAPI_reauth_notification_info_free(OpenAPI_reauth_notification_info_t *reauth_notification_info);
OpenAPI_reauth_notification_info_t *OpenAPI_reauth_notification_info_parseFromJSON(cJSON *reauth_notification_infoJSON);
cJSON *OpenAPI_reauth_notification_info_convertToJSON(OpenAPI_reauth_notification_info_t *reauth_notification_info);
OpenAPI_reauth_notification_info_t *OpenAPI_reauth_notification_info_copy(OpenAPI_reauth_notification_info_t *dst, OpenAPI_reauth_notification_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reauth_notification_info_H_ */

