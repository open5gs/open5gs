/*
 * available_bitrate_monitoring_request.h
 *
 * Available Bitrate Monitoring Request
 */

#ifndef _OpenAPI_available_bitrate_monitoring_request_H_
#define _OpenAPI_available_bitrate_monitoring_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_available_bitrate_monitoring_request_s OpenAPI_available_bitrate_monitoring_request_t;
#include "available_bitrate_request.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_available_bitrate_monitoring_request_s {
    OpenAPI_available_bitrate_request_e avail_bitrate_req;
    OpenAPI_list_t *avail_bitrate_ul_thrs;
    OpenAPI_list_t *avail_bitrate_dl_thrs;
};

OpenAPI_available_bitrate_monitoring_request_t *OpenAPI_available_bitrate_monitoring_request_create(
    OpenAPI_available_bitrate_request_e avail_bitrate_req,
    OpenAPI_list_t *avail_bitrate_ul_thrs,
    OpenAPI_list_t *avail_bitrate_dl_thrs
);
void OpenAPI_available_bitrate_monitoring_request_free(OpenAPI_available_bitrate_monitoring_request_t *available_bitrate_monitoring_request);
OpenAPI_available_bitrate_monitoring_request_t *OpenAPI_available_bitrate_monitoring_request_parseFromJSON(cJSON *available_bitrate_monitoring_requestJSON);
cJSON *OpenAPI_available_bitrate_monitoring_request_convertToJSON(OpenAPI_available_bitrate_monitoring_request_t *available_bitrate_monitoring_request);
OpenAPI_available_bitrate_monitoring_request_t *OpenAPI_available_bitrate_monitoring_request_copy(OpenAPI_available_bitrate_monitoring_request_t *dst, OpenAPI_available_bitrate_monitoring_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_available_bitrate_monitoring_request_H_ */

