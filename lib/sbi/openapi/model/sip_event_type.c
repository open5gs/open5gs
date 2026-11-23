
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sip_event_type.h"

OpenAPI_sip_event_type_t *OpenAPI_sip_event_type_create(
    char *s_ip_method,
    char *event_header,
    bool is_expires_header,
    int expires_header
)
{
    OpenAPI_sip_event_type_t *sip_event_type_local_var = ogs_malloc(sizeof(OpenAPI_sip_event_type_t));
    ogs_assert(sip_event_type_local_var);

    sip_event_type_local_var->s_ip_method = s_ip_method;
    sip_event_type_local_var->event_header = event_header;
    sip_event_type_local_var->is_expires_header = is_expires_header;
    sip_event_type_local_var->expires_header = expires_header;

    return sip_event_type_local_var;
}

void OpenAPI_sip_event_type_free(OpenAPI_sip_event_type_t *sip_event_type)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sip_event_type) {
        return;
    }
    if (sip_event_type->s_ip_method) {
        ogs_free(sip_event_type->s_ip_method);
        sip_event_type->s_ip_method = NULL;
    }
    if (sip_event_type->event_header) {
        ogs_free(sip_event_type->event_header);
        sip_event_type->event_header = NULL;
    }
    ogs_free(sip_event_type);
}

cJSON *OpenAPI_sip_event_type_convertToJSON(OpenAPI_sip_event_type_t *sip_event_type)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sip_event_type == NULL) {
        ogs_error("OpenAPI_sip_event_type_convertToJSON() failed [SIPEventType]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sip_event_type->s_ip_method) {
    if (cJSON_AddStringToObject(item, "sIPMethod", sip_event_type->s_ip_method) == NULL) {
        ogs_error("OpenAPI_sip_event_type_convertToJSON() failed [s_ip_method]");
        goto end;
    }
    }

    if (sip_event_type->event_header) {
    if (cJSON_AddStringToObject(item, "eventHeader", sip_event_type->event_header) == NULL) {
        ogs_error("OpenAPI_sip_event_type_convertToJSON() failed [event_header]");
        goto end;
    }
    }

    if (sip_event_type->is_expires_header) {
    if (cJSON_AddNumberToObject(item, "expiresHeader", sip_event_type->expires_header) == NULL) {
        ogs_error("OpenAPI_sip_event_type_convertToJSON() failed [expires_header]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sip_event_type_t *OpenAPI_sip_event_type_parseFromJSON(cJSON *sip_event_typeJSON)
{
    OpenAPI_sip_event_type_t *sip_event_type_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_ip_method = NULL;
    cJSON *event_header = NULL;
    cJSON *expires_header = NULL;
    s_ip_method = cJSON_GetObjectItemCaseSensitive(sip_event_typeJSON, "sIPMethod");
    if (s_ip_method) {
    if (!cJSON_IsString(s_ip_method) && !cJSON_IsNull(s_ip_method)) {
        ogs_error("OpenAPI_sip_event_type_parseFromJSON() failed [s_ip_method]");
        goto end;
    }
    }

    event_header = cJSON_GetObjectItemCaseSensitive(sip_event_typeJSON, "eventHeader");
    if (event_header) {
    if (!cJSON_IsString(event_header) && !cJSON_IsNull(event_header)) {
        ogs_error("OpenAPI_sip_event_type_parseFromJSON() failed [event_header]");
        goto end;
    }
    }

    expires_header = cJSON_GetObjectItemCaseSensitive(sip_event_typeJSON, "expiresHeader");
    if (expires_header) {
    if (!cJSON_IsNumber(expires_header)) {
        ogs_error("OpenAPI_sip_event_type_parseFromJSON() failed [expires_header]");
        goto end;
    }
    }

    sip_event_type_local_var = OpenAPI_sip_event_type_create (
        s_ip_method && !cJSON_IsNull(s_ip_method) ? ogs_strdup(s_ip_method->valuestring) : NULL,
        event_header && !cJSON_IsNull(event_header) ? ogs_strdup(event_header->valuestring) : NULL,
        expires_header ? true : false,
        expires_header ? expires_header->valuedouble : 0
    );

    return sip_event_type_local_var;
end:
    return NULL;
}

OpenAPI_sip_event_type_t *OpenAPI_sip_event_type_copy(OpenAPI_sip_event_type_t *dst, OpenAPI_sip_event_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sip_event_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sip_event_type_convertToJSON() failed");
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

    OpenAPI_sip_event_type_free(dst);
    dst = OpenAPI_sip_event_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

