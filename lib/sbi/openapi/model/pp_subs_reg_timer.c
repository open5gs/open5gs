
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_subs_reg_timer.h"

OpenAPI_pp_subs_reg_timer_t *OpenAPI_pp_subs_reg_timer_create(
    int subs_reg_timer,
    char *af_instance_id,
    int reference_id,
    char *validity_time
    )
{
    OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer_local_var = OpenAPI_malloc(sizeof(OpenAPI_pp_subs_reg_timer_t));
    if (!pp_subs_reg_timer_local_var) {
        return NULL;
    }
    pp_subs_reg_timer_local_var->subs_reg_timer = subs_reg_timer;
    pp_subs_reg_timer_local_var->af_instance_id = af_instance_id;
    pp_subs_reg_timer_local_var->reference_id = reference_id;
    pp_subs_reg_timer_local_var->validity_time = validity_time;

    return pp_subs_reg_timer_local_var;
}

void OpenAPI_pp_subs_reg_timer_free(OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer)
{
    if (NULL == pp_subs_reg_timer) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pp_subs_reg_timer->af_instance_id);
    ogs_free(pp_subs_reg_timer->validity_time);
    ogs_free(pp_subs_reg_timer);
}

cJSON *OpenAPI_pp_subs_reg_timer_convertToJSON(OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer)
{
    cJSON *item = NULL;

    if (pp_subs_reg_timer == NULL) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [PpSubsRegTimer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pp_subs_reg_timer->subs_reg_timer) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [subs_reg_timer]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "subsRegTimer", pp_subs_reg_timer->subs_reg_timer) == NULL) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [subs_reg_timer]");
        goto end;
    }

    if (!pp_subs_reg_timer->af_instance_id) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [af_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", pp_subs_reg_timer->af_instance_id) == NULL) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (!pp_subs_reg_timer->reference_id) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [reference_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "referenceId", pp_subs_reg_timer->reference_id) == NULL) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (pp_subs_reg_timer->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", pp_subs_reg_timer->validity_time) == NULL) {
            ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pp_subs_reg_timer_t *OpenAPI_pp_subs_reg_timer_parseFromJSON(cJSON *pp_subs_reg_timerJSON)
{
    OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer_local_var = NULL;
    cJSON *subs_reg_timer = cJSON_GetObjectItemCaseSensitive(pp_subs_reg_timerJSON, "subsRegTimer");
    if (!subs_reg_timer) {
        ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON() failed [subs_reg_timer]");
        goto end;
    }


    if (!cJSON_IsNumber(subs_reg_timer)) {
        ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON() failed [subs_reg_timer]");
        goto end;
    }

    cJSON *af_instance_id = cJSON_GetObjectItemCaseSensitive(pp_subs_reg_timerJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON() failed [af_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    cJSON *reference_id = cJSON_GetObjectItemCaseSensitive(pp_subs_reg_timerJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON() failed [reference_id]");
        goto end;
    }


    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON() failed [reference_id]");
        goto end;
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(pp_subs_reg_timerJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    pp_subs_reg_timer_local_var = OpenAPI_pp_subs_reg_timer_create (
        subs_reg_timer->valuedouble,
        ogs_strdup(af_instance_id->valuestring),
        reference_id->valuedouble,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
        );

    return pp_subs_reg_timer_local_var;
end:
    return NULL;
}

OpenAPI_pp_subs_reg_timer_t *OpenAPI_pp_subs_reg_timer_copy(OpenAPI_pp_subs_reg_timer_t *dst, OpenAPI_pp_subs_reg_timer_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_subs_reg_timer_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_subs_reg_timer_convertToJSON() failed");
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

    OpenAPI_pp_subs_reg_timer_free(dst);
    dst = OpenAPI_pp_subs_reg_timer_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

