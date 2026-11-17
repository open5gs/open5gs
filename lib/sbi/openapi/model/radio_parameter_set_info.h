/*
 * radio_parameter_set_info.h
 *
 * 
 */

#ifndef _OpenAPI_radio_parameter_set_info_H_
#define _OpenAPI_radio_parameter_set_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_radio_parameter_set_info_s OpenAPI_radio_parameter_set_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_radio_parameter_set_info_s {
    OpenAPI_list_t *radio_parameter_set_values;
    char *change_timestamp;
};

OpenAPI_radio_parameter_set_info_t *OpenAPI_radio_parameter_set_info_create(
    OpenAPI_list_t *radio_parameter_set_values,
    char *change_timestamp
);
void OpenAPI_radio_parameter_set_info_free(OpenAPI_radio_parameter_set_info_t *radio_parameter_set_info);
OpenAPI_radio_parameter_set_info_t *OpenAPI_radio_parameter_set_info_parseFromJSON(cJSON *radio_parameter_set_infoJSON);
cJSON *OpenAPI_radio_parameter_set_info_convertToJSON(OpenAPI_radio_parameter_set_info_t *radio_parameter_set_info);
OpenAPI_radio_parameter_set_info_t *OpenAPI_radio_parameter_set_info_copy(OpenAPI_radio_parameter_set_info_t *dst, OpenAPI_radio_parameter_set_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_radio_parameter_set_info_H_ */

