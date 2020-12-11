/*
 * up_path_chg_event.h
 *
 *
 */

#ifndef _OpenAPI_up_path_chg_event_H_
#define _OpenAPI_up_path_chg_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnai_change_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_up_path_chg_event_s OpenAPI_up_path_chg_event_t;
typedef struct OpenAPI_up_path_chg_event_s {
    char *notification_uri;
    char *notif_corre_id;
    struct OpenAPI_dnai_change_type_s *dnai_chg_type;
    int af_ack_ind;
} OpenAPI_up_path_chg_event_t;

OpenAPI_up_path_chg_event_t *OpenAPI_up_path_chg_event_create(
    char *notification_uri,
    char *notif_corre_id,
    OpenAPI_dnai_change_type_t *dnai_chg_type,
    int af_ack_ind
    );
void OpenAPI_up_path_chg_event_free(OpenAPI_up_path_chg_event_t *up_path_chg_event);
OpenAPI_up_path_chg_event_t *OpenAPI_up_path_chg_event_parseFromJSON(cJSON *up_path_chg_eventJSON);
cJSON *OpenAPI_up_path_chg_event_convertToJSON(OpenAPI_up_path_chg_event_t *up_path_chg_event);
OpenAPI_up_path_chg_event_t *OpenAPI_up_path_chg_event_copy(OpenAPI_up_path_chg_event_t *dst, OpenAPI_up_path_chg_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_path_chg_event_H_ */

