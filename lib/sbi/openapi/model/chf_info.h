/*
 * chf_info.h
 *
 * Information of a CHF NF Instance
 */

#ifndef _OpenAPI_chf_info_H_
#define _OpenAPI_chf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "plmn_range.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_chf_info_s OpenAPI_chf_info_t;
typedef struct OpenAPI_chf_info_s {
    OpenAPI_list_t *supi_range_list;
    OpenAPI_list_t *gpsi_range_list;
    OpenAPI_list_t *plmn_range_list;
    char *group_id;
} OpenAPI_chf_info_t;

OpenAPI_chf_info_t *OpenAPI_chf_info_create(
    OpenAPI_list_t *supi_range_list,
    OpenAPI_list_t *gpsi_range_list,
    OpenAPI_list_t *plmn_range_list,
    char *group_id
    );
void OpenAPI_chf_info_free(OpenAPI_chf_info_t *chf_info);
OpenAPI_chf_info_t *OpenAPI_chf_info_parseFromJSON(cJSON *chf_infoJSON);
cJSON *OpenAPI_chf_info_convertToJSON(OpenAPI_chf_info_t *chf_info);
OpenAPI_chf_info_t *OpenAPI_chf_info_copy(OpenAPI_chf_info_t *dst, OpenAPI_chf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_chf_info_H_ */

