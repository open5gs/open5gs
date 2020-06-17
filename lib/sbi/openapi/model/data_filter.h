/*
 * data_filter.h
 *
 * Identifies a data filter.
 */

#ifndef _OpenAPI_data_filter_H_
#define _OpenAPI_data_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "data_ind.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_data_filter_s OpenAPI_data_filter_t;
typedef struct OpenAPI_data_filter_s {
    struct OpenAPI_data_ind_s *data_ind;
    OpenAPI_list_t *dnns;
    OpenAPI_list_t *snssais;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *app_ids;
    OpenAPI_list_t *ue_ipv4s;
    OpenAPI_list_t *ue_ipv6s;
    OpenAPI_list_t *ue_macs;
} OpenAPI_data_filter_t;

OpenAPI_data_filter_t *OpenAPI_data_filter_create(
    OpenAPI_data_ind_t *data_ind,
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *snssais,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *ue_ipv4s,
    OpenAPI_list_t *ue_ipv6s,
    OpenAPI_list_t *ue_macs
    );
void OpenAPI_data_filter_free(OpenAPI_data_filter_t *data_filter);
OpenAPI_data_filter_t *OpenAPI_data_filter_parseFromJSON(cJSON *data_filterJSON);
cJSON *OpenAPI_data_filter_convertToJSON(OpenAPI_data_filter_t *data_filter);
OpenAPI_data_filter_t *OpenAPI_data_filter_copy(OpenAPI_data_filter_t *dst, OpenAPI_data_filter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_filter_H_ */

