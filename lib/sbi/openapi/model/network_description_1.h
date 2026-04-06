/*
 * network_description_1.h
 *
 * Represents the description of a PLMN, by the definition of the PLMN ID, the MCC (and applicable MNC(s)) or the indication of any PLMN. 
 */

#ifndef _OpenAPI_network_description_1_H_
#define _OpenAPI_network_description_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_network_description_1_s OpenAPI_network_description_1_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_network_description_1_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *mcc;
    OpenAPI_list_t *mncs;
    bool is_any_plmn_ind;
    int any_plmn_ind;
};

OpenAPI_network_description_1_t *OpenAPI_network_description_1_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *mcc,
    OpenAPI_list_t *mncs,
    bool is_any_plmn_ind,
    int any_plmn_ind
);
void OpenAPI_network_description_1_free(OpenAPI_network_description_1_t *network_description_1);
OpenAPI_network_description_1_t *OpenAPI_network_description_1_parseFromJSON(cJSON *network_description_1JSON);
cJSON *OpenAPI_network_description_1_convertToJSON(OpenAPI_network_description_1_t *network_description_1);
OpenAPI_network_description_1_t *OpenAPI_network_description_1_copy(OpenAPI_network_description_1_t *dst, OpenAPI_network_description_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_description_1_H_ */

