/*
 * user_data_congestion_info.h
 *
 * Represents the user data congestion information.
 */

#ifndef _OpenAPI_user_data_congestion_info_H_
#define _OpenAPI_user_data_congestion_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "congestion_info.h"
#include "network_area_info.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_user_data_congestion_info_s OpenAPI_user_data_congestion_info_t;
typedef struct OpenAPI_user_data_congestion_info_s {
    struct OpenAPI_network_area_info_s *network_area;
    struct OpenAPI_congestion_info_s *congestion_info;
    struct OpenAPI_snssai_s *snssai;
} OpenAPI_user_data_congestion_info_t;

OpenAPI_user_data_congestion_info_t *OpenAPI_user_data_congestion_info_create(
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_congestion_info_t *congestion_info,
    OpenAPI_snssai_t *snssai
);
void OpenAPI_user_data_congestion_info_free(OpenAPI_user_data_congestion_info_t *user_data_congestion_info);
OpenAPI_user_data_congestion_info_t *OpenAPI_user_data_congestion_info_parseFromJSON(cJSON *user_data_congestion_infoJSON);
cJSON *OpenAPI_user_data_congestion_info_convertToJSON(OpenAPI_user_data_congestion_info_t *user_data_congestion_info);
OpenAPI_user_data_congestion_info_t *OpenAPI_user_data_congestion_info_copy(OpenAPI_user_data_congestion_info_t *dst, OpenAPI_user_data_congestion_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_user_data_congestion_info_H_ */

