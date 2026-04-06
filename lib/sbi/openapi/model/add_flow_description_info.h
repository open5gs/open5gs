/*
 * add_flow_description_info.h
 *
 * Contains additional flow description information.
 */

#ifndef _OpenAPI_add_flow_description_info_H_
#define _OpenAPI_add_flow_description_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_add_flow_description_info_s OpenAPI_add_flow_description_info_t;
#include "flow_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_add_flow_description_info_s {
    char *spi;
    char *flow_label;
    OpenAPI_flow_direction_e flow_dir;
};

OpenAPI_add_flow_description_info_t *OpenAPI_add_flow_description_info_create(
    char *spi,
    char *flow_label,
    OpenAPI_flow_direction_e flow_dir
);
void OpenAPI_add_flow_description_info_free(OpenAPI_add_flow_description_info_t *add_flow_description_info);
OpenAPI_add_flow_description_info_t *OpenAPI_add_flow_description_info_parseFromJSON(cJSON *add_flow_description_infoJSON);
cJSON *OpenAPI_add_flow_description_info_convertToJSON(OpenAPI_add_flow_description_info_t *add_flow_description_info);
OpenAPI_add_flow_description_info_t *OpenAPI_add_flow_description_info_copy(OpenAPI_add_flow_description_info_t *dst, OpenAPI_add_flow_description_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_add_flow_description_info_H_ */

