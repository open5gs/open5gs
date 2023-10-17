/*
 * plmn_id_nid.h
 *
 * Contains the serving core network operator PLMN ID and, for an SNPN, the NID that together with the PLMN ID identifies the SNPN. 
 */

#ifndef _OpenAPI_plmn_id_nid_H_
#define _OpenAPI_plmn_id_nid_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_id_nid_s OpenAPI_plmn_id_nid_t;
typedef struct OpenAPI_plmn_id_nid_s {
    char *mcc;
    char *mnc;
    char *nid;
} OpenAPI_plmn_id_nid_t;

OpenAPI_plmn_id_nid_t *OpenAPI_plmn_id_nid_create(
    char *mcc,
    char *mnc,
    char *nid
);
void OpenAPI_plmn_id_nid_free(OpenAPI_plmn_id_nid_t *plmn_id_nid);
OpenAPI_plmn_id_nid_t *OpenAPI_plmn_id_nid_parseFromJSON(cJSON *plmn_id_nidJSON);
cJSON *OpenAPI_plmn_id_nid_convertToJSON(OpenAPI_plmn_id_nid_t *plmn_id_nid);
OpenAPI_plmn_id_nid_t *OpenAPI_plmn_id_nid_copy(OpenAPI_plmn_id_nid_t *dst, OpenAPI_plmn_id_nid_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_id_nid_H_ */

