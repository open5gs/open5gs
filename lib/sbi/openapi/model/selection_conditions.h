/*
 * selection_conditions.h
 *
 * It contains the set of conditions that shall be evaluated to determine whether a consumer shall select a given producer. The producer shall only be selected if the evaluation of the conditions is &lt;true&gt;. The set of conditions can be represented by a single  ConditionItem or by a ConditionGroup, where the latter contains a (recursive) list of conditions joined by the \&quot;and\&quot; or \&quot;or\&quot; logical relationships. 
 */

#ifndef _OpenAPI_selection_conditions_H_
#define _OpenAPI_selection_conditions_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_selection_conditions_s OpenAPI_selection_conditions_t;
#include "identity_range.h"
#include "nf_type.h"
#include "supi_range.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_selection_conditions_s {
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
    OpenAPI_list_t *_and;
    OpenAPI_list_t *_or;
};

OpenAPI_selection_conditions_t *OpenAPI_selection_conditions_create(
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
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *_and,
    OpenAPI_list_t *_or
);
void OpenAPI_selection_conditions_free(OpenAPI_selection_conditions_t *selection_conditions);
OpenAPI_selection_conditions_t *OpenAPI_selection_conditions_parseFromJSON(cJSON *selection_conditionsJSON);
cJSON *OpenAPI_selection_conditions_convertToJSON(OpenAPI_selection_conditions_t *selection_conditions);
OpenAPI_selection_conditions_t *OpenAPI_selection_conditions_copy(OpenAPI_selection_conditions_t *dst, OpenAPI_selection_conditions_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_selection_conditions_H_ */

