/*
 * pdv_monitoring_report.h
 *
 * Packet Delay Variation reporting information.
 */

#ifndef _OpenAPI_pdv_monitoring_report_H_
#define _OpenAPI_pdv_monitoring_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdv_monitoring_report_s OpenAPI_pdv_monitoring_report_t;
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdv_monitoring_report_s {
    OpenAPI_list_t *flows;
    bool is_ul_pdv;
    int ul_pdv;
    bool is_dl_pdv;
    int dl_pdv;
    bool is_rt_pdv;
    int rt_pdv;
};

OpenAPI_pdv_monitoring_report_t *OpenAPI_pdv_monitoring_report_create(
    OpenAPI_list_t *flows,
    bool is_ul_pdv,
    int ul_pdv,
    bool is_dl_pdv,
    int dl_pdv,
    bool is_rt_pdv,
    int rt_pdv
);
void OpenAPI_pdv_monitoring_report_free(OpenAPI_pdv_monitoring_report_t *pdv_monitoring_report);
OpenAPI_pdv_monitoring_report_t *OpenAPI_pdv_monitoring_report_parseFromJSON(cJSON *pdv_monitoring_reportJSON);
cJSON *OpenAPI_pdv_monitoring_report_convertToJSON(OpenAPI_pdv_monitoring_report_t *pdv_monitoring_report);
OpenAPI_pdv_monitoring_report_t *OpenAPI_pdv_monitoring_report_copy(OpenAPI_pdv_monitoring_report_t *dst, OpenAPI_pdv_monitoring_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdv_monitoring_report_H_ */

