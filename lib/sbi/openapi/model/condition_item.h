/*
 * condition_item.h
 *
 * 
 */

#ifndef _OpenAPI_condition_item_H_
#define _OpenAPI_condition_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_condition_item_s OpenAPI_condition_item_t;
#include "identity_range.h"
#include "nf_type.h"
#include "supi_range.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_condition_item_s {
    OpenAPI_list_t *consumer_nf_types;
    bool is_service_feature;
    int service_feature;
    bool is_vs_service_feature;
    int vs_service_feature;
    OpenAPI_list_t *supi_range_list;
    OpenAPI_list_t *gpsi_range_list;
    OpenAPI_list_t *impu_range_list;
    OpenAPI_list_t *impi_range_list;
    OpenAPI_list_t *pei_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *dnn_list;
};

OpenAPI_condition_item_t *OpenAPI_condition_item_create(
    OpenAPI_list_t *consumer_nf_types,
    bool is_service_feature,
    int service_feature,
    bool is_vs_service_feature,
    int vs_service_feature,
    OpenAPI_list_t *supi_range_list,
    OpenAPI_list_t *gpsi_range_list,
    OpenAPI_list_t *impu_range_list,
    OpenAPI_list_t *impi_range_list,
    OpenAPI_list_t *pei_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *dnn_list
);
void OpenAPI_condition_item_free(OpenAPI_condition_item_t *condition_item);
OpenAPI_condition_item_t *OpenAPI_condition_item_parseFromJSON(cJSON *condition_itemJSON);
cJSON *OpenAPI_condition_item_convertToJSON(OpenAPI_condition_item_t *condition_item);
OpenAPI_condition_item_t *OpenAPI_condition_item_copy(OpenAPI_condition_item_t *dst, OpenAPI_condition_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_condition_item_H_ */

