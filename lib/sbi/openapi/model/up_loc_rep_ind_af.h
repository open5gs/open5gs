/*
 * up_loc_rep_ind_af.h
 *
 * Indicates one of the mutually exclusive global settings for location reporting over user plane connectivity to an LCS client or AF. 
 */

#ifndef _OpenAPI_up_loc_rep_ind_af_H_
#define _OpenAPI_up_loc_rep_ind_af_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_up_loc_rep_ind_af_NULL = 0, OpenAPI_up_loc_rep_ind_af_USER_PLANE_REPORT_ALLOWED, OpenAPI_up_loc_rep_ind_af_USER_PLANE_REPORT_NOT_ALLOWED } OpenAPI_up_loc_rep_ind_af_e;

char* OpenAPI_up_loc_rep_ind_af_ToString(OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af);

OpenAPI_up_loc_rep_ind_af_e OpenAPI_up_loc_rep_ind_af_FromString(char* up_loc_rep_ind_af);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_loc_rep_ind_af_H_ */

