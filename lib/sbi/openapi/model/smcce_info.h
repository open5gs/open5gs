/*
 * smcce_info.h
 *
 * Represents the Session Management congestion control experience information.
 */

#ifndef _OpenAPI_smcce_info_H_
#define _OpenAPI_smcce_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "smcce_ue_list.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smcce_info_s OpenAPI_smcce_info_t;
typedef struct OpenAPI_smcce_info_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    struct OpenAPI_smcce_ue_list_s *smcce_ue_list;
} OpenAPI_smcce_info_t;

OpenAPI_smcce_info_t *OpenAPI_smcce_info_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_smcce_ue_list_t *smcce_ue_list
);
void OpenAPI_smcce_info_free(OpenAPI_smcce_info_t *smcce_info);
OpenAPI_smcce_info_t *OpenAPI_smcce_info_parseFromJSON(cJSON *smcce_infoJSON);
cJSON *OpenAPI_smcce_info_convertToJSON(OpenAPI_smcce_info_t *smcce_info);
OpenAPI_smcce_info_t *OpenAPI_smcce_info_copy(OpenAPI_smcce_info_t *dst, OpenAPI_smcce_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smcce_info_H_ */

