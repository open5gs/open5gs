/*
 * dn_perf.h
 *
 * Represents DN performance for the application.
 */

#ifndef _OpenAPI_dn_perf_H_
#define _OpenAPI_dn_perf_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "addr_fqdn.h"
#include "network_area_info.h"
#include "perf_data.h"
#include "time_window.h"
#include "upf_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dn_perf_s OpenAPI_dn_perf_t;
typedef struct OpenAPI_dn_perf_s {
    struct OpenAPI_addr_fqdn_s *app_server_ins_addr;
    struct OpenAPI_upf_information_s *upf_info;
    char *dnai;
    struct OpenAPI_perf_data_s *perf_data;
    struct OpenAPI_network_area_info_s *spatial_valid_con;
    struct OpenAPI_time_window_s *temporal_valid_con;
} OpenAPI_dn_perf_t;

OpenAPI_dn_perf_t *OpenAPI_dn_perf_create(
    OpenAPI_addr_fqdn_t *app_server_ins_addr,
    OpenAPI_upf_information_t *upf_info,
    char *dnai,
    OpenAPI_perf_data_t *perf_data,
    OpenAPI_network_area_info_t *spatial_valid_con,
    OpenAPI_time_window_t *temporal_valid_con
);
void OpenAPI_dn_perf_free(OpenAPI_dn_perf_t *dn_perf);
OpenAPI_dn_perf_t *OpenAPI_dn_perf_parseFromJSON(cJSON *dn_perfJSON);
cJSON *OpenAPI_dn_perf_convertToJSON(OpenAPI_dn_perf_t *dn_perf);
OpenAPI_dn_perf_t *OpenAPI_dn_perf_copy(OpenAPI_dn_perf_t *dst, OpenAPI_dn_perf_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dn_perf_H_ */

