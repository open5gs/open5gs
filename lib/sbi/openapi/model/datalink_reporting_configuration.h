/*
 * datalink_reporting_configuration.h
 *
 *
 */

#ifndef _OpenAPI_datalink_reporting_configuration_H_
#define _OpenAPI_datalink_reporting_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ddd_traffic_descriptor.h"
#include "dl_data_delivery_status.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_datalink_reporting_configuration_s OpenAPI_datalink_reporting_configuration_t;
typedef struct OpenAPI_datalink_reporting_configuration_s {
    OpenAPI_list_t *ddd_traffic_des;
    char *dnn;
    struct OpenAPI_snssai_s *slice;
    OpenAPI_list_t *ddd_status_list;
} OpenAPI_datalink_reporting_configuration_t;

OpenAPI_datalink_reporting_configuration_t *OpenAPI_datalink_reporting_configuration_create(
    OpenAPI_list_t *ddd_traffic_des,
    char *dnn,
    OpenAPI_snssai_t *slice,
    OpenAPI_list_t *ddd_status_list
    );
void OpenAPI_datalink_reporting_configuration_free(OpenAPI_datalink_reporting_configuration_t *datalink_reporting_configuration);
OpenAPI_datalink_reporting_configuration_t *OpenAPI_datalink_reporting_configuration_parseFromJSON(cJSON *datalink_reporting_configurationJSON);
cJSON *OpenAPI_datalink_reporting_configuration_convertToJSON(OpenAPI_datalink_reporting_configuration_t *datalink_reporting_configuration);
OpenAPI_datalink_reporting_configuration_t *OpenAPI_datalink_reporting_configuration_copy(OpenAPI_datalink_reporting_configuration_t *dst, OpenAPI_datalink_reporting_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_datalink_reporting_configuration_H_ */

