/*
 * ue_identifiers.h
 *
 * Contains lists of UE IDs
 */

#ifndef _OpenAPI_ue_identifiers_H_
#define _OpenAPI_ue_identifiers_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_identifiers_s OpenAPI_ue_identifiers_t;
#include "gpsi_information.h"
#include "supi_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_identifiers_s {
    OpenAPI_list_t* ue_id_list;
    OpenAPI_list_t* ue_id_gpsi_list;
};

OpenAPI_ue_identifiers_t *OpenAPI_ue_identifiers_create(
    OpenAPI_list_t* ue_id_list,
    OpenAPI_list_t* ue_id_gpsi_list
);
void OpenAPI_ue_identifiers_free(OpenAPI_ue_identifiers_t *ue_identifiers);
OpenAPI_ue_identifiers_t *OpenAPI_ue_identifiers_parseFromJSON(cJSON *ue_identifiersJSON);
cJSON *OpenAPI_ue_identifiers_convertToJSON(OpenAPI_ue_identifiers_t *ue_identifiers);
OpenAPI_ue_identifiers_t *OpenAPI_ue_identifiers_copy(OpenAPI_ue_identifiers_t *dst, OpenAPI_ue_identifiers_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_identifiers_H_ */

