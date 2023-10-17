/*
 * datalink_reporting_configuration_1.h
 *
 * 
 */

#ifndef _OpenAPI_datalink_reporting_configuration_1_H_
#define _OpenAPI_datalink_reporting_configuration_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ddd_traffic_descriptor_1.h"
#include "dl_data_delivery_status.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_datalink_reporting_configuration_1_s OpenAPI_datalink_reporting_configuration_1_t;
typedef struct OpenAPI_datalink_reporting_configuration_1_s {
    OpenAPI_list_t *ddd_traffic_des;
    char *dnn;
    struct OpenAPI_snssai_s *slice;
    OpenAPI_list_t *ddd_status_list;
} OpenAPI_datalink_reporting_configuration_1_t;

OpenAPI_datalink_reporting_configuration_1_t *OpenAPI_datalink_reporting_configuration_1_create(
    OpenAPI_list_t *ddd_traffic_des,
    char *dnn,
    OpenAPI_snssai_t *slice,
    OpenAPI_list_t *ddd_status_list
);
void OpenAPI_datalink_reporting_configuration_1_free(OpenAPI_datalink_reporting_configuration_1_t *datalink_reporting_configuration_1);
OpenAPI_datalink_reporting_configuration_1_t *OpenAPI_datalink_reporting_configuration_1_parseFromJSON(cJSON *datalink_reporting_configuration_1JSON);
cJSON *OpenAPI_datalink_reporting_configuration_1_convertToJSON(OpenAPI_datalink_reporting_configuration_1_t *datalink_reporting_configuration_1);
OpenAPI_datalink_reporting_configuration_1_t *OpenAPI_datalink_reporting_configuration_1_copy(OpenAPI_datalink_reporting_configuration_1_t *dst, OpenAPI_datalink_reporting_configuration_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_datalink_reporting_configuration_1_H_ */

