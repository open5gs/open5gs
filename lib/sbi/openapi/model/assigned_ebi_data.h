/*
 * assigned_ebi_data.h
 *
 *
 */

#ifndef _OpenAPI_assigned_ebi_data_H_
#define _OpenAPI_assigned_ebi_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"
#include "ebi_arp_mapping.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_assigned_ebi_data_s OpenAPI_assigned_ebi_data_t;
typedef struct OpenAPI_assigned_ebi_data_s {
    int pdu_session_id;
    OpenAPI_list_t *assigned_ebi_list;
    OpenAPI_list_t *failed_arp_list;
    OpenAPI_list_t *released_ebi_list;
} OpenAPI_assigned_ebi_data_t;

OpenAPI_assigned_ebi_data_t *OpenAPI_assigned_ebi_data_create(
    int pdu_session_id,
    OpenAPI_list_t *assigned_ebi_list,
    OpenAPI_list_t *failed_arp_list,
    OpenAPI_list_t *released_ebi_list
    );
void OpenAPI_assigned_ebi_data_free(OpenAPI_assigned_ebi_data_t *assigned_ebi_data);
OpenAPI_assigned_ebi_data_t *OpenAPI_assigned_ebi_data_parseFromJSON(cJSON *assigned_ebi_dataJSON);
cJSON *OpenAPI_assigned_ebi_data_convertToJSON(OpenAPI_assigned_ebi_data_t *assigned_ebi_data);
OpenAPI_assigned_ebi_data_t *OpenAPI_assigned_ebi_data_copy(OpenAPI_assigned_ebi_data_t *dst, OpenAPI_assigned_ebi_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_assigned_ebi_data_H_ */

