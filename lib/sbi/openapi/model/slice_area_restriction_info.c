
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_area_restriction_info.h"

OpenAPI_slice_area_restriction_info_t *OpenAPI_slice_area_restriction_info_create(
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_presence_state_e presence
)
{
    OpenAPI_slice_area_restriction_info_t *slice_area_restriction_info_local_var = ogs_malloc(sizeof(OpenAPI_slice_area_restriction_info_t));
    ogs_assert(slice_area_restriction_info_local_var);

    slice_area_restriction_info_local_var->s_nssai = s_nssai;
    slice_area_restriction_info_local_var->presence = presence;

    return slice_area_restriction_info_local_var;
}

void OpenAPI_slice_area_restriction_info_free(OpenAPI_slice_area_restriction_info_t *slice_area_restriction_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_area_restriction_info) {
        return;
    }
    if (slice_area_restriction_info->s_nssai) {
        OpenAPI_snssai_free(slice_area_restriction_info->s_nssai);
        slice_area_restriction_info->s_nssai = NULL;
    }
    ogs_free(slice_area_restriction_info);
}

cJSON *OpenAPI_slice_area_restriction_info_convertToJSON(OpenAPI_slice_area_restriction_info_t *slice_area_restriction_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_area_restriction_info == NULL) {
        ogs_error("OpenAPI_slice_area_restriction_info_convertToJSON() failed [SliceAreaRestrictionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slice_area_restriction_info->s_nssai) {
        ogs_error("OpenAPI_slice_area_restriction_info_convertToJSON() failed [s_nssai]");
        return NULL;
    }
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(slice_area_restriction_info->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_area_restriction_info_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_area_restriction_info_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (slice_area_restriction_info->presence != OpenAPI_presence_state_NULL) {
    if (cJSON_AddStringToObject(item, "presence", OpenAPI_presence_state_ToString(slice_area_restriction_info->presence)) == NULL) {
        ogs_error("OpenAPI_slice_area_restriction_info_convertToJSON() failed [presence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_slice_area_restriction_info_t *OpenAPI_slice_area_restriction_info_parseFromJSON(cJSON *slice_area_restriction_infoJSON)
{
    OpenAPI_slice_area_restriction_info_t *slice_area_restriction_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *presence = NULL;
    OpenAPI_presence_state_e presenceVariable = 0;
    s_nssai = cJSON_GetObjectItemCaseSensitive(slice_area_restriction_infoJSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_slice_area_restriction_info_parseFromJSON() failed [s_nssai]");
        goto end;
    }
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }

    presence = cJSON_GetObjectItemCaseSensitive(slice_area_restriction_infoJSON, "presence");
    if (presence) {
    if (!cJSON_IsString(presence)) {
        ogs_error("OpenAPI_slice_area_restriction_info_parseFromJSON() failed [presence]");
        goto end;
    }
    presenceVariable = OpenAPI_presence_state_FromString(presence->valuestring);
    }

    slice_area_restriction_info_local_var = OpenAPI_slice_area_restriction_info_create (
        s_nssai_local_nonprim,
        presence ? presenceVariable : 0
    );

    return slice_area_restriction_info_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_slice_area_restriction_info_t *OpenAPI_slice_area_restriction_info_copy(OpenAPI_slice_area_restriction_info_t *dst, OpenAPI_slice_area_restriction_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_area_restriction_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_area_restriction_info_convertToJSON() failed");
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

    OpenAPI_slice_area_restriction_info_free(dst);
    dst = OpenAPI_slice_area_restriction_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

