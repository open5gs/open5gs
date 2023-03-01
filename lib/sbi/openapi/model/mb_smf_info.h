/*
 * mb_smf_info.h
 *
 * Information of an MB-SMF NF Instance
 */

#ifndef _OpenAPI_mb_smf_info_H_
#define _OpenAPI_mb_smf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mbs_session.h"
#include "snssai_mb_smf_info_item.h"
#include "tai.h"
#include "tai_range.h"
#include "tmgi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mb_smf_info_s OpenAPI_mb_smf_info_t;
typedef struct OpenAPI_mb_smf_info_s {
    OpenAPI_list_t* s_nssai_info_list;
    OpenAPI_list_t* tmgi_range_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t* mbs_session_list;
} OpenAPI_mb_smf_info_t;

OpenAPI_mb_smf_info_t *OpenAPI_mb_smf_info_create(
    OpenAPI_list_t* s_nssai_info_list,
    OpenAPI_list_t* tmgi_range_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t* mbs_session_list
);
void OpenAPI_mb_smf_info_free(OpenAPI_mb_smf_info_t *mb_smf_info);
OpenAPI_mb_smf_info_t *OpenAPI_mb_smf_info_parseFromJSON(cJSON *mb_smf_infoJSON);
cJSON *OpenAPI_mb_smf_info_convertToJSON(OpenAPI_mb_smf_info_t *mb_smf_info);
OpenAPI_mb_smf_info_t *OpenAPI_mb_smf_info_copy(OpenAPI_mb_smf_info_t *dst, OpenAPI_mb_smf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mb_smf_info_H_ */

