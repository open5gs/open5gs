/*
 * network_description.h
 *
 * Represents the description of a PLMN, by the definition of the PLMN ID, the MCC (and applicable MNC(s)) or the indication of any PLMN. 
 */

#ifndef _OpenAPI_network_description_H_
#define _OpenAPI_network_description_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_network_description_s OpenAPI_network_description_t;
#include "plmn_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_network_description_s {
    struct OpenAPI_plmn_id_1_s *plmn_id;
    char *mcc;
    OpenAPI_list_t *mncs;
    bool is_any_plmn_ind;
    int any_plmn_ind;
};

OpenAPI_network_description_t *OpenAPI_network_description_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *mcc,
    OpenAPI_list_t *mncs,
    bool is_any_plmn_ind,
    int any_plmn_ind
);
void OpenAPI_network_description_free(OpenAPI_network_description_t *network_description);
OpenAPI_network_description_t *OpenAPI_network_description_parseFromJSON(cJSON *network_descriptionJSON);
cJSON *OpenAPI_network_description_convertToJSON(OpenAPI_network_description_t *network_description);
OpenAPI_network_description_t *OpenAPI_network_description_copy(OpenAPI_network_description_t *dst, OpenAPI_network_description_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_description_H_ */

