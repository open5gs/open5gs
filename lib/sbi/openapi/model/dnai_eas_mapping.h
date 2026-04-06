/*
 * dnai_eas_mapping.h
 *
 * Contains DNAI to EAS mapping information.
 */

#ifndef _OpenAPI_dnai_eas_mapping_H_
#define _OpenAPI_dnai_eas_mapping_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dnai_eas_mapping_s OpenAPI_dnai_eas_mapping_t;
#include "dnai_eas_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dnai_eas_mapping_s {
    OpenAPI_list_t *dnai_eas_infos;
};

OpenAPI_dnai_eas_mapping_t *OpenAPI_dnai_eas_mapping_create(
    OpenAPI_list_t *dnai_eas_infos
);
void OpenAPI_dnai_eas_mapping_free(OpenAPI_dnai_eas_mapping_t *dnai_eas_mapping);
OpenAPI_dnai_eas_mapping_t *OpenAPI_dnai_eas_mapping_parseFromJSON(cJSON *dnai_eas_mappingJSON);
cJSON *OpenAPI_dnai_eas_mapping_convertToJSON(OpenAPI_dnai_eas_mapping_t *dnai_eas_mapping);
OpenAPI_dnai_eas_mapping_t *OpenAPI_dnai_eas_mapping_copy(OpenAPI_dnai_eas_mapping_t *dst, OpenAPI_dnai_eas_mapping_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnai_eas_mapping_H_ */

