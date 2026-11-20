
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_address_info.h"

OpenAPI_sm_address_info_t *OpenAPI_sm_address_info_create(
    OpenAPI_sm_address_type_e s_maddress_type,
    char *s_maddress_data,
    OpenAPI_sm_address_domain_t *s_maddress_domain
)
{
    OpenAPI_sm_address_info_t *sm_address_info_local_var = ogs_malloc(sizeof(OpenAPI_sm_address_info_t));
    ogs_assert(sm_address_info_local_var);

    sm_address_info_local_var->s_maddress_type = s_maddress_type;
    sm_address_info_local_var->s_maddress_data = s_maddress_data;
    sm_address_info_local_var->s_maddress_domain = s_maddress_domain;

    return sm_address_info_local_var;
}

void OpenAPI_sm_address_info_free(OpenAPI_sm_address_info_t *sm_address_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_address_info) {
        return;
    }
    if (sm_address_info->s_maddress_data) {
        ogs_free(sm_address_info->s_maddress_data);
        sm_address_info->s_maddress_data = NULL;
    }
    if (sm_address_info->s_maddress_domain) {
        OpenAPI_sm_address_domain_free(sm_address_info->s_maddress_domain);
        sm_address_info->s_maddress_domain = NULL;
    }
    ogs_free(sm_address_info);
}

cJSON *OpenAPI_sm_address_info_convertToJSON(OpenAPI_sm_address_info_t *sm_address_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_address_info == NULL) {
        ogs_error("OpenAPI_sm_address_info_convertToJSON() failed [SMAddressInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_address_info->s_maddress_type != OpenAPI_sm_address_type_NULL) {
    if (cJSON_AddStringToObject(item, "sMaddressType", OpenAPI_sm_address_type_ToString(sm_address_info->s_maddress_type)) == NULL) {
        ogs_error("OpenAPI_sm_address_info_convertToJSON() failed [s_maddress_type]");
        goto end;
    }
    }

    if (sm_address_info->s_maddress_data) {
    if (cJSON_AddStringToObject(item, "sMaddressData", sm_address_info->s_maddress_data) == NULL) {
        ogs_error("OpenAPI_sm_address_info_convertToJSON() failed [s_maddress_data]");
        goto end;
    }
    }

    if (sm_address_info->s_maddress_domain) {
    cJSON *s_maddress_domain_local_JSON = OpenAPI_sm_address_domain_convertToJSON(sm_address_info->s_maddress_domain);
    if (s_maddress_domain_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_address_info_convertToJSON() failed [s_maddress_domain]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sMaddressDomain", s_maddress_domain_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_address_info_convertToJSON() failed [s_maddress_domain]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_address_info_t *OpenAPI_sm_address_info_parseFromJSON(cJSON *sm_address_infoJSON)
{
    OpenAPI_sm_address_info_t *sm_address_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_maddress_type = NULL;
    OpenAPI_sm_address_type_e s_maddress_typeVariable = 0;
    cJSON *s_maddress_data = NULL;
    cJSON *s_maddress_domain = NULL;
    OpenAPI_sm_address_domain_t *s_maddress_domain_local_nonprim = NULL;
    s_maddress_type = cJSON_GetObjectItemCaseSensitive(sm_address_infoJSON, "sMaddressType");
    if (s_maddress_type) {
    if (!cJSON_IsString(s_maddress_type)) {
        ogs_error("OpenAPI_sm_address_info_parseFromJSON() failed [s_maddress_type]");
        goto end;
    }
    s_maddress_typeVariable = OpenAPI_sm_address_type_FromString(s_maddress_type->valuestring);
    }

    s_maddress_data = cJSON_GetObjectItemCaseSensitive(sm_address_infoJSON, "sMaddressData");
    if (s_maddress_data) {
    if (!cJSON_IsString(s_maddress_data) && !cJSON_IsNull(s_maddress_data)) {
        ogs_error("OpenAPI_sm_address_info_parseFromJSON() failed [s_maddress_data]");
        goto end;
    }
    }

    s_maddress_domain = cJSON_GetObjectItemCaseSensitive(sm_address_infoJSON, "sMaddressDomain");
    if (s_maddress_domain) {
    s_maddress_domain_local_nonprim = OpenAPI_sm_address_domain_parseFromJSON(s_maddress_domain);
    if (!s_maddress_domain_local_nonprim) {
        ogs_error("OpenAPI_sm_address_domain_parseFromJSON failed [s_maddress_domain]");
        goto end;
    }
    }

    sm_address_info_local_var = OpenAPI_sm_address_info_create (
        s_maddress_type ? s_maddress_typeVariable : 0,
        s_maddress_data && !cJSON_IsNull(s_maddress_data) ? ogs_strdup(s_maddress_data->valuestring) : NULL,
        s_maddress_domain ? s_maddress_domain_local_nonprim : NULL
    );

    return sm_address_info_local_var;
end:
    if (s_maddress_domain_local_nonprim) {
        OpenAPI_sm_address_domain_free(s_maddress_domain_local_nonprim);
        s_maddress_domain_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_address_info_t *OpenAPI_sm_address_info_copy(OpenAPI_sm_address_info_t *dst, OpenAPI_sm_address_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_address_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_address_info_convertToJSON() failed");
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

    OpenAPI_sm_address_info_free(dst);
    dst = OpenAPI_sm_address_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

