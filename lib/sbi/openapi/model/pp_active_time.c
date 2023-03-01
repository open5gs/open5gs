
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_active_time.h"

OpenAPI_pp_active_time_t *OpenAPI_pp_active_time_create(
    int active_time,
    char *af_instance_id,
    int reference_id,
    char *validity_time,
    char *mtc_provider_information
)
{
    OpenAPI_pp_active_time_t *pp_active_time_local_var = ogs_malloc(sizeof(OpenAPI_pp_active_time_t));
    ogs_assert(pp_active_time_local_var);

    pp_active_time_local_var->active_time = active_time;
    pp_active_time_local_var->af_instance_id = af_instance_id;
    pp_active_time_local_var->reference_id = reference_id;
    pp_active_time_local_var->validity_time = validity_time;
    pp_active_time_local_var->mtc_provider_information = mtc_provider_information;

    return pp_active_time_local_var;
}

void OpenAPI_pp_active_time_free(OpenAPI_pp_active_time_t *pp_active_time)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pp_active_time) {
        return;
    }
    if (pp_active_time->af_instance_id) {
        ogs_free(pp_active_time->af_instance_id);
        pp_active_time->af_instance_id = NULL;
    }
    if (pp_active_time->validity_time) {
        ogs_free(pp_active_time->validity_time);
        pp_active_time->validity_time = NULL;
    }
    if (pp_active_time->mtc_provider_information) {
        ogs_free(pp_active_time->mtc_provider_information);
        pp_active_time->mtc_provider_information = NULL;
    }
    ogs_free(pp_active_time);
}

cJSON *OpenAPI_pp_active_time_convertToJSON(OpenAPI_pp_active_time_t *pp_active_time)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pp_active_time == NULL) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed [PpActiveTime]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "activeTime", pp_active_time->active_time) == NULL) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed [active_time]");
        goto end;
    }

    if (!pp_active_time->af_instance_id) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed [af_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", pp_active_time->af_instance_id) == NULL) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "referenceId", pp_active_time->reference_id) == NULL) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (pp_active_time->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", pp_active_time->validity_time) == NULL) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (pp_active_time->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", pp_active_time->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pp_active_time_t *OpenAPI_pp_active_time_parseFromJSON(cJSON *pp_active_timeJSON)
{
    OpenAPI_pp_active_time_t *pp_active_time_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *active_time = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *validity_time = NULL;
    cJSON *mtc_provider_information = NULL;
    active_time = cJSON_GetObjectItemCaseSensitive(pp_active_timeJSON, "activeTime");
    if (!active_time) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [active_time]");
        goto end;
    }
    if (!cJSON_IsNumber(active_time)) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [active_time]");
        goto end;
    }

    af_instance_id = cJSON_GetObjectItemCaseSensitive(pp_active_timeJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(pp_active_timeJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [reference_id]");
        goto end;
    }
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [reference_id]");
        goto end;
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(pp_active_timeJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(pp_active_timeJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    pp_active_time_local_var = OpenAPI_pp_active_time_create (
        
        active_time->valuedouble,
        ogs_strdup(af_instance_id->valuestring),
        
        reference_id->valuedouble,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL
    );

    return pp_active_time_local_var;
end:
    return NULL;
}

OpenAPI_pp_active_time_t *OpenAPI_pp_active_time_copy(OpenAPI_pp_active_time_t *dst, OpenAPI_pp_active_time_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_active_time_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_active_time_convertToJSON() failed");
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

    OpenAPI_pp_active_time_free(dst);
    dst = OpenAPI_pp_active_time_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

