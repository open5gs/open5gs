/*
 * search_result.h
 *
 *
 */

#ifndef _OpenAPILinksH_
#define _OpenAPILinksH_

#include <string.h>
#include "../openapi/external/cJSON.h"
#include "../openapi/include/list.h"
#include "../openapi/include/keyValuePair.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_links_s ogs_sbi_links_t;
typedef struct ogs_sbi_links_s {
    OpenAPI_list_t *items;
    char *self;
} ogs_sbi_links_t;

cJSON *ogs_sbi_links_convertToJSON(ogs_sbi_links_t *links);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPILinksH_ */

