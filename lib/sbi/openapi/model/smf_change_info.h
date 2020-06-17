/*
 * smf_change_info.h
 *
 *
 */

#ifndef _OpenAPI_smf_change_info_H_
#define _OpenAPI_smf_change_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "smf_change_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smf_change_info_s OpenAPI_smf_change_info_t;
typedef struct OpenAPI_smf_change_info_s {
    OpenAPI_list_t *pdu_session_id_list;
    OpenAPI_smf_change_indication_e smf_change_ind;
} OpenAPI_smf_change_info_t;

OpenAPI_smf_change_info_t *OpenAPI_smf_change_info_create(
    OpenAPI_list_t *pdu_session_id_list,
    OpenAPI_smf_change_indication_e smf_change_ind
    );
void OpenAPI_smf_change_info_free(OpenAPI_smf_change_info_t *smf_change_info);
OpenAPI_smf_change_info_t *OpenAPI_smf_change_info_parseFromJSON(cJSON *smf_change_infoJSON);
cJSON *OpenAPI_smf_change_info_convertToJSON(OpenAPI_smf_change_info_t *smf_change_info);
OpenAPI_smf_change_info_t *OpenAPI_smf_change_info_copy(OpenAPI_smf_change_info_t *dst, OpenAPI_smf_change_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_change_info_H_ */

