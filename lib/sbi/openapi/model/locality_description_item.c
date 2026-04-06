
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "locality_description_item.h"

OpenAPI_locality_description_item_t *OpenAPI_locality_description_item_create(
    OpenAPI_locality_type_e locality_type,
    char *locality_value
)
{
    OpenAPI_locality_description_item_t *locality_description_item_local_var = ogs_malloc(sizeof(OpenAPI_locality_description_item_t));
    ogs_assert(locality_description_item_local_var);

    locality_description_item_local_var->locality_type = locality_type;
    locality_description_item_local_var->locality_value = locality_value;

    return locality_description_item_local_var;
}

void OpenAPI_locality_description_item_free(OpenAPI_locality_description_item_t *locality_description_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == locality_description_item) {
        return;
    }
    if (locality_description_item->locality_value) {
        ogs_free(locality_description_item->locality_value);
        locality_description_item->locality_value = NULL;
    }
    ogs_free(locality_description_item);
}

cJSON *OpenAPI_locality_description_item_convertToJSON(OpenAPI_locality_description_item_t *locality_description_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (locality_description_item == NULL) {
        ogs_error("OpenAPI_locality_description_item_convertToJSON() failed [LocalityDescriptionItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (locality_description_item->locality_type == OpenAPI_locality_type_NULL) {
        ogs_error("OpenAPI_locality_description_item_convertToJSON() failed [locality_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "localityType", OpenAPI_locality_type_ToString(locality_description_item->locality_type)) == NULL) {
        ogs_error("OpenAPI_locality_description_item_convertToJSON() failed [locality_type]");
        goto end;
    }

    if (!locality_description_item->locality_value) {
        ogs_error("OpenAPI_locality_description_item_convertToJSON() failed [locality_value]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "localityValue", locality_description_item->locality_value) == NULL) {
        ogs_error("OpenAPI_locality_description_item_convertToJSON() failed [locality_value]");
        goto end;
    }

end:
    return item;
}

OpenAPI_locality_description_item_t *OpenAPI_locality_description_item_parseFromJSON(cJSON *locality_description_itemJSON)
{
    OpenAPI_locality_description_item_t *locality_description_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *locality_type = NULL;
    OpenAPI_locality_type_e locality_typeVariable = 0;
    cJSON *locality_value = NULL;
    locality_type = cJSON_GetObjectItemCaseSensitive(locality_description_itemJSON, "localityType");
    if (!locality_type) {
        ogs_error("OpenAPI_locality_description_item_parseFromJSON() failed [locality_type]");
        goto end;
    }
    if (!cJSON_IsString(locality_type)) {
        ogs_error("OpenAPI_locality_description_item_parseFromJSON() failed [locality_type]");
        goto end;
    }
    locality_typeVariable = OpenAPI_locality_type_FromString(locality_type->valuestring);

    locality_value = cJSON_GetObjectItemCaseSensitive(locality_description_itemJSON, "localityValue");
    if (!locality_value) {
        ogs_error("OpenAPI_locality_description_item_parseFromJSON() failed [locality_value]");
        goto end;
    }
    if (!cJSON_IsString(locality_value)) {
        ogs_error("OpenAPI_locality_description_item_parseFromJSON() failed [locality_value]");
        goto end;
    }

    locality_description_item_local_var = OpenAPI_locality_description_item_create (
        locality_typeVariable,
        ogs_strdup(locality_value->valuestring)
    );

    return locality_description_item_local_var;
end:
    return NULL;
}

OpenAPI_locality_description_item_t *OpenAPI_locality_description_item_copy(OpenAPI_locality_description_item_t *dst, OpenAPI_locality_description_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_locality_description_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_locality_description_item_convertToJSON() failed");
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

    OpenAPI_locality_description_item_free(dst);
    dst = OpenAPI_locality_description_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

