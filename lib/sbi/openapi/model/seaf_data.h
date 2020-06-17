/*
 * seaf_data.h
 *
 *
 */

#ifndef _OpenAPI_seaf_data_H_
#define _OpenAPI_seaf_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "key_amf.h"
#include "ng_ksi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_seaf_data_s OpenAPI_seaf_data_t;
typedef struct OpenAPI_seaf_data_s {
    struct OpenAPI_ng_ksi_s *ng_ksi;
    struct OpenAPI_key_amf_s *key_amf;
    char *nh;
    int ncc;
    int key_amf_change_ind;
    int key_amf_h_derivation_ind;
} OpenAPI_seaf_data_t;

OpenAPI_seaf_data_t *OpenAPI_seaf_data_create(
    OpenAPI_ng_ksi_t *ng_ksi,
    OpenAPI_key_amf_t *key_amf,
    char *nh,
    int ncc,
    int key_amf_change_ind,
    int key_amf_h_derivation_ind
    );
void OpenAPI_seaf_data_free(OpenAPI_seaf_data_t *seaf_data);
OpenAPI_seaf_data_t *OpenAPI_seaf_data_parseFromJSON(cJSON *seaf_dataJSON);
cJSON *OpenAPI_seaf_data_convertToJSON(OpenAPI_seaf_data_t *seaf_data);
OpenAPI_seaf_data_t *OpenAPI_seaf_data_copy(OpenAPI_seaf_data_t *dst, OpenAPI_seaf_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_seaf_data_H_ */

