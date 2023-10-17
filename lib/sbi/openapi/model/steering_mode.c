
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_mode.h"

OpenAPI_steering_mode_t *OpenAPI_steering_mode_create(
    OpenAPI_steer_mode_value_e steer_mode_value,
    OpenAPI_access_type_e active,
    OpenAPI_access_type_rm_t *standby,
    bool is__3g_load,
    int _3g_load,
    OpenAPI_access_type_e prio_acc,
    bool is_thres_value_null,
    OpenAPI_threshold_value_t *thres_value,
    OpenAPI_steer_mode_indicator_e steer_mode_ind
)
{
    OpenAPI_steering_mode_t *steering_mode_local_var = ogs_malloc(sizeof(OpenAPI_steering_mode_t));
    ogs_assert(steering_mode_local_var);

    steering_mode_local_var->steer_mode_value = steer_mode_value;
    steering_mode_local_var->active = active;
    steering_mode_local_var->standby = standby;
    steering_mode_local_var->is__3g_load = is__3g_load;
    steering_mode_local_var->_3g_load = _3g_load;
    steering_mode_local_var->prio_acc = prio_acc;
    steering_mode_local_var->is_thres_value_null = is_thres_value_null;
    steering_mode_local_var->thres_value = thres_value;
    steering_mode_local_var->steer_mode_ind = steer_mode_ind;

    return steering_mode_local_var;
}

void OpenAPI_steering_mode_free(OpenAPI_steering_mode_t *steering_mode)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == steering_mode) {
        return;
    }
    if (steering_mode->standby) {
        OpenAPI_access_type_rm_free(steering_mode->standby);
        steering_mode->standby = NULL;
    }
    if (steering_mode->thres_value) {
        OpenAPI_threshold_value_free(steering_mode->thres_value);
        steering_mode->thres_value = NULL;
    }
    ogs_free(steering_mode);
}

cJSON *OpenAPI_steering_mode_convertToJSON(OpenAPI_steering_mode_t *steering_mode)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (steering_mode == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [SteeringMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (steering_mode->steer_mode_value == OpenAPI_steer_mode_value_NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [steer_mode_value]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "steerModeValue", OpenAPI_steer_mode_value_ToString(steering_mode->steer_mode_value)) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [steer_mode_value]");
        goto end;
    }

    if (steering_mode->active != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "active", OpenAPI_access_type_ToString(steering_mode->active)) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [active]");
        goto end;
    }
    }

    if (steering_mode->standby) {
    cJSON *standby_local_JSON = OpenAPI_access_type_rm_convertToJSON(steering_mode->standby);
    if (standby_local_JSON == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [standby]");
        goto end;
    }
    cJSON_AddItemToObject(item, "standby", standby_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [standby]");
        goto end;
    }
    }

    if (steering_mode->is__3g_load) {
    if (cJSON_AddNumberToObject(item, "3gLoad", steering_mode->_3g_load) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [_3g_load]");
        goto end;
    }
    }

    if (steering_mode->prio_acc != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "prioAcc", OpenAPI_access_type_ToString(steering_mode->prio_acc)) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [prio_acc]");
        goto end;
    }
    }

    if (steering_mode->thres_value) {
    cJSON *thres_value_local_JSON = OpenAPI_threshold_value_convertToJSON(steering_mode->thres_value);
    if (thres_value_local_JSON == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [thres_value]");
        goto end;
    }
    cJSON_AddItemToObject(item, "thresValue", thres_value_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [thres_value]");
        goto end;
    }
    } else if (steering_mode->is_thres_value_null) {
        if (cJSON_AddNullToObject(item, "thresValue") == NULL) {
            ogs_error("OpenAPI_steering_mode_convertToJSON() failed [thres_value]");
            goto end;
        }
    }

    if (steering_mode->steer_mode_ind != OpenAPI_steer_mode_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "steerModeInd", OpenAPI_steer_mode_indicator_ToString(steering_mode->steer_mode_ind)) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [steer_mode_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_steering_mode_t *OpenAPI_steering_mode_parseFromJSON(cJSON *steering_modeJSON)
{
    OpenAPI_steering_mode_t *steering_mode_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *steer_mode_value = NULL;
    OpenAPI_steer_mode_value_e steer_mode_valueVariable = 0;
    cJSON *active = NULL;
    OpenAPI_access_type_e activeVariable = 0;
    cJSON *standby = NULL;
    OpenAPI_access_type_rm_t *standby_local_nonprim = NULL;
    cJSON *_3g_load = NULL;
    cJSON *prio_acc = NULL;
    OpenAPI_access_type_e prio_accVariable = 0;
    cJSON *thres_value = NULL;
    OpenAPI_threshold_value_t *thres_value_local_nonprim = NULL;
    cJSON *steer_mode_ind = NULL;
    OpenAPI_steer_mode_indicator_e steer_mode_indVariable = 0;
    steer_mode_value = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "steerModeValue");
    if (!steer_mode_value) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [steer_mode_value]");
        goto end;
    }
    if (!cJSON_IsString(steer_mode_value)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [steer_mode_value]");
        goto end;
    }
    steer_mode_valueVariable = OpenAPI_steer_mode_value_FromString(steer_mode_value->valuestring);

    active = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "active");
    if (active) {
    if (!cJSON_IsString(active)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [active]");
        goto end;
    }
    activeVariable = OpenAPI_access_type_FromString(active->valuestring);
    }

    standby = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "standby");
    if (standby) {
    standby_local_nonprim = OpenAPI_access_type_rm_parseFromJSON(standby);
    if (!standby_local_nonprim) {
        ogs_error("OpenAPI_access_type_rm_parseFromJSON failed [standby]");
        goto end;
    }
    }

    _3g_load = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "3gLoad");
    if (_3g_load) {
    if (!cJSON_IsNumber(_3g_load)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [_3g_load]");
        goto end;
    }
    }

    prio_acc = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "prioAcc");
    if (prio_acc) {
    if (!cJSON_IsString(prio_acc)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [prio_acc]");
        goto end;
    }
    prio_accVariable = OpenAPI_access_type_FromString(prio_acc->valuestring);
    }

    thres_value = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "thresValue");
    if (thres_value) {
    if (!cJSON_IsNull(thres_value)) {
    thres_value_local_nonprim = OpenAPI_threshold_value_parseFromJSON(thres_value);
    if (!thres_value_local_nonprim) {
        ogs_error("OpenAPI_threshold_value_parseFromJSON failed [thres_value]");
        goto end;
    }
    }
    }

    steer_mode_ind = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "steerModeInd");
    if (steer_mode_ind) {
    if (!cJSON_IsString(steer_mode_ind)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [steer_mode_ind]");
        goto end;
    }
    steer_mode_indVariable = OpenAPI_steer_mode_indicator_FromString(steer_mode_ind->valuestring);
    }

    steering_mode_local_var = OpenAPI_steering_mode_create (
        steer_mode_valueVariable,
        active ? activeVariable : 0,
        standby ? standby_local_nonprim : NULL,
        _3g_load ? true : false,
        _3g_load ? _3g_load->valuedouble : 0,
        prio_acc ? prio_accVariable : 0,
        thres_value && cJSON_IsNull(thres_value) ? true : false,
        thres_value ? thres_value_local_nonprim : NULL,
        steer_mode_ind ? steer_mode_indVariable : 0
    );

    return steering_mode_local_var;
end:
    if (standby_local_nonprim) {
        OpenAPI_access_type_rm_free(standby_local_nonprim);
        standby_local_nonprim = NULL;
    }
    if (thres_value_local_nonprim) {
        OpenAPI_threshold_value_free(thres_value_local_nonprim);
        thres_value_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_steering_mode_t *OpenAPI_steering_mode_copy(OpenAPI_steering_mode_t *dst, OpenAPI_steering_mode_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steering_mode_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed");
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

    OpenAPI_steering_mode_free(dst);
    dst = OpenAPI_steering_mode_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

