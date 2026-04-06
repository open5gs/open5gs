/*
 * signal_info.h
 *
 * Represents the signalling information.
 */

#ifndef _OpenAPI_signal_info_H_
#define _OpenAPI_signal_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_signal_info_s OpenAPI_signal_info_t;
#include "signal_analytics.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_signal_info_s {
    OpenAPI_list_t *impact_ref_points;
    OpenAPI_list_t *impact_srv_ops;
    OpenAPI_list_t *sig_analytics;
};

OpenAPI_signal_info_t *OpenAPI_signal_info_create(
    OpenAPI_list_t *impact_ref_points,
    OpenAPI_list_t *impact_srv_ops,
    OpenAPI_list_t *sig_analytics
);
void OpenAPI_signal_info_free(OpenAPI_signal_info_t *signal_info);
OpenAPI_signal_info_t *OpenAPI_signal_info_parseFromJSON(cJSON *signal_infoJSON);
cJSON *OpenAPI_signal_info_convertToJSON(OpenAPI_signal_info_t *signal_info);
OpenAPI_signal_info_t *OpenAPI_signal_info_copy(OpenAPI_signal_info_t *dst, OpenAPI_signal_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_signal_info_H_ */

