/*
 * global_ran_node_id.h
 *
 * 
 */

#ifndef _OpenAPI_global_ran_node_id_H_
#define _OpenAPI_global_ran_node_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gnb_id.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_global_ran_node_id_s OpenAPI_global_ran_node_id_t;
typedef struct OpenAPI_global_ran_node_id_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *n3_iwf_id;
    struct OpenAPI_gnb_id_s *g_nb_id;
    char *nge_nb_id;
    char *wagf_id;
    char *tngf_id;
    char *nid;
    char *e_nb_id;
} OpenAPI_global_ran_node_id_t;

OpenAPI_global_ran_node_id_t *OpenAPI_global_ran_node_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *n3_iwf_id,
    OpenAPI_gnb_id_t *g_nb_id,
    char *nge_nb_id,
    char *wagf_id,
    char *tngf_id,
    char *nid,
    char *e_nb_id
);
void OpenAPI_global_ran_node_id_free(OpenAPI_global_ran_node_id_t *global_ran_node_id);
OpenAPI_global_ran_node_id_t *OpenAPI_global_ran_node_id_parseFromJSON(cJSON *global_ran_node_idJSON);
cJSON *OpenAPI_global_ran_node_id_convertToJSON(OpenAPI_global_ran_node_id_t *global_ran_node_id);
OpenAPI_global_ran_node_id_t *OpenAPI_global_ran_node_id_copy(OpenAPI_global_ran_node_id_t *dst, OpenAPI_global_ran_node_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_global_ran_node_id_H_ */

