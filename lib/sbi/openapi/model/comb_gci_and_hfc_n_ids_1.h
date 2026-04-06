/*
 * comb_gci_and_hfc_n_ids_1.h
 *
 * Combined Global Cable Identifier and HFC Node Id 
 */

#ifndef _OpenAPI_comb_gci_and_hfc_n_ids_1_H_
#define _OpenAPI_comb_gci_and_hfc_n_ids_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_comb_gci_and_hfc_n_ids_1_s OpenAPI_comb_gci_and_hfc_n_ids_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_comb_gci_and_hfc_n_ids_1_s {
    char *global_cable_id;
    char *hfc_nid;
};

OpenAPI_comb_gci_and_hfc_n_ids_1_t *OpenAPI_comb_gci_and_hfc_n_ids_1_create(
    char *global_cable_id,
    char *hfc_nid
);
void OpenAPI_comb_gci_and_hfc_n_ids_1_free(OpenAPI_comb_gci_and_hfc_n_ids_1_t *comb_gci_and_hfc_n_ids_1);
OpenAPI_comb_gci_and_hfc_n_ids_1_t *OpenAPI_comb_gci_and_hfc_n_ids_1_parseFromJSON(cJSON *comb_gci_and_hfc_n_ids_1JSON);
cJSON *OpenAPI_comb_gci_and_hfc_n_ids_1_convertToJSON(OpenAPI_comb_gci_and_hfc_n_ids_1_t *comb_gci_and_hfc_n_ids_1);
OpenAPI_comb_gci_and_hfc_n_ids_1_t *OpenAPI_comb_gci_and_hfc_n_ids_1_copy(OpenAPI_comb_gci_and_hfc_n_ids_1_t *dst, OpenAPI_comb_gci_and_hfc_n_ids_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_comb_gci_and_hfc_n_ids_1_H_ */

