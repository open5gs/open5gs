/*
 * traffic_characterization.h
 *
 * Identifies the detailed traffic characterization.
 */

#ifndef _OpenAPI_traffic_characterization_H_
#define _OpenAPI_traffic_characterization_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_eth_flow_description.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_characterization_s OpenAPI_traffic_characterization_t;
typedef struct OpenAPI_traffic_characterization_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *app_id;
    OpenAPI_list_t *f_descs;
    bool is_ul_vol;
    long ul_vol;
    bool is_ul_vol_variance;
    float ul_vol_variance;
    bool is_dl_vol;
    long dl_vol;
    bool is_dl_vol_variance;
    float dl_vol_variance;
} OpenAPI_traffic_characterization_t;

OpenAPI_traffic_characterization_t *OpenAPI_traffic_characterization_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *app_id,
    OpenAPI_list_t *f_descs,
    bool is_ul_vol,
    long ul_vol,
    bool is_ul_vol_variance,
    float ul_vol_variance,
    bool is_dl_vol,
    long dl_vol,
    bool is_dl_vol_variance,
    float dl_vol_variance
);
void OpenAPI_traffic_characterization_free(OpenAPI_traffic_characterization_t *traffic_characterization);
OpenAPI_traffic_characterization_t *OpenAPI_traffic_characterization_parseFromJSON(cJSON *traffic_characterizationJSON);
cJSON *OpenAPI_traffic_characterization_convertToJSON(OpenAPI_traffic_characterization_t *traffic_characterization);
OpenAPI_traffic_characterization_t *OpenAPI_traffic_characterization_copy(OpenAPI_traffic_characterization_t *dst, OpenAPI_traffic_characterization_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_characterization_H_ */

