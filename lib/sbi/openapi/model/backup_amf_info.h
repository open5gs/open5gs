/*
 * backup_amf_info.h
 *
 * Provides details of the Backup AMF.
 */

#ifndef _OpenAPI_backup_amf_info_H_
#define _OpenAPI_backup_amf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_backup_amf_info_s OpenAPI_backup_amf_info_t;
typedef struct OpenAPI_backup_amf_info_s {
    char *backup_amf;
    OpenAPI_list_t *guami_list;
} OpenAPI_backup_amf_info_t;

OpenAPI_backup_amf_info_t *OpenAPI_backup_amf_info_create(
    char *backup_amf,
    OpenAPI_list_t *guami_list
);
void OpenAPI_backup_amf_info_free(OpenAPI_backup_amf_info_t *backup_amf_info);
OpenAPI_backup_amf_info_t *OpenAPI_backup_amf_info_parseFromJSON(cJSON *backup_amf_infoJSON);
cJSON *OpenAPI_backup_amf_info_convertToJSON(OpenAPI_backup_amf_info_t *backup_amf_info);
OpenAPI_backup_amf_info_t *OpenAPI_backup_amf_info_copy(OpenAPI_backup_amf_info_t *dst, OpenAPI_backup_amf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_backup_amf_info_H_ */

