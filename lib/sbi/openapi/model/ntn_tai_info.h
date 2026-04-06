/*
 * ntn_tai_info.h
 *
 * Contains NR NTN TAI Information.
 */

#ifndef _OpenAPI_ntn_tai_info_H_
#define _OpenAPI_ntn_tai_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ntn_tai_info_s OpenAPI_ntn_tai_info_t;
#include "plmn_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ntn_tai_info_s {
    struct OpenAPI_plmn_id_nid_s *plmn_id;
    OpenAPI_list_t *tac_list;
    char *derived_tac;
};

OpenAPI_ntn_tai_info_t *OpenAPI_ntn_tai_info_create(
    OpenAPI_plmn_id_nid_t *plmn_id,
    OpenAPI_list_t *tac_list,
    char *derived_tac
);
void OpenAPI_ntn_tai_info_free(OpenAPI_ntn_tai_info_t *ntn_tai_info);
OpenAPI_ntn_tai_info_t *OpenAPI_ntn_tai_info_parseFromJSON(cJSON *ntn_tai_infoJSON);
cJSON *OpenAPI_ntn_tai_info_convertToJSON(OpenAPI_ntn_tai_info_t *ntn_tai_info);
OpenAPI_ntn_tai_info_t *OpenAPI_ntn_tai_info_copy(OpenAPI_ntn_tai_info_t *dst, OpenAPI_ntn_tai_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ntn_tai_info_H_ */

