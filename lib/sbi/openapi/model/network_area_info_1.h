/*
 * network_area_info_1.h
 *
 * Describes a network area information in which the NF service consumer requests the number of UEs.
 */

#ifndef _OpenAPI_network_area_info_1_H_
#define _OpenAPI_network_area_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecgi_1.h"
#include "global_ran_node_id_1.h"
#include "ncgi_1.h"
#include "tai_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_network_area_info_1_s OpenAPI_network_area_info_1_t;
typedef struct OpenAPI_network_area_info_1_s {
    OpenAPI_list_t *ecgis;
    OpenAPI_list_t *ncgis;
    OpenAPI_list_t *g_ran_node_ids;
    OpenAPI_list_t *tais;
} OpenAPI_network_area_info_1_t;

OpenAPI_network_area_info_1_t *OpenAPI_network_area_info_1_create(
    OpenAPI_list_t *ecgis,
    OpenAPI_list_t *ncgis,
    OpenAPI_list_t *g_ran_node_ids,
    OpenAPI_list_t *tais
);
void OpenAPI_network_area_info_1_free(OpenAPI_network_area_info_1_t *network_area_info_1);
OpenAPI_network_area_info_1_t *OpenAPI_network_area_info_1_parseFromJSON(cJSON *network_area_info_1JSON);
cJSON *OpenAPI_network_area_info_1_convertToJSON(OpenAPI_network_area_info_1_t *network_area_info_1);
OpenAPI_network_area_info_1_t *OpenAPI_network_area_info_1_copy(OpenAPI_network_area_info_1_t *dst, OpenAPI_network_area_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_area_info_1_H_ */

