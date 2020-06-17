/*
 * udsf_info.h
 *
 *
 */

#ifndef _OpenAPI_udsf_info_H_
#define _OpenAPI_udsf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_udsf_info_s OpenAPI_udsf_info_t;
typedef struct OpenAPI_udsf_info_s {
    char *group_id;
    OpenAPI_list_t *supi_ranges;
} OpenAPI_udsf_info_t;

OpenAPI_udsf_info_t *OpenAPI_udsf_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges
    );
void OpenAPI_udsf_info_free(OpenAPI_udsf_info_t *udsf_info);
OpenAPI_udsf_info_t *OpenAPI_udsf_info_parseFromJSON(cJSON *udsf_infoJSON);
cJSON *OpenAPI_udsf_info_convertToJSON(OpenAPI_udsf_info_t *udsf_info);
OpenAPI_udsf_info_t *OpenAPI_udsf_info_copy(OpenAPI_udsf_info_t *dst, OpenAPI_udsf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_udsf_info_H_ */

