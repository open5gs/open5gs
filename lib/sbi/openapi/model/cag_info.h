/*
 * cag_info.h
 *
 * Contains Closed Access Group Information
 */

#ifndef _OpenAPI_cag_info_H_
#define _OpenAPI_cag_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_cag_info_s OpenAPI_cag_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_cag_info_s {
    OpenAPI_list_t *allowed_cag_list;
    bool is_cag_only_indicator;
    int cag_only_indicator;
};

OpenAPI_cag_info_t *OpenAPI_cag_info_create(
    OpenAPI_list_t *allowed_cag_list,
    bool is_cag_only_indicator,
    int cag_only_indicator
);
void OpenAPI_cag_info_free(OpenAPI_cag_info_t *cag_info);
OpenAPI_cag_info_t *OpenAPI_cag_info_parseFromJSON(cJSON *cag_infoJSON);
cJSON *OpenAPI_cag_info_convertToJSON(OpenAPI_cag_info_t *cag_info);
OpenAPI_cag_info_t *OpenAPI_cag_info_copy(OpenAPI_cag_info_t *dst, OpenAPI_cag_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_info_H_ */

