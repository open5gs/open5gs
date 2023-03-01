/*
 * congestion_info.h
 *
 * Represents the congestion information.
 */

#ifndef _OpenAPI_congestion_info_H_
#define _OpenAPI_congestion_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "congestion_type.h"
#include "threshold_level.h"
#include "time_window.h"
#include "top_application.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_congestion_info_s OpenAPI_congestion_info_t;
typedef struct OpenAPI_congestion_info_s {
    struct OpenAPI_congestion_type_s *cong_type;
    struct OpenAPI_time_window_s *time_intev;
    struct OpenAPI_threshold_level_s *nsi;
    bool is_confidence;
    int confidence;
    OpenAPI_list_t *top_app_list_ul;
    OpenAPI_list_t *top_app_list_dl;
} OpenAPI_congestion_info_t;

OpenAPI_congestion_info_t *OpenAPI_congestion_info_create(
    OpenAPI_congestion_type_t *cong_type,
    OpenAPI_time_window_t *time_intev,
    OpenAPI_threshold_level_t *nsi,
    bool is_confidence,
    int confidence,
    OpenAPI_list_t *top_app_list_ul,
    OpenAPI_list_t *top_app_list_dl
);
void OpenAPI_congestion_info_free(OpenAPI_congestion_info_t *congestion_info);
OpenAPI_congestion_info_t *OpenAPI_congestion_info_parseFromJSON(cJSON *congestion_infoJSON);
cJSON *OpenAPI_congestion_info_convertToJSON(OpenAPI_congestion_info_t *congestion_info);
OpenAPI_congestion_info_t *OpenAPI_congestion_info_copy(OpenAPI_congestion_info_t *dst, OpenAPI_congestion_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_congestion_info_H_ */

