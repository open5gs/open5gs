/*
 * vplmn_dl_ambr.h
 *
 * an Authorized DL Session AMBR for Offloading for the VPLMN
 */

#ifndef _OpenAPI_vplmn_dl_ambr_H_
#define _OpenAPI_vplmn_dl_ambr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vplmn_dl_ambr_s OpenAPI_vplmn_dl_ambr_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vplmn_dl_ambr_s {
    struct OpenAPI_plmn_id_s *vplmn_id;
    char *session_dl_ambr;
};

OpenAPI_vplmn_dl_ambr_t *OpenAPI_vplmn_dl_ambr_create(
    OpenAPI_plmn_id_t *vplmn_id,
    char *session_dl_ambr
);
void OpenAPI_vplmn_dl_ambr_free(OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr);
OpenAPI_vplmn_dl_ambr_t *OpenAPI_vplmn_dl_ambr_parseFromJSON(cJSON *vplmn_dl_ambrJSON);
cJSON *OpenAPI_vplmn_dl_ambr_convertToJSON(OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr);
OpenAPI_vplmn_dl_ambr_t *OpenAPI_vplmn_dl_ambr_copy(OpenAPI_vplmn_dl_ambr_t *dst, OpenAPI_vplmn_dl_ambr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vplmn_dl_ambr_H_ */

