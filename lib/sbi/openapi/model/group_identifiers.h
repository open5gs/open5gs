/*
 * group_identifiers.h
 *
 * Contains Information to identify a group of Ues
 */

#ifndef _OpenAPI_group_identifiers_H_
#define _OpenAPI_group_identifiers_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_group_identifiers_s OpenAPI_group_identifiers_t;
#include "ue_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_group_identifiers_s {
    char *ext_group_id;
    char *int_group_id;
    OpenAPI_list_t *ue_id_list;
};

OpenAPI_group_identifiers_t *OpenAPI_group_identifiers_create(
    char *ext_group_id,
    char *int_group_id,
    OpenAPI_list_t *ue_id_list
);
void OpenAPI_group_identifiers_free(OpenAPI_group_identifiers_t *group_identifiers);
OpenAPI_group_identifiers_t *OpenAPI_group_identifiers_parseFromJSON(cJSON *group_identifiersJSON);
cJSON *OpenAPI_group_identifiers_convertToJSON(OpenAPI_group_identifiers_t *group_identifiers);
OpenAPI_group_identifiers_t *OpenAPI_group_identifiers_copy(OpenAPI_group_identifiers_t *dst, OpenAPI_group_identifiers_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_group_identifiers_H_ */

