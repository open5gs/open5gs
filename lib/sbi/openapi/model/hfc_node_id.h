/*
 * hfc_node_id.h
 *
 * REpresents the HFC Node Identifer received over NGAP.
 */

#ifndef _OpenAPI_hfc_node_id_H_
#define _OpenAPI_hfc_node_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hfc_node_id_s OpenAPI_hfc_node_id_t;
typedef struct OpenAPI_hfc_node_id_s {
    char *hfc_nid;
} OpenAPI_hfc_node_id_t;

OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_create(
    char *hfc_nid
);
void OpenAPI_hfc_node_id_free(OpenAPI_hfc_node_id_t *hfc_node_id);
OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_parseFromJSON(cJSON *hfc_node_idJSON);
cJSON *OpenAPI_hfc_node_id_convertToJSON(OpenAPI_hfc_node_id_t *hfc_node_id);
OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_copy(OpenAPI_hfc_node_id_t *dst, OpenAPI_hfc_node_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hfc_node_id_H_ */

