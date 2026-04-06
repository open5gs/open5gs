
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_replacement_mapping.h"

OpenAPI_slice_replacement_mapping_t *OpenAPI_slice_replacement_mapping_create(
    OpenAPI_snssai_t *replaced_snssai,
    OpenAPI_snssai_t *alt_snssai
)
{
    OpenAPI_slice_replacement_mapping_t *slice_replacement_mapping_local_var = ogs_malloc(sizeof(OpenAPI_slice_replacement_mapping_t));
    ogs_assert(slice_replacement_mapping_local_var);

    slice_replacement_mapping_local_var->replaced_snssai = replaced_snssai;
    slice_replacement_mapping_local_var->alt_snssai = alt_snssai;

    return slice_replacement_mapping_local_var;
}

void OpenAPI_slice_replacement_mapping_free(OpenAPI_slice_replacement_mapping_t *slice_replacement_mapping)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_replacement_mapping) {
        return;
    }
    if (slice_replacement_mapping->replaced_snssai) {
        OpenAPI_snssai_free(slice_replacement_mapping->replaced_snssai);
        slice_replacement_mapping->replaced_snssai = NULL;
    }
    if (slice_replacement_mapping->alt_snssai) {
        OpenAPI_snssai_free(slice_replacement_mapping->alt_snssai);
        slice_replacement_mapping->alt_snssai = NULL;
    }
    ogs_free(slice_replacement_mapping);
}

cJSON *OpenAPI_slice_replacement_mapping_convertToJSON(OpenAPI_slice_replacement_mapping_t *slice_replacement_mapping)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_replacement_mapping == NULL) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed [SliceReplacementMapping]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slice_replacement_mapping->replaced_snssai) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed [replaced_snssai]");
        return NULL;
    }
    cJSON *replaced_snssai_local_JSON = OpenAPI_snssai_convertToJSON(slice_replacement_mapping->replaced_snssai);
    if (replaced_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed [replaced_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "replacedSnssai", replaced_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed [replaced_snssai]");
        goto end;
    }

    if (!slice_replacement_mapping->alt_snssai) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed [alt_snssai]");
        return NULL;
    }
    cJSON *alt_snssai_local_JSON = OpenAPI_snssai_convertToJSON(slice_replacement_mapping->alt_snssai);
    if (alt_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed [alt_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "altSnssai", alt_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed [alt_snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_slice_replacement_mapping_t *OpenAPI_slice_replacement_mapping_parseFromJSON(cJSON *slice_replacement_mappingJSON)
{
    OpenAPI_slice_replacement_mapping_t *slice_replacement_mapping_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *replaced_snssai = NULL;
    OpenAPI_snssai_t *replaced_snssai_local_nonprim = NULL;
    cJSON *alt_snssai = NULL;
    OpenAPI_snssai_t *alt_snssai_local_nonprim = NULL;
    replaced_snssai = cJSON_GetObjectItemCaseSensitive(slice_replacement_mappingJSON, "replacedSnssai");
    if (!replaced_snssai) {
        ogs_error("OpenAPI_slice_replacement_mapping_parseFromJSON() failed [replaced_snssai]");
        goto end;
    }
    replaced_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(replaced_snssai);
    if (!replaced_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [replaced_snssai]");
        goto end;
    }

    alt_snssai = cJSON_GetObjectItemCaseSensitive(slice_replacement_mappingJSON, "altSnssai");
    if (!alt_snssai) {
        ogs_error("OpenAPI_slice_replacement_mapping_parseFromJSON() failed [alt_snssai]");
        goto end;
    }
    alt_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(alt_snssai);
    if (!alt_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [alt_snssai]");
        goto end;
    }

    slice_replacement_mapping_local_var = OpenAPI_slice_replacement_mapping_create (
        replaced_snssai_local_nonprim,
        alt_snssai_local_nonprim
    );

    return slice_replacement_mapping_local_var;
end:
    if (replaced_snssai_local_nonprim) {
        OpenAPI_snssai_free(replaced_snssai_local_nonprim);
        replaced_snssai_local_nonprim = NULL;
    }
    if (alt_snssai_local_nonprim) {
        OpenAPI_snssai_free(alt_snssai_local_nonprim);
        alt_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_slice_replacement_mapping_t *OpenAPI_slice_replacement_mapping_copy(OpenAPI_slice_replacement_mapping_t *dst, OpenAPI_slice_replacement_mapping_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_replacement_mapping_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_replacement_mapping_convertToJSON() failed");
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

    OpenAPI_slice_replacement_mapping_free(dst);
    dst = OpenAPI_slice_replacement_mapping_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

