/*
 * anchor_smf_features.h
 *
 * Anchor SMF supported features
 */

#ifndef _OpenAPI_anchor_smf_features_H_
#define _OpenAPI_anchor_smf_features_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_anchor_smf_features_s OpenAPI_anchor_smf_features_t;
typedef struct OpenAPI_anchor_smf_features_s {
    bool is_psetr_support_ind;
    int psetr_support_ind;
} OpenAPI_anchor_smf_features_t;

OpenAPI_anchor_smf_features_t *OpenAPI_anchor_smf_features_create(
    bool is_psetr_support_ind,
    int psetr_support_ind
);
void OpenAPI_anchor_smf_features_free(OpenAPI_anchor_smf_features_t *anchor_smf_features);
OpenAPI_anchor_smf_features_t *OpenAPI_anchor_smf_features_parseFromJSON(cJSON *anchor_smf_featuresJSON);
cJSON *OpenAPI_anchor_smf_features_convertToJSON(OpenAPI_anchor_smf_features_t *anchor_smf_features);
OpenAPI_anchor_smf_features_t *OpenAPI_anchor_smf_features_copy(OpenAPI_anchor_smf_features_t *dst, OpenAPI_anchor_smf_features_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_anchor_smf_features_H_ */

