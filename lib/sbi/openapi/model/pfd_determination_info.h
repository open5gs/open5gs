/*
 * pfd_determination_info.h
 *
 * Represents the PFD Determination information for a known application identifier.
 */

#ifndef _OpenAPI_pfd_determination_info_H_
#define _OpenAPI_pfd_determination_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pfd_determination_info_s OpenAPI_pfd_determination_info_t;
#include "suggested_pfd_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pfd_determination_info_s {
    char *app_id;
    OpenAPI_list_t *sugg_pfd_info_list;
};

OpenAPI_pfd_determination_info_t *OpenAPI_pfd_determination_info_create(
    char *app_id,
    OpenAPI_list_t *sugg_pfd_info_list
);
void OpenAPI_pfd_determination_info_free(OpenAPI_pfd_determination_info_t *pfd_determination_info);
OpenAPI_pfd_determination_info_t *OpenAPI_pfd_determination_info_parseFromJSON(cJSON *pfd_determination_infoJSON);
cJSON *OpenAPI_pfd_determination_info_convertToJSON(OpenAPI_pfd_determination_info_t *pfd_determination_info);
OpenAPI_pfd_determination_info_t *OpenAPI_pfd_determination_info_copy(OpenAPI_pfd_determination_info_t *dst, OpenAPI_pfd_determination_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfd_determination_info_H_ */

