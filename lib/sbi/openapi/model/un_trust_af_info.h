/*
 * un_trust_af_info.h
 *
 * Information of a untrusted AF Instance
 */

#ifndef _OpenAPI_un_trust_af_info_H_
#define _OpenAPI_un_trust_af_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai_info_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_un_trust_af_info_s OpenAPI_un_trust_af_info_t;
typedef struct OpenAPI_un_trust_af_info_s {
    char *af_id;
    OpenAPI_list_t *s_nssai_info_list;
    bool is_mapping_ind;
    int mapping_ind;
} OpenAPI_un_trust_af_info_t;

OpenAPI_un_trust_af_info_t *OpenAPI_un_trust_af_info_create(
    char *af_id,
    OpenAPI_list_t *s_nssai_info_list,
    bool is_mapping_ind,
    int mapping_ind
);
void OpenAPI_un_trust_af_info_free(OpenAPI_un_trust_af_info_t *un_trust_af_info);
OpenAPI_un_trust_af_info_t *OpenAPI_un_trust_af_info_parseFromJSON(cJSON *un_trust_af_infoJSON);
cJSON *OpenAPI_un_trust_af_info_convertToJSON(OpenAPI_un_trust_af_info_t *un_trust_af_info);
OpenAPI_un_trust_af_info_t *OpenAPI_un_trust_af_info_copy(OpenAPI_un_trust_af_info_t *dst, OpenAPI_un_trust_af_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_un_trust_af_info_H_ */

