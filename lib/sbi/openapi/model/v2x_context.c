
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "v2x_context.h"

OpenAPI_v2x_context_t *OpenAPI_v2x_context_create(
    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth,
    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth,
    char *nr_ue_sidelink_ambr,
    char *lte_ue_sidelink_ambr,
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para
    )
{
    OpenAPI_v2x_context_t *v2x_context_local_var = OpenAPI_malloc(sizeof(OpenAPI_v2x_context_t));
    if (!v2x_context_local_var) {
        return NULL;
    }
    v2x_context_local_var->nr_v2x_services_auth = nr_v2x_services_auth;
    v2x_context_local_var->lte_v2x_services_auth = lte_v2x_services_auth;
    v2x_context_local_var->nr_ue_sidelink_ambr = nr_ue_sidelink_ambr;
    v2x_context_local_var->lte_ue_sidelink_ambr = lte_ue_sidelink_ambr;
    v2x_context_local_var->pc5_qo_s_para = pc5_qo_s_para;

    return v2x_context_local_var;
}

void OpenAPI_v2x_context_free(OpenAPI_v2x_context_t *v2x_context)
{
    if (NULL == v2x_context) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_nr_v2x_auth_free(v2x_context->nr_v2x_services_auth);
    OpenAPI_lte_v2x_auth_free(v2x_context->lte_v2x_services_auth);
    ogs_free(v2x_context->nr_ue_sidelink_ambr);
    ogs_free(v2x_context->lte_ue_sidelink_ambr);
    OpenAPI_pc5_qo_s_para_free(v2x_context->pc5_qo_s_para);
    ogs_free(v2x_context);
}

cJSON *OpenAPI_v2x_context_convertToJSON(OpenAPI_v2x_context_t *v2x_context)
{
    cJSON *item = NULL;

    if (v2x_context == NULL) {
        ogs_error("OpenAPI_v2x_context_convertToJSON() failed [V2xContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (v2x_context->nr_v2x_services_auth) {
        cJSON *nr_v2x_services_auth_local_JSON = OpenAPI_nr_v2x_auth_convertToJSON(v2x_context->nr_v2x_services_auth);
        if (nr_v2x_services_auth_local_JSON == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [nr_v2x_services_auth]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nrV2xServicesAuth", nr_v2x_services_auth_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [nr_v2x_services_auth]");
            goto end;
        }
    }

    if (v2x_context->lte_v2x_services_auth) {
        cJSON *lte_v2x_services_auth_local_JSON = OpenAPI_lte_v2x_auth_convertToJSON(v2x_context->lte_v2x_services_auth);
        if (lte_v2x_services_auth_local_JSON == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [lte_v2x_services_auth]");
            goto end;
        }
        cJSON_AddItemToObject(item, "lteV2xServicesAuth", lte_v2x_services_auth_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [lte_v2x_services_auth]");
            goto end;
        }
    }

    if (v2x_context->nr_ue_sidelink_ambr) {
        if (cJSON_AddStringToObject(item, "nrUeSidelinkAmbr", v2x_context->nr_ue_sidelink_ambr) == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [nr_ue_sidelink_ambr]");
            goto end;
        }
    }

    if (v2x_context->lte_ue_sidelink_ambr) {
        if (cJSON_AddStringToObject(item, "lteUeSidelinkAmbr", v2x_context->lte_ue_sidelink_ambr) == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [lte_ue_sidelink_ambr]");
            goto end;
        }
    }

    if (v2x_context->pc5_qo_s_para) {
        cJSON *pc5_qo_s_para_local_JSON = OpenAPI_pc5_qo_s_para_convertToJSON(v2x_context->pc5_qo_s_para);
        if (pc5_qo_s_para_local_JSON == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [pc5_qo_s_para]");
            goto end;
        }
        cJSON_AddItemToObject(item, "Pc5QoSPara", pc5_qo_s_para_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_v2x_context_convertToJSON() failed [pc5_qo_s_para]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_v2x_context_t *OpenAPI_v2x_context_parseFromJSON(cJSON *v2x_contextJSON)
{
    OpenAPI_v2x_context_t *v2x_context_local_var = NULL;
    cJSON *nr_v2x_services_auth = cJSON_GetObjectItemCaseSensitive(v2x_contextJSON, "nrV2xServicesAuth");

    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth_local_nonprim = NULL;
    if (nr_v2x_services_auth) {
        nr_v2x_services_auth_local_nonprim = OpenAPI_nr_v2x_auth_parseFromJSON(nr_v2x_services_auth);
    }

    cJSON *lte_v2x_services_auth = cJSON_GetObjectItemCaseSensitive(v2x_contextJSON, "lteV2xServicesAuth");

    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth_local_nonprim = NULL;
    if (lte_v2x_services_auth) {
        lte_v2x_services_auth_local_nonprim = OpenAPI_lte_v2x_auth_parseFromJSON(lte_v2x_services_auth);
    }

    cJSON *nr_ue_sidelink_ambr = cJSON_GetObjectItemCaseSensitive(v2x_contextJSON, "nrUeSidelinkAmbr");

    if (nr_ue_sidelink_ambr) {
        if (!cJSON_IsString(nr_ue_sidelink_ambr)) {
            ogs_error("OpenAPI_v2x_context_parseFromJSON() failed [nr_ue_sidelink_ambr]");
            goto end;
        }
    }

    cJSON *lte_ue_sidelink_ambr = cJSON_GetObjectItemCaseSensitive(v2x_contextJSON, "lteUeSidelinkAmbr");

    if (lte_ue_sidelink_ambr) {
        if (!cJSON_IsString(lte_ue_sidelink_ambr)) {
            ogs_error("OpenAPI_v2x_context_parseFromJSON() failed [lte_ue_sidelink_ambr]");
            goto end;
        }
    }

    cJSON *pc5_qo_s_para = cJSON_GetObjectItemCaseSensitive(v2x_contextJSON, "Pc5QoSPara");

    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para_local_nonprim = NULL;
    if (pc5_qo_s_para) {
        pc5_qo_s_para_local_nonprim = OpenAPI_pc5_qo_s_para_parseFromJSON(pc5_qo_s_para);
    }

    v2x_context_local_var = OpenAPI_v2x_context_create (
        nr_v2x_services_auth ? nr_v2x_services_auth_local_nonprim : NULL,
        lte_v2x_services_auth ? lte_v2x_services_auth_local_nonprim : NULL,
        nr_ue_sidelink_ambr ? ogs_strdup(nr_ue_sidelink_ambr->valuestring) : NULL,
        lte_ue_sidelink_ambr ? ogs_strdup(lte_ue_sidelink_ambr->valuestring) : NULL,
        pc5_qo_s_para ? pc5_qo_s_para_local_nonprim : NULL
        );

    return v2x_context_local_var;
end:
    return NULL;
}

OpenAPI_v2x_context_t *OpenAPI_v2x_context_copy(OpenAPI_v2x_context_t *dst, OpenAPI_v2x_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_v2x_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_v2x_context_convertToJSON() failed");
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

    OpenAPI_v2x_context_free(dst);
    dst = OpenAPI_v2x_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

