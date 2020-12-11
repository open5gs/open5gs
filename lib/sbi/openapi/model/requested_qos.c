
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_qos.h"

OpenAPI_requested_qos_t *OpenAPI_requested_qos_create(
    int _5qi,
    char *gbr_ul,
    char *gbr_dl
    )
{
    OpenAPI_requested_qos_t *requested_qos_local_var = OpenAPI_malloc(sizeof(OpenAPI_requested_qos_t));
    if (!requested_qos_local_var) {
        return NULL;
    }
    requested_qos_local_var->_5qi = _5qi;
    requested_qos_local_var->gbr_ul = gbr_ul;
    requested_qos_local_var->gbr_dl = gbr_dl;

    return requested_qos_local_var;
}

void OpenAPI_requested_qos_free(OpenAPI_requested_qos_t *requested_qos)
{
    if (NULL == requested_qos) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(requested_qos->gbr_ul);
    ogs_free(requested_qos->gbr_dl);
    ogs_free(requested_qos);
}

cJSON *OpenAPI_requested_qos_convertToJSON(OpenAPI_requested_qos_t *requested_qos)
{
    cJSON *item = NULL;

    if (requested_qos == NULL) {
        ogs_error("OpenAPI_requested_qos_convertToJSON() failed [RequestedQos]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!requested_qos->_5qi) {
        ogs_error("OpenAPI_requested_qos_convertToJSON() failed [_5qi]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "5qi", requested_qos->_5qi) == NULL) {
        ogs_error("OpenAPI_requested_qos_convertToJSON() failed [_5qi]");
        goto end;
    }

    if (requested_qos->gbr_ul) {
        if (cJSON_AddStringToObject(item, "gbrUl", requested_qos->gbr_ul) == NULL) {
            ogs_error("OpenAPI_requested_qos_convertToJSON() failed [gbr_ul]");
            goto end;
        }
    }

    if (requested_qos->gbr_dl) {
        if (cJSON_AddStringToObject(item, "gbrDl", requested_qos->gbr_dl) == NULL) {
            ogs_error("OpenAPI_requested_qos_convertToJSON() failed [gbr_dl]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_requested_qos_t *OpenAPI_requested_qos_parseFromJSON(cJSON *requested_qosJSON)
{
    OpenAPI_requested_qos_t *requested_qos_local_var = NULL;
    cJSON *_5qi = cJSON_GetObjectItemCaseSensitive(requested_qosJSON, "5qi");
    if (!_5qi) {
        ogs_error("OpenAPI_requested_qos_parseFromJSON() failed [_5qi]");
        goto end;
    }


    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_requested_qos_parseFromJSON() failed [_5qi]");
        goto end;
    }

    cJSON *gbr_ul = cJSON_GetObjectItemCaseSensitive(requested_qosJSON, "gbrUl");

    if (gbr_ul) {
        if (!cJSON_IsString(gbr_ul)) {
            ogs_error("OpenAPI_requested_qos_parseFromJSON() failed [gbr_ul]");
            goto end;
        }
    }

    cJSON *gbr_dl = cJSON_GetObjectItemCaseSensitive(requested_qosJSON, "gbrDl");

    if (gbr_dl) {
        if (!cJSON_IsString(gbr_dl)) {
            ogs_error("OpenAPI_requested_qos_parseFromJSON() failed [gbr_dl]");
            goto end;
        }
    }

    requested_qos_local_var = OpenAPI_requested_qos_create (
        _5qi->valuedouble,
        gbr_ul ? ogs_strdup(gbr_ul->valuestring) : NULL,
        gbr_dl ? ogs_strdup(gbr_dl->valuestring) : NULL
        );

    return requested_qos_local_var;
end:
    return NULL;
}

OpenAPI_requested_qos_t *OpenAPI_requested_qos_copy(OpenAPI_requested_qos_t *dst, OpenAPI_requested_qos_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_requested_qos_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_requested_qos_convertToJSON() failed");
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

    OpenAPI_requested_qos_free(dst);
    dst = OpenAPI_requested_qos_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

