/*
 * network_area_info_2.h
 *
 * Describes a network area information in which the NF service consumer requests the number of UEs.
 */

#ifndef _OpenAPI_network_area_info_2_H_
#define _OpenAPI_network_area_info_2_H_

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

typedef struct OpenAPI_network_area_info_2_s OpenAPI_network_area_info_2_t;
typedef struct OpenAPI_network_area_info_2_s {
    OpenAPI_list_t *ecgis;
    OpenAPI_list_t *ncgis;
    OpenAPI_list_t *g_ran_node_ids;
    OpenAPI_list_t *tais;
} OpenAPI_network_area_info_2_t;

OpenAPI_network_area_info_2_t *OpenAPI_network_area_info_2_create(
    OpenAPI_list_t *ecgis,
    OpenAPI_list_t *ncgis,
    OpenAPI_list_t *g_ran_node_ids,
    OpenAPI_list_t *tais
    );
void OpenAPI_network_area_info_2_free(OpenAPI_network_area_info_2_t *network_area_info_2);
OpenAPI_network_area_info_2_t *OpenAPI_network_area_info_2_parseFromJSON(cJSON *network_area_info_2JSON);
cJSON *OpenAPI_network_area_info_2_convertToJSON(OpenAPI_network_area_info_2_t *network_area_info_2);
OpenAPI_network_area_info_2_t *OpenAPI_network_area_info_2_copy(OpenAPI_network_area_info_2_t *dst, OpenAPI_network_area_info_2_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_area_info_2_H_ */

