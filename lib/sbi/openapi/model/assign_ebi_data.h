/*
 * assign_ebi_data.h
 *
 *
 */

#ifndef _OpenAPI_assign_ebi_data_H_
#define _OpenAPI_assign_ebi_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_assign_ebi_data_s OpenAPI_assign_ebi_data_t;
typedef struct OpenAPI_assign_ebi_data_s {
    int pdu_session_id;
    OpenAPI_list_t *arp_list;
    OpenAPI_list_t *released_ebi_list;
    struct OpenAPI_guami_s *old_guami;
} OpenAPI_assign_ebi_data_t;

OpenAPI_assign_ebi_data_t *OpenAPI_assign_ebi_data_create(
    int pdu_session_id,
    OpenAPI_list_t *arp_list,
    OpenAPI_list_t *released_ebi_list,
    OpenAPI_guami_t *old_guami
    );
void OpenAPI_assign_ebi_data_free(OpenAPI_assign_ebi_data_t *assign_ebi_data);
OpenAPI_assign_ebi_data_t *OpenAPI_assign_ebi_data_parseFromJSON(cJSON *assign_ebi_dataJSON);
cJSON *OpenAPI_assign_ebi_data_convertToJSON(OpenAPI_assign_ebi_data_t *assign_ebi_data);
OpenAPI_assign_ebi_data_t *OpenAPI_assign_ebi_data_copy(OpenAPI_assign_ebi_data_t *dst, OpenAPI_assign_ebi_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_assign_ebi_data_H_ */

