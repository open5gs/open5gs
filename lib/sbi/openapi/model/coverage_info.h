/*
 * coverage_info.h
 *
 * 
 */

#ifndef _OpenAPI_coverage_info_H_
#define _OpenAPI_coverage_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_coverage_info_s OpenAPI_coverage_info_t;
#include "charging_user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_coverage_info_s {
    bool is_coverage_status;
    int coverage_status;
    char *change_time;
    OpenAPI_list_t *location_info;
};

OpenAPI_coverage_info_t *OpenAPI_coverage_info_create(
    bool is_coverage_status,
    int coverage_status,
    char *change_time,
    OpenAPI_list_t *location_info
);
void OpenAPI_coverage_info_free(OpenAPI_coverage_info_t *coverage_info);
OpenAPI_coverage_info_t *OpenAPI_coverage_info_parseFromJSON(cJSON *coverage_infoJSON);
cJSON *OpenAPI_coverage_info_convertToJSON(OpenAPI_coverage_info_t *coverage_info);
OpenAPI_coverage_info_t *OpenAPI_coverage_info_copy(OpenAPI_coverage_info_t *dst, OpenAPI_coverage_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_coverage_info_H_ */

