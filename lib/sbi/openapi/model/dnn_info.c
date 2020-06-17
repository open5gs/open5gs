
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_info.h"

OpenAPI_dnn_info_t *OpenAPI_dnn_info_create(
    char *dnn,
    int default_dnn_indicator,
    int lbo_roaming_allowed,
    int iwk_eps_ind,
    int dnn_barred
    )
{
    OpenAPI_dnn_info_t *dnn_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_dnn_info_t));
    if (!dnn_info_local_var) {
        return NULL;
    }
    dnn_info_local_var->dnn = dnn;
    dnn_info_local_var->default_dnn_indicator = default_dnn_indicator;
    dnn_info_local_var->lbo_roaming_allowed = lbo_roaming_allowed;
    dnn_info_local_var->iwk_eps_ind = iwk_eps_ind;
    dnn_info_local_var->dnn_barred = dnn_barred;

    return dnn_info_local_var;
}

void OpenAPI_dnn_info_free(OpenAPI_dnn_info_t *dnn_info)
{
    if (NULL == dnn_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dnn_info->dnn);
    ogs_free(dnn_info);
}

cJSON *OpenAPI_dnn_info_convertToJSON(OpenAPI_dnn_info_t *dnn_info)
{
    cJSON *item = NULL;

    if (dnn_info == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [DnnInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_info->dnn) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_info->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_info->default_dnn_indicator) {
        if (cJSON_AddBoolToObject(item, "defaultDnnIndicator", dnn_info->default_dnn_indicator) == NULL) {
            ogs_error("OpenAPI_dnn_info_convertToJSON() failed [default_dnn_indicator]");
            goto end;
        }
    }

    if (dnn_info->lbo_roaming_allowed) {
        if (cJSON_AddBoolToObject(item, "lboRoamingAllowed", dnn_info->lbo_roaming_allowed) == NULL) {
            ogs_error("OpenAPI_dnn_info_convertToJSON() failed [lbo_roaming_allowed]");
            goto end;
        }
    }

    if (dnn_info->iwk_eps_ind) {
        if (cJSON_AddBoolToObject(item, "iwkEpsInd", dnn_info->iwk_eps_ind) == NULL) {
            ogs_error("OpenAPI_dnn_info_convertToJSON() failed [iwk_eps_ind]");
            goto end;
        }
    }

    if (dnn_info->dnn_barred) {
        if (cJSON_AddBoolToObject(item, "dnnBarred", dnn_info->dnn_barred) == NULL) {
            ogs_error("OpenAPI_dnn_info_convertToJSON() failed [dnn_barred]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_dnn_info_t *OpenAPI_dnn_info_parseFromJSON(cJSON *dnn_infoJSON)
{
    OpenAPI_dnn_info_t *dnn_info_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *default_dnn_indicator = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "defaultDnnIndicator");

    if (default_dnn_indicator) {
        if (!cJSON_IsBool(default_dnn_indicator)) {
            ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [default_dnn_indicator]");
            goto end;
        }
    }

    cJSON *lbo_roaming_allowed = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "lboRoamingAllowed");

    if (lbo_roaming_allowed) {
        if (!cJSON_IsBool(lbo_roaming_allowed)) {
            ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [lbo_roaming_allowed]");
            goto end;
        }
    }

    cJSON *iwk_eps_ind = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "iwkEpsInd");

    if (iwk_eps_ind) {
        if (!cJSON_IsBool(iwk_eps_ind)) {
            ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [iwk_eps_ind]");
            goto end;
        }
    }

    cJSON *dnn_barred = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "dnnBarred");

    if (dnn_barred) {
        if (!cJSON_IsBool(dnn_barred)) {
            ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [dnn_barred]");
            goto end;
        }
    }

    dnn_info_local_var = OpenAPI_dnn_info_create (
        ogs_strdup(dnn->valuestring),
        default_dnn_indicator ? default_dnn_indicator->valueint : 0,
        lbo_roaming_allowed ? lbo_roaming_allowed->valueint : 0,
        iwk_eps_ind ? iwk_eps_ind->valueint : 0,
        dnn_barred ? dnn_barred->valueint : 0
        );

    return dnn_info_local_var;
end:
    return NULL;
}

OpenAPI_dnn_info_t *OpenAPI_dnn_info_copy(OpenAPI_dnn_info_t *dst, OpenAPI_dnn_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed");
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

    OpenAPI_dnn_info_free(dst);
    dst = OpenAPI_dnn_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

