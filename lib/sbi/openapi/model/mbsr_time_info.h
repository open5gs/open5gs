/*
 * mbsr_time_info.h
 *
 * MBSR Time Information
 */

#ifndef _OpenAPI_mbsr_time_info_H_
#define _OpenAPI_mbsr_time_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbsr_time_info_s OpenAPI_mbsr_time_info_t;
#include "recur_time.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbsr_time_info_s {
    OpenAPI_list_t *mbsr_time_window;
    OpenAPI_list_t *mbsr_recur_time;
};

OpenAPI_mbsr_time_info_t *OpenAPI_mbsr_time_info_create(
    OpenAPI_list_t *mbsr_time_window,
    OpenAPI_list_t *mbsr_recur_time
);
void OpenAPI_mbsr_time_info_free(OpenAPI_mbsr_time_info_t *mbsr_time_info);
OpenAPI_mbsr_time_info_t *OpenAPI_mbsr_time_info_parseFromJSON(cJSON *mbsr_time_infoJSON);
cJSON *OpenAPI_mbsr_time_info_convertToJSON(OpenAPI_mbsr_time_info_t *mbsr_time_info);
OpenAPI_mbsr_time_info_t *OpenAPI_mbsr_time_info_copy(OpenAPI_mbsr_time_info_t *dst, OpenAPI_mbsr_time_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbsr_time_info_H_ */

