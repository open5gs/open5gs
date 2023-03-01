/*
 * dispersion_collection.h
 *
 * Dispersion collection per UE location or per slice.
 */

#ifndef _OpenAPI_dispersion_collection_H_
#define _OpenAPI_dispersion_collection_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "application_volume.h"
#include "dispersion_class.h"
#include "snssai.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dispersion_collection_s OpenAPI_dispersion_collection_t;
typedef struct OpenAPI_dispersion_collection_s {
    struct OpenAPI_user_location_s *ue_loc;
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *gpsis;
    OpenAPI_list_t *app_volumes;
    bool is_disper_amount;
    int disper_amount;
    struct OpenAPI_dispersion_class_s *disper_class;
    bool is_usage_rank;
    int usage_rank;
    bool is_percentile_rank;
    int percentile_rank;
    bool is_ue_ratio;
    int ue_ratio;
    bool is_confidence;
    int confidence;
} OpenAPI_dispersion_collection_t;

OpenAPI_dispersion_collection_t *OpenAPI_dispersion_collection_create(
    OpenAPI_user_location_t *ue_loc,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *app_volumes,
    bool is_disper_amount,
    int disper_amount,
    OpenAPI_dispersion_class_t *disper_class,
    bool is_usage_rank,
    int usage_rank,
    bool is_percentile_rank,
    int percentile_rank,
    bool is_ue_ratio,
    int ue_ratio,
    bool is_confidence,
    int confidence
);
void OpenAPI_dispersion_collection_free(OpenAPI_dispersion_collection_t *dispersion_collection);
OpenAPI_dispersion_collection_t *OpenAPI_dispersion_collection_parseFromJSON(cJSON *dispersion_collectionJSON);
cJSON *OpenAPI_dispersion_collection_convertToJSON(OpenAPI_dispersion_collection_t *dispersion_collection);
OpenAPI_dispersion_collection_t *OpenAPI_dispersion_collection_copy(OpenAPI_dispersion_collection_t *dst, OpenAPI_dispersion_collection_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_collection_H_ */

