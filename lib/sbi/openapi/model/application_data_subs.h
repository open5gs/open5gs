/*
 * application_data_subs.h
 *
 * Identifies a subscription to application data change notification.
 */

#ifndef _OpenAPI_application_data_subs_H_
#define _OpenAPI_application_data_subs_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "data_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_application_data_subs_s OpenAPI_application_data_subs_t;
typedef struct OpenAPI_application_data_subs_s {
    char *notification_uri;
    OpenAPI_list_t *data_filters;
    char *expiry;
    char *supported_features;
} OpenAPI_application_data_subs_t;

OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_create(
    char *notification_uri,
    OpenAPI_list_t *data_filters,
    char *expiry,
    char *supported_features
);
void OpenAPI_application_data_subs_free(OpenAPI_application_data_subs_t *application_data_subs);
OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_parseFromJSON(cJSON *application_data_subsJSON);
cJSON *OpenAPI_application_data_subs_convertToJSON(OpenAPI_application_data_subs_t *application_data_subs);
OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_copy(OpenAPI_application_data_subs_t *dst, OpenAPI_application_data_subs_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_application_data_subs_H_ */

