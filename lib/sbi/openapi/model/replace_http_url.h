/*
 * replace_http_url.h
 *
 * replacement HTTP URL per stream
 */

#ifndef _OpenAPI_replace_http_url_H_
#define _OpenAPI_replace_http_url_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_replace_http_url_s OpenAPI_replace_http_url_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_replace_http_url_s {
    char *replace_http_url;
    bool is_stream_id;
    int stream_id;
};

OpenAPI_replace_http_url_t *OpenAPI_replace_http_url_create(
    char *replace_http_url,
    bool is_stream_id,
    int stream_id
);
void OpenAPI_replace_http_url_free(OpenAPI_replace_http_url_t *replace_http_url);
OpenAPI_replace_http_url_t *OpenAPI_replace_http_url_parseFromJSON(cJSON *replace_http_urlJSON);
cJSON *OpenAPI_replace_http_url_convertToJSON(OpenAPI_replace_http_url_t *replace_http_url);
OpenAPI_replace_http_url_t *OpenAPI_replace_http_url_copy(OpenAPI_replace_http_url_t *dst, OpenAPI_replace_http_url_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_replace_http_url_H_ */

