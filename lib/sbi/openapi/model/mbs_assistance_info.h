/*
 * mbs_assistance_info.h
 *
 * Contains MBS Assistance Info
 */

#ifndef _OpenAPI_mbs_assistance_info_H_
#define _OpenAPI_mbs_assistance_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbs_assistance_info_s OpenAPI_mbs_assistance_info_t;
#include "mbs_session_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbs_assistance_info_s {
    struct OpenAPI_mbs_session_id_s *mbs_session_id;
    OpenAPI_list_t *assistance_info;
};

OpenAPI_mbs_assistance_info_t *OpenAPI_mbs_assistance_info_create(
    OpenAPI_mbs_session_id_t *mbs_session_id,
    OpenAPI_list_t *assistance_info
);
void OpenAPI_mbs_assistance_info_free(OpenAPI_mbs_assistance_info_t *mbs_assistance_info);
OpenAPI_mbs_assistance_info_t *OpenAPI_mbs_assistance_info_parseFromJSON(cJSON *mbs_assistance_infoJSON);
cJSON *OpenAPI_mbs_assistance_info_convertToJSON(OpenAPI_mbs_assistance_info_t *mbs_assistance_info);
OpenAPI_mbs_assistance_info_t *OpenAPI_mbs_assistance_info_copy(OpenAPI_mbs_assistance_info_t *dst, OpenAPI_mbs_assistance_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_assistance_info_H_ */

