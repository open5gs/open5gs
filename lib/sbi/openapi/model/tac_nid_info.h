/*
 * tac_nid_info.h
 *
 * contains a list of the tracking area codes in SNPN.
 */

#ifndef _OpenAPI_tac_nid_info_H_
#define _OpenAPI_tac_nid_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tac_nid_info_s OpenAPI_tac_nid_info_t;
#include "tac_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tac_nid_info_s {
    OpenAPI_list_t *tac_nid_list;
};

OpenAPI_tac_nid_info_t *OpenAPI_tac_nid_info_create(
    OpenAPI_list_t *tac_nid_list
);
void OpenAPI_tac_nid_info_free(OpenAPI_tac_nid_info_t *tac_nid_info);
OpenAPI_tac_nid_info_t *OpenAPI_tac_nid_info_parseFromJSON(cJSON *tac_nid_infoJSON);
cJSON *OpenAPI_tac_nid_info_convertToJSON(OpenAPI_tac_nid_info_t *tac_nid_info);
OpenAPI_tac_nid_info_t *OpenAPI_tac_nid_info_copy(OpenAPI_tac_nid_info_t *dst, OpenAPI_tac_nid_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tac_nid_info_H_ */

