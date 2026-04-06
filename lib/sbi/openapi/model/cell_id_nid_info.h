/*
 * cell_id_nid_info.h
 *
 * contains a list of the NR Cell Identities in SNPN.
 */

#ifndef _OpenAPI_cell_id_nid_info_H_
#define _OpenAPI_cell_id_nid_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_cell_id_nid_info_s OpenAPI_cell_id_nid_info_t;
#include "cell_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_cell_id_nid_info_s {
    OpenAPI_list_t *cell_id_nid_list;
};

OpenAPI_cell_id_nid_info_t *OpenAPI_cell_id_nid_info_create(
    OpenAPI_list_t *cell_id_nid_list
);
void OpenAPI_cell_id_nid_info_free(OpenAPI_cell_id_nid_info_t *cell_id_nid_info);
OpenAPI_cell_id_nid_info_t *OpenAPI_cell_id_nid_info_parseFromJSON(cJSON *cell_id_nid_infoJSON);
cJSON *OpenAPI_cell_id_nid_info_convertToJSON(OpenAPI_cell_id_nid_info_t *cell_id_nid_info);
OpenAPI_cell_id_nid_info_t *OpenAPI_cell_id_nid_info_copy(OpenAPI_cell_id_nid_info_t *dst, OpenAPI_cell_id_nid_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cell_id_nid_info_H_ */

