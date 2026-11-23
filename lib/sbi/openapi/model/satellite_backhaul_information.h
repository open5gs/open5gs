/*
 * satellite_backhaul_information.h
 *
 * 
 */

#ifndef _OpenAPI_satellite_backhaul_information_H_
#define _OpenAPI_satellite_backhaul_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_satellite_backhaul_information_s OpenAPI_satellite_backhaul_information_t;
#include "satellite_backhaul_category.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_satellite_backhaul_information_s {
    OpenAPI_list_t *satellite_backhaul_category;
    char *g_eo_satellite_id;
};

OpenAPI_satellite_backhaul_information_t *OpenAPI_satellite_backhaul_information_create(
    OpenAPI_list_t *satellite_backhaul_category,
    char *g_eo_satellite_id
);
void OpenAPI_satellite_backhaul_information_free(OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information);
OpenAPI_satellite_backhaul_information_t *OpenAPI_satellite_backhaul_information_parseFromJSON(cJSON *satellite_backhaul_informationJSON);
cJSON *OpenAPI_satellite_backhaul_information_convertToJSON(OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information);
OpenAPI_satellite_backhaul_information_t *OpenAPI_satellite_backhaul_information_copy(OpenAPI_satellite_backhaul_information_t *dst, OpenAPI_satellite_backhaul_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_satellite_backhaul_information_H_ */

