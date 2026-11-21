/*
 * ee_perf_req.h
 *
 * 
 */

#ifndef _OpenAPI_ee_perf_req_H_
#define _OpenAPI_ee_perf_req_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ee_perf_req_s OpenAPI_ee_perf_req_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ee_perf_req_KPITYPE_NULL = 0, OpenAPI_ee_perf_req_KPITYPE_NUMOFBITS, OpenAPI_ee_perf_req_KPITYPE_NUMOFBITS_RANBASED } OpenAPI_ee_perf_req_kpi_type_e;

char* OpenAPI_ee_perf_req_kpi_type_ToString(OpenAPI_ee_perf_req_kpi_type_e kpi_type);

OpenAPI_ee_perf_req_kpi_type_e OpenAPI_ee_perf_req_kpi_type_FromString(char* kpi_type);
struct OpenAPI_ee_perf_req_s {
    OpenAPI_ee_perf_req_kpi_type_e kpi_type;
    bool is_req;
    double req;
};

OpenAPI_ee_perf_req_t *OpenAPI_ee_perf_req_create(
    OpenAPI_ee_perf_req_kpi_type_e kpi_type,
    bool is_req,
    double req
);
void OpenAPI_ee_perf_req_free(OpenAPI_ee_perf_req_t *ee_perf_req);
OpenAPI_ee_perf_req_t *OpenAPI_ee_perf_req_parseFromJSON(cJSON *ee_perf_reqJSON);
cJSON *OpenAPI_ee_perf_req_convertToJSON(OpenAPI_ee_perf_req_t *ee_perf_req);
OpenAPI_ee_perf_req_t *OpenAPI_ee_perf_req_copy(OpenAPI_ee_perf_req_t *dst, OpenAPI_ee_perf_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ee_perf_req_H_ */

