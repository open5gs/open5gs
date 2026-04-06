/*
 * comb_gci_and_hfc_n_ids.h
 *
 * Combined Global Cable Identifier and HFC Node Id 
 */

#ifndef _OpenAPI_comb_gci_and_hfc_n_ids_H_
#define _OpenAPI_comb_gci_and_hfc_n_ids_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_comb_gci_and_hfc_n_ids_s OpenAPI_comb_gci_and_hfc_n_ids_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_comb_gci_and_hfc_n_ids_s {
    char *global_cable_id;
    char *hfc_nid;
};

OpenAPI_comb_gci_and_hfc_n_ids_t *OpenAPI_comb_gci_and_hfc_n_ids_create(
    char *global_cable_id,
    char *hfc_nid
);
void OpenAPI_comb_gci_and_hfc_n_ids_free(OpenAPI_comb_gci_and_hfc_n_ids_t *comb_gci_and_hfc_n_ids);
OpenAPI_comb_gci_and_hfc_n_ids_t *OpenAPI_comb_gci_and_hfc_n_ids_parseFromJSON(cJSON *comb_gci_and_hfc_n_idsJSON);
cJSON *OpenAPI_comb_gci_and_hfc_n_ids_convertToJSON(OpenAPI_comb_gci_and_hfc_n_ids_t *comb_gci_and_hfc_n_ids);
OpenAPI_comb_gci_and_hfc_n_ids_t *OpenAPI_comb_gci_and_hfc_n_ids_copy(OpenAPI_comb_gci_and_hfc_n_ids_t *dst, OpenAPI_comb_gci_and_hfc_n_ids_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_comb_gci_and_hfc_n_ids_H_ */

