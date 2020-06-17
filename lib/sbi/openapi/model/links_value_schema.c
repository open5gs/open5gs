
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "links_value_schema.h"

OpenAPI_links_value_schema_t *OpenAPI_links_value_schema_create(
    char *href
    )
{
    OpenAPI_links_value_schema_t *links_value_schema_local_var = OpenAPI_malloc(sizeof(OpenAPI_links_value_schema_t));
    if (!links_value_schema_local_var) {
        return NULL;
    }
    links_value_schema_local_var->href = href;

    return links_value_schema_local_var;
}

void OpenAPI_links_value_schema_free(OpenAPI_links_value_schema_t *links_value_schema)
{
    if (NULL == links_value_schema) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(links_value_schema->href);
    ogs_free(links_value_schema);
}

cJSON *OpenAPI_links_value_schema_convertToJSON(OpenAPI_links_value_schema_t *links_value_schema)
{
    cJSON *item = NULL;

    if (links_value_schema == NULL) {
        ogs_error("OpenAPI_links_value_schema_convertToJSON() failed [LinksValueSchema]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (links_value_schema->href) {
        if (cJSON_AddStringToObject(item, "href", links_value_schema->href) == NULL) {
            ogs_error("OpenAPI_links_value_schema_convertToJSON() failed [href]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_links_value_schema_t *OpenAPI_links_value_schema_parseFromJSON(cJSON *links_value_schemaJSON)
{
    OpenAPI_links_value_schema_t *links_value_schema_local_var = NULL;
    cJSON *href = cJSON_GetObjectItemCaseSensitive(links_value_schemaJSON, "href");

    if (href) {
        if (!cJSON_IsString(href)) {
            ogs_error("OpenAPI_links_value_schema_parseFromJSON() failed [href]");
            goto end;
        }
    }

    links_value_schema_local_var = OpenAPI_links_value_schema_create (
        href ? ogs_strdup(href->valuestring) : NULL
        );

    return links_value_schema_local_var;
end:
    return NULL;
}

OpenAPI_links_value_schema_t *OpenAPI_links_value_schema_copy(OpenAPI_links_value_schema_t *dst, OpenAPI_links_value_schema_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_links_value_schema_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_links_value_schema_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_links_value_schema_free(dst);
    dst = OpenAPI_links_value_schema_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

