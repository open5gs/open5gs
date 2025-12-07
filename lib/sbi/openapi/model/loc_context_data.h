/*
 * loc_context_data.h
 *
 * Information within Transfer Location Context Request.
 */

#ifndef _OpenAPI_loc_context_data_H_
#define _OpenAPI_loc_context_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "area_event_info.h"
#include "ecgi.h"
#include "event_report_message.h"
#include "event_reporting_status.h"
#include "guami.h"
#include "ldr_type.h"
#include "location_qo_s.h"
#include "motion_event_info.h"
#include "ncgi.h"
#include "periodic_event_info.h"
#include "supported_gad_shapes.h"
#include "ue_location_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_loc_context_data_s OpenAPI_loc_context_data_t;
typedef struct OpenAPI_loc_context_data_s {
    char *amf_id;
    struct OpenAPI_location_qo_s_s *location_qo_s;
    OpenAPI_list_t *supported_gad_shapes;
    char *supi;
    char *gpsi;
    struct OpenAPI_ldr_type_s *ldr_type;
    char *hgmlc_call_back_uri;
    char *ldr_reference;
    struct OpenAPI_periodic_event_info_s *periodic_event_info;
    struct OpenAPI_area_event_info_s *area_event_info;
    struct OpenAPI_motion_event_info_s *motion_event_info;
    struct OpenAPI_event_report_message_s *event_report_message;
    struct OpenAPI_event_reporting_status_s *event_reporting_status;
    struct OpenAPI_ue_location_info_s *ue_location_info;
    bool is_c_io_t5_gs_optimisation;
    int c_io_t5_gs_optimisation;
    struct OpenAPI_ecgi_s *ecgi;
    struct OpenAPI_ncgi_s *ncgi;
    struct OpenAPI_guami_s *guami;
    char *supported_features;
    char *ue_positioning_cap;
    char *scheduled_loc_time;
} OpenAPI_loc_context_data_t;

OpenAPI_loc_context_data_t *OpenAPI_loc_context_data_create(
    char *amf_id,
    OpenAPI_location_qo_s_t *location_qo_s,
    OpenAPI_list_t *supported_gad_shapes,
    char *supi,
    char *gpsi,
    OpenAPI_ldr_type_t *ldr_type,
    char *hgmlc_call_back_uri,
    char *ldr_reference,
    OpenAPI_periodic_event_info_t *periodic_event_info,
    OpenAPI_area_event_info_t *area_event_info,
    OpenAPI_motion_event_info_t *motion_event_info,
    OpenAPI_event_report_message_t *event_report_message,
    OpenAPI_event_reporting_status_t *event_reporting_status,
    OpenAPI_ue_location_info_t *ue_location_info,
    bool is_c_io_t5_gs_optimisation,
    int c_io_t5_gs_optimisation,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_ncgi_t *ncgi,
    OpenAPI_guami_t *guami,
    char *supported_features,
    char *ue_positioning_cap,
    char *scheduled_loc_time
);
void OpenAPI_loc_context_data_free(OpenAPI_loc_context_data_t *loc_context_data);
OpenAPI_loc_context_data_t *OpenAPI_loc_context_data_parseFromJSON(cJSON *loc_context_dataJSON);
cJSON *OpenAPI_loc_context_data_convertToJSON(OpenAPI_loc_context_data_t *loc_context_data);
OpenAPI_loc_context_data_t *OpenAPI_loc_context_data_copy(OpenAPI_loc_context_data_t *dst, OpenAPI_loc_context_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_loc_context_data_H_ */

