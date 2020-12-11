/*
 * presence_info.h
 *
 *
 */

#ifndef _OpenAPI_presence_info_H_
#define _OpenAPI_presence_info_H_

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

typedef struct OpenAPI_presence_info_s OpenAPI_presence_info_t;
typedef struct OpenAPI_presence_info_s {
    char *pra_id;
    OpenAPI_presence_state_e presence_state;
    OpenAPI_list_t *tracking_area_list;
    OpenAPI_list_t *ecgi_list;
    OpenAPI_list_t *ncgi_list;
    OpenAPI_list_t *global_ran_node_id_list;
    OpenAPI_list_t *globale_nb_id_list;
} OpenAPI_presence_info_t;

OpenAPI_presence_info_t *OpenAPI_presence_info_create(
    char *pra_id,
    OpenAPI_presence_state_e presence_state,
    OpenAPI_list_t *tracking_area_list,
    OpenAPI_list_t *ecgi_list,
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *global_ran_node_id_list,
    OpenAPI_list_t *globale_nb_id_list
    );
void OpenAPI_presence_info_free(OpenAPI_presence_info_t *presence_info);
OpenAPI_presence_info_t *OpenAPI_presence_info_parseFromJSON(cJSON *presence_infoJSON);
cJSON *OpenAPI_presence_info_convertToJSON(OpenAPI_presence_info_t *presence_info);
OpenAPI_presence_info_t *OpenAPI_presence_info_copy(OpenAPI_presence_info_t *dst, OpenAPI_presence_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_presence_info_H_ */

