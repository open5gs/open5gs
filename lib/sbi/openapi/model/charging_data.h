/*
 * charging_data.h
 *
 *
 */

#ifndef _OpenAPI_charging_data_H_
#define _OpenAPI_charging_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "metering_method.h"
#include "reporting_level.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_charging_data_s OpenAPI_charging_data_t;
typedef struct OpenAPI_charging_data_s {
    char *chg_id;
    struct OpenAPI_metering_method_s *metering_method;
    int offline;
    int online;
    int sdf_handl;
    int rating_group;
    struct OpenAPI_reporting_level_s *reporting_level;
    int service_id;
    char *sponsor_id;
    char *app_svc_prov_id;
    int af_charging_identifier;
    char *af_charg_id;
} OpenAPI_charging_data_t;

OpenAPI_charging_data_t *OpenAPI_charging_data_create(
    char *chg_id,
    OpenAPI_metering_method_t *metering_method,
    int offline,
    int online,
    int sdf_handl,
    int rating_group,
    OpenAPI_reporting_level_t *reporting_level,
    int service_id,
    char *sponsor_id,
    char *app_svc_prov_id,
    int af_charging_identifier,
    char *af_charg_id
    );
void OpenAPI_charging_data_free(OpenAPI_charging_data_t *charging_data);
OpenAPI_charging_data_t *OpenAPI_charging_data_parseFromJSON(cJSON *charging_dataJSON);
cJSON *OpenAPI_charging_data_convertToJSON(OpenAPI_charging_data_t *charging_data);
OpenAPI_charging_data_t *OpenAPI_charging_data_copy(OpenAPI_charging_data_t *dst, OpenAPI_charging_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_data_H_ */

