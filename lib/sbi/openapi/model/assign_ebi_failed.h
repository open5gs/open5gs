/*
 * assign_ebi_failed.h
 *
 *
 */

#ifndef _OpenAPI_assign_ebi_failed_H_
#define _OpenAPI_assign_ebi_failed_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_assign_ebi_failed_s OpenAPI_assign_ebi_failed_t;
typedef struct OpenAPI_assign_ebi_failed_s {
    int pdu_session_id;
    OpenAPI_list_t *failed_arp_list;
} OpenAPI_assign_ebi_failed_t;

OpenAPI_assign_ebi_failed_t *OpenAPI_assign_ebi_failed_create(
    int pdu_session_id,
    OpenAPI_list_t *failed_arp_list
    );
void OpenAPI_assign_ebi_failed_free(OpenAPI_assign_ebi_failed_t *assign_ebi_failed);
OpenAPI_assign_ebi_failed_t *OpenAPI_assign_ebi_failed_parseFromJSON(cJSON *assign_ebi_failedJSON);
cJSON *OpenAPI_assign_ebi_failed_convertToJSON(OpenAPI_assign_ebi_failed_t *assign_ebi_failed);
OpenAPI_assign_ebi_failed_t *OpenAPI_assign_ebi_failed_copy(OpenAPI_assign_ebi_failed_t *dst, OpenAPI_assign_ebi_failed_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_assign_ebi_failed_H_ */

