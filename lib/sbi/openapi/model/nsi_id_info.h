/*
 * nsi_id_info.h
 *
 * Represents the S-NSSAI and the optionally associated Network Slice Instance(s).
 */

#ifndef _OpenAPI_nsi_id_info_H_
#define _OpenAPI_nsi_id_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nsi_id_info_s OpenAPI_nsi_id_info_t;
typedef struct OpenAPI_nsi_id_info_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *nsi_ids;
} OpenAPI_nsi_id_info_t;

OpenAPI_nsi_id_info_t *OpenAPI_nsi_id_info_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *nsi_ids
);
void OpenAPI_nsi_id_info_free(OpenAPI_nsi_id_info_t *nsi_id_info);
OpenAPI_nsi_id_info_t *OpenAPI_nsi_id_info_parseFromJSON(cJSON *nsi_id_infoJSON);
cJSON *OpenAPI_nsi_id_info_convertToJSON(OpenAPI_nsi_id_info_t *nsi_id_info);
OpenAPI_nsi_id_info_t *OpenAPI_nsi_id_info_copy(OpenAPI_nsi_id_info_t *dst, OpenAPI_nsi_id_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsi_id_info_H_ */

