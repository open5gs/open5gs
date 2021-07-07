/*
 * global_ran_node_id_1.h
 *
 * 
 */

#ifndef _OpenAPI_global_ran_node_id_1_H_
#define _OpenAPI_global_ran_node_id_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gnb_id.h"
#include "plmn_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_global_ran_node_id_1_s OpenAPI_global_ran_node_id_1_t;
typedef struct OpenAPI_global_ran_node_id_1_s {
    struct OpenAPI_plmn_id_1_s *plmn_id;
    char *n3_iwf_id;
    struct OpenAPI_gnb_id_s *g_nb_id;
    char *nge_nb_id;
    char *wagf_id;
    char *tngf_id;
    char *nid;
    char *e_nb_id;
} OpenAPI_global_ran_node_id_1_t;

OpenAPI_global_ran_node_id_1_t *OpenAPI_global_ran_node_id_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *n3_iwf_id,
    OpenAPI_gnb_id_t *g_nb_id,
    char *nge_nb_id,
    char *wagf_id,
    char *tngf_id,
    char *nid,
    char *e_nb_id
);
void OpenAPI_global_ran_node_id_1_free(OpenAPI_global_ran_node_id_1_t *global_ran_node_id_1);
OpenAPI_global_ran_node_id_1_t *OpenAPI_global_ran_node_id_1_parseFromJSON(cJSON *global_ran_node_id_1JSON);
cJSON *OpenAPI_global_ran_node_id_1_convertToJSON(OpenAPI_global_ran_node_id_1_t *global_ran_node_id_1);
OpenAPI_global_ran_node_id_1_t *OpenAPI_global_ran_node_id_1_copy(OpenAPI_global_ran_node_id_1_t *dst, OpenAPI_global_ran_node_id_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_global_ran_node_id_1_H_ */

