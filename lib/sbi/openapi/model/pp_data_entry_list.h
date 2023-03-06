/*
 * pp_data_entry_list.h
 *
 * Contains a list of the Provisioned Parameters entries
 */

#ifndef _OpenAPI_pp_data_entry_list_H_
#define _OpenAPI_pp_data_entry_list_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pp_data_entry.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_data_entry_list_s OpenAPI_pp_data_entry_list_t;
typedef struct OpenAPI_pp_data_entry_list_s {
    OpenAPI_list_t *pp_data_entry_list;
    char *supported_features;
} OpenAPI_pp_data_entry_list_t;

OpenAPI_pp_data_entry_list_t *OpenAPI_pp_data_entry_list_create(
    OpenAPI_list_t *pp_data_entry_list,
    char *supported_features
);
void OpenAPI_pp_data_entry_list_free(OpenAPI_pp_data_entry_list_t *pp_data_entry_list);
OpenAPI_pp_data_entry_list_t *OpenAPI_pp_data_entry_list_parseFromJSON(cJSON *pp_data_entry_listJSON);
cJSON *OpenAPI_pp_data_entry_list_convertToJSON(OpenAPI_pp_data_entry_list_t *pp_data_entry_list);
OpenAPI_pp_data_entry_list_t *OpenAPI_pp_data_entry_list_copy(OpenAPI_pp_data_entry_list_t *dst, OpenAPI_pp_data_entry_list_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_data_entry_list_H_ */

