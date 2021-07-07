/*
 * traffic_influ_data_notif.h
 *
 * 
 */

#ifndef _OpenAPI_traffic_influ_data_notif_H_
#define _OpenAPI_traffic_influ_data_notif_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "traffic_influ_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_influ_data_notif_s OpenAPI_traffic_influ_data_notif_t;
typedef struct OpenAPI_traffic_influ_data_notif_s {
    char *res_uri;
    struct OpenAPI_traffic_influ_data_s *traffic_influ_data;
} OpenAPI_traffic_influ_data_notif_t;

OpenAPI_traffic_influ_data_notif_t *OpenAPI_traffic_influ_data_notif_create(
    char *res_uri,
    OpenAPI_traffic_influ_data_t *traffic_influ_data
);
void OpenAPI_traffic_influ_data_notif_free(OpenAPI_traffic_influ_data_notif_t *traffic_influ_data_notif);
OpenAPI_traffic_influ_data_notif_t *OpenAPI_traffic_influ_data_notif_parseFromJSON(cJSON *traffic_influ_data_notifJSON);
cJSON *OpenAPI_traffic_influ_data_notif_convertToJSON(OpenAPI_traffic_influ_data_notif_t *traffic_influ_data_notif);
OpenAPI_traffic_influ_data_notif_t *OpenAPI_traffic_influ_data_notif_copy(OpenAPI_traffic_influ_data_notif_t *dst, OpenAPI_traffic_influ_data_notif_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influ_data_notif_H_ */

