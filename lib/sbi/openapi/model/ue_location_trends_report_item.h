/*
 * ue_location_trends_report_item.h
 *
 * Report Item for UE Location Trends event.
 */

#ifndef _OpenAPI_ue_location_trends_report_item_H_
#define _OpenAPI_ue_location_trends_report_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_location_trends_report_item_s OpenAPI_ue_location_trends_report_item_t;
#include "ecgi.h"
#include "global_ran_node_id.h"
#include "n3ga_location.h"
#include "ncgi.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_location_trends_report_item_s {
    struct OpenAPI_tai_s *tai;
    struct OpenAPI_global_ran_node_id_s *ran_node_id;
    struct OpenAPI_ncgi_s *ncgi;
    struct OpenAPI_ecgi_s *ecgi;
    struct OpenAPI_n3ga_location_s *n3ga_location;
    int spacing;
    bool is_spacing_var;
    float spacing_var;
    int duration;
    bool is_duration_var;
    float duration_var;
    bool is_occurrences;
    int occurrences;
    char *timestamp;
};

OpenAPI_ue_location_trends_report_item_t *OpenAPI_ue_location_trends_report_item_create(
    OpenAPI_tai_t *tai,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_ncgi_t *ncgi,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_n3ga_location_t *n3ga_location,
    int spacing,
    bool is_spacing_var,
    float spacing_var,
    int duration,
    bool is_duration_var,
    float duration_var,
    bool is_occurrences,
    int occurrences,
    char *timestamp
);
void OpenAPI_ue_location_trends_report_item_free(OpenAPI_ue_location_trends_report_item_t *ue_location_trends_report_item);
OpenAPI_ue_location_trends_report_item_t *OpenAPI_ue_location_trends_report_item_parseFromJSON(cJSON *ue_location_trends_report_itemJSON);
cJSON *OpenAPI_ue_location_trends_report_item_convertToJSON(OpenAPI_ue_location_trends_report_item_t *ue_location_trends_report_item);
OpenAPI_ue_location_trends_report_item_t *OpenAPI_ue_location_trends_report_item_copy(OpenAPI_ue_location_trends_report_item_t *dst, OpenAPI_ue_location_trends_report_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_location_trends_report_item_H_ */

