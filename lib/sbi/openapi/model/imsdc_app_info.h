/*
 * imsdc_app_info.h
 *
 * 
 */

#ifndef _OpenAPI_imsdc_app_info_H_
#define _OpenAPI_imsdc_app_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_imsdc_app_info_s OpenAPI_imsdc_app_info_t;
#include "replace_http_url.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_imsdc_app_info_s {
    char *application_id;
    struct OpenAPI_replace_http_url_s *http_url;
};

OpenAPI_imsdc_app_info_t *OpenAPI_imsdc_app_info_create(
    char *application_id,
    OpenAPI_replace_http_url_t *http_url
);
void OpenAPI_imsdc_app_info_free(OpenAPI_imsdc_app_info_t *imsdc_app_info);
OpenAPI_imsdc_app_info_t *OpenAPI_imsdc_app_info_parseFromJSON(cJSON *imsdc_app_infoJSON);
cJSON *OpenAPI_imsdc_app_info_convertToJSON(OpenAPI_imsdc_app_info_t *imsdc_app_info);
OpenAPI_imsdc_app_info_t *OpenAPI_imsdc_app_info_copy(OpenAPI_imsdc_app_info_t *dst, OpenAPI_imsdc_app_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_imsdc_app_info_H_ */

