/*
 * nsacf_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_nsacf_charging_information_H_
#define _OpenAPI_nsacf_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nsacf_charging_information_s OpenAPI_nsacf_charging_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nsacf_charging_information_s {
    bool is_n_sac_charging_indicator;
    int n_sac_charging_indicator;
};

OpenAPI_nsacf_charging_information_t *OpenAPI_nsacf_charging_information_create(
    bool is_n_sac_charging_indicator,
    int n_sac_charging_indicator
);
void OpenAPI_nsacf_charging_information_free(OpenAPI_nsacf_charging_information_t *nsacf_charging_information);
OpenAPI_nsacf_charging_information_t *OpenAPI_nsacf_charging_information_parseFromJSON(cJSON *nsacf_charging_informationJSON);
cJSON *OpenAPI_nsacf_charging_information_convertToJSON(OpenAPI_nsacf_charging_information_t *nsacf_charging_information);
OpenAPI_nsacf_charging_information_t *OpenAPI_nsacf_charging_information_copy(OpenAPI_nsacf_charging_information_t *dst, OpenAPI_nsacf_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsacf_charging_information_H_ */

