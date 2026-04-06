/*
 * tss_information.h
 *
 * Represents a Tss related N2 information data part
 */

#ifndef _OpenAPI_tss_information_H_
#define _OpenAPI_tss_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tss_information_s OpenAPI_tss_information_t;
#include "n2_info_content.h"
#include "tss_rsp_per_ngran.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tss_information_s {
    char *nf_id;
    OpenAPI_list_t *tss_container;
    OpenAPI_list_t *tss_rsp_per_ngran_list;
};

OpenAPI_tss_information_t *OpenAPI_tss_information_create(
    char *nf_id,
    OpenAPI_list_t *tss_container,
    OpenAPI_list_t *tss_rsp_per_ngran_list
);
void OpenAPI_tss_information_free(OpenAPI_tss_information_t *tss_information);
OpenAPI_tss_information_t *OpenAPI_tss_information_parseFromJSON(cJSON *tss_informationJSON);
cJSON *OpenAPI_tss_information_convertToJSON(OpenAPI_tss_information_t *tss_information);
OpenAPI_tss_information_t *OpenAPI_tss_information_copy(OpenAPI_tss_information_t *dst, OpenAPI_tss_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tss_information_H_ */

