/*
 * network_area_info.h
 *
 * Describes a network area information in which the NF service consumer requests the number of UEs.
 */

#ifndef _OpenAPI_network_area_info_H_
#define _OpenAPI_network_area_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecgi.h"
#include "global_ran_node_id.h"
#include "ncgi.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_network_area_info_s OpenAPI_network_area_info_t;
typedef struct OpenAPI_network_area_info_s {
    OpenAPI_list_t *ecgis;
    OpenAPI_list_t *ncgis;
    OpenAPI_list_t *g_ran_node_ids;
    OpenAPI_list_t *tais;
} OpenAPI_network_area_info_t;

OpenAPI_network_area_info_t *OpenAPI_network_area_info_create(
    OpenAPI_list_t *ecgis,
    OpenAPI_list_t *ncgis,
    OpenAPI_list_t *g_ran_node_ids,
    OpenAPI_list_t *tais
    );
void OpenAPI_network_area_info_free(OpenAPI_network_area_info_t *network_area_info);
OpenAPI_network_area_info_t *OpenAPI_network_area_info_parseFromJSON(cJSON *network_area_infoJSON);
cJSON *OpenAPI_network_area_info_convertToJSON(OpenAPI_network_area_info_t *network_area_info);
OpenAPI_network_area_info_t *OpenAPI_network_area_info_copy(OpenAPI_network_area_info_t *dst, OpenAPI_network_area_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_area_info_H_ */

