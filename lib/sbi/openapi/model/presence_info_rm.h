/*
 * presence_info_rm.h
 *
 * This data type is defined in the same way as the &#39;PresenceInfo&#39; data type, but with the OpenAPI &#39;nullable: true&#39; property.  If the additionalPraId IE is present, this IE shall state the presence information of the UE for the individual PRA identified by the additionalPraId IE;  If the additionalPraId IE is not present, this IE shall state the presence information of the UE for the PRA identified by the praId IE.  
 */

#ifndef _OpenAPI_presence_info_rm_H_
#define _OpenAPI_presence_info_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecgi.h"
#include "global_ran_node_id.h"
#include "ncgi.h"
#include "presence_state.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_presence_info_rm_s OpenAPI_presence_info_rm_t;
typedef struct OpenAPI_presence_info_rm_s {
    char *pra_id;
    char *additional_pra_id;
    OpenAPI_presence_state_e presence_state;
    OpenAPI_list_t *tracking_area_list;
    OpenAPI_list_t *ecgi_list;
    OpenAPI_list_t *ncgi_list;
    OpenAPI_list_t *global_ran_node_id_list;
    OpenAPI_list_t *globale_nb_id_list;
} OpenAPI_presence_info_rm_t;

OpenAPI_presence_info_rm_t *OpenAPI_presence_info_rm_create(
    char *pra_id,
    char *additional_pra_id,
    OpenAPI_presence_state_e presence_state,
    OpenAPI_list_t *tracking_area_list,
    OpenAPI_list_t *ecgi_list,
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *global_ran_node_id_list,
    OpenAPI_list_t *globale_nb_id_list
);
void OpenAPI_presence_info_rm_free(OpenAPI_presence_info_rm_t *presence_info_rm);
OpenAPI_presence_info_rm_t *OpenAPI_presence_info_rm_parseFromJSON(cJSON *presence_info_rmJSON);
cJSON *OpenAPI_presence_info_rm_convertToJSON(OpenAPI_presence_info_rm_t *presence_info_rm);
OpenAPI_presence_info_rm_t *OpenAPI_presence_info_rm_copy(OpenAPI_presence_info_rm_t *dst, OpenAPI_presence_info_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_presence_info_rm_H_ */

