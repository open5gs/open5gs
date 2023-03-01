/*
 * nf_load_level_information.h
 *
 * Represents load level information of a given NF instance.
 */

#ifndef _OpenAPI_nf_load_level_information_H_
#define _OpenAPI_nf_load_level_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"
#include "nnwdaf_nf_status.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_load_level_information_s OpenAPI_nf_load_level_information_t;
typedef struct OpenAPI_nf_load_level_information_s {
    OpenAPI_nf_type_e nf_type;
    char *nf_instance_id;
    char *nf_set_id;
    struct OpenAPI_nnwdaf_nf_status_s *nf_status;
    bool is_nf_cpu_usage;
    int nf_cpu_usage;
    bool is_nf_memory_usage;
    int nf_memory_usage;
    bool is_nf_storage_usage;
    int nf_storage_usage;
    bool is_nf_load_level_average;
    int nf_load_level_average;
    bool is_nf_load_levelpeak;
    int nf_load_levelpeak;
    bool is_nf_load_avg_in_aoi;
    int nf_load_avg_in_aoi;
    struct OpenAPI_snssai_s *snssai;
    bool is_confidence;
    int confidence;
} OpenAPI_nf_load_level_information_t;

OpenAPI_nf_load_level_information_t *OpenAPI_nf_load_level_information_create(
    OpenAPI_nf_type_e nf_type,
    char *nf_instance_id,
    char *nf_set_id,
    OpenAPI_nnwdaf_nf_status_t *nf_status,
    bool is_nf_cpu_usage,
    int nf_cpu_usage,
    bool is_nf_memory_usage,
    int nf_memory_usage,
    bool is_nf_storage_usage,
    int nf_storage_usage,
    bool is_nf_load_level_average,
    int nf_load_level_average,
    bool is_nf_load_levelpeak,
    int nf_load_levelpeak,
    bool is_nf_load_avg_in_aoi,
    int nf_load_avg_in_aoi,
    OpenAPI_snssai_t *snssai,
    bool is_confidence,
    int confidence
);
void OpenAPI_nf_load_level_information_free(OpenAPI_nf_load_level_information_t *nf_load_level_information);
OpenAPI_nf_load_level_information_t *OpenAPI_nf_load_level_information_parseFromJSON(cJSON *nf_load_level_informationJSON);
cJSON *OpenAPI_nf_load_level_information_convertToJSON(OpenAPI_nf_load_level_information_t *nf_load_level_information);
OpenAPI_nf_load_level_information_t *OpenAPI_nf_load_level_information_copy(OpenAPI_nf_load_level_information_t *dst, OpenAPI_nf_load_level_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_load_level_information_H_ */

