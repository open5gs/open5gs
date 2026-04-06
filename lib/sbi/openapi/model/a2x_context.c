
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "a2x_context.h"

OpenAPI_a2x_context_t *OpenAPI_a2x_context_create(
    OpenAPI_nr_a2x_auth_t *nr_a2x_services_auth,
    OpenAPI_lte_a2x_auth_t *lte_a2x_services_auth,
    char *nr_ue_sidelink_ambr,
    char *lte_ue_sidelink_ambr,
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para
)
{
    OpenAPI_a2x_context_t *a2x_context_local_var = ogs_malloc(sizeof(OpenAPI_a2x_context_t));
    ogs_assert(a2x_context_local_var);

    a2x_context_local_var->nr_a2x_services_auth = nr_a2x_services_auth;
    a2x_context_local_var->lte_a2x_services_auth = lte_a2x_services_auth;
    a2x_context_local_var->nr_ue_sidelink_ambr = nr_ue_sidelink_ambr;
    a2x_context_local_var->lte_ue_sidelink_ambr = lte_ue_sidelink_ambr;
    a2x_context_local_var->pc5_qo_s_para = pc5_qo_s_para;

    return a2x_context_local_var;
}

void OpenAPI_a2x_context_free(OpenAPI_a2x_context_t *a2x_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == a2x_context) {
        return;
    }
    if (a2x_context->nr_a2x_services_auth) {
        OpenAPI_nr_a2x_auth_free(a2x_context->nr_a2x_services_auth);
        a2x_context->nr_a2x_services_auth = NULL;
    }
    if (a2x_context->lte_a2x_services_auth) {
        OpenAPI_lte_a2x_auth_free(a2x_context->lte_a2x_services_auth);
        a2x_context->lte_a2x_services_auth = NULL;
    }
    if (a2x_context->nr_ue_sidelink_ambr) {
        ogs_free(a2x_context->nr_ue_sidelink_ambr);
        a2x_context->nr_ue_sidelink_ambr = NULL;
    }
    if (a2x_context->lte_ue_sidelink_ambr) {
        ogs_free(a2x_context->lte_ue_sidelink_ambr);
        a2x_context->lte_ue_sidelink_ambr = NULL;
    }
    if (a2x_context->pc5_qo_s_para) {
        OpenAPI_pc5_qo_s_para_free(a2x_context->pc5_qo_s_para);
        a2x_context->pc5_qo_s_para = NULL;
    }
    ogs_free(a2x_context);
}

cJSON *OpenAPI_a2x_context_convertToJSON(OpenAPI_a2x_context_t *a2x_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (a2x_context == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [A2xContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (a2x_context->nr_a2x_services_auth) {
    cJSON *nr_a2x_services_auth_local_JSON = OpenAPI_nr_a2x_auth_convertToJSON(a2x_context->nr_a2x_services_auth);
    if (nr_a2x_services_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [nr_a2x_services_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nrA2xServicesAuth", nr_a2x_services_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [nr_a2x_services_auth]");
        goto end;
    }
    }

    if (a2x_context->lte_a2x_services_auth) {
    cJSON *lte_a2x_services_auth_local_JSON = OpenAPI_lte_a2x_auth_convertToJSON(a2x_context->lte_a2x_services_auth);
    if (lte_a2x_services_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [lte_a2x_services_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lteA2xServicesAuth", lte_a2x_services_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [lte_a2x_services_auth]");
        goto end;
    }
    }

    if (a2x_context->nr_ue_sidelink_ambr) {
    if (cJSON_AddStringToObject(item, "nrUeSidelinkAmbr", a2x_context->nr_ue_sidelink_ambr) == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [nr_ue_sidelink_ambr]");
        goto end;
    }
    }

    if (a2x_context->lte_ue_sidelink_ambr) {
    if (cJSON_AddStringToObject(item, "lteUeSidelinkAmbr", a2x_context->lte_ue_sidelink_ambr) == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [lte_ue_sidelink_ambr]");
        goto end;
    }
    }

    if (a2x_context->pc5_qo_s_para) {
    cJSON *pc5_qo_s_para_local_JSON = OpenAPI_pc5_qo_s_para_convertToJSON(a2x_context->pc5_qo_s_para);
    if (pc5_qo_s_para_local_JSON == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [pc5_qo_s_para]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pc5QoSPara", pc5_qo_s_para_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed [pc5_qo_s_para]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_a2x_context_t *OpenAPI_a2x_context_parseFromJSON(cJSON *a2x_contextJSON)
{
    OpenAPI_a2x_context_t *a2x_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nr_a2x_services_auth = NULL;
    OpenAPI_nr_a2x_auth_t *nr_a2x_services_auth_local_nonprim = NULL;
    cJSON *lte_a2x_services_auth = NULL;
    OpenAPI_lte_a2x_auth_t *lte_a2x_services_auth_local_nonprim = NULL;
    cJSON *nr_ue_sidelink_ambr = NULL;
    cJSON *lte_ue_sidelink_ambr = NULL;
    cJSON *pc5_qo_s_para = NULL;
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para_local_nonprim = NULL;
    nr_a2x_services_auth = cJSON_GetObjectItemCaseSensitive(a2x_contextJSON, "nrA2xServicesAuth");
    if (nr_a2x_services_auth) {
    nr_a2x_services_auth_local_nonprim = OpenAPI_nr_a2x_auth_parseFromJSON(nr_a2x_services_auth);
    if (!nr_a2x_services_auth_local_nonprim) {
        ogs_error("OpenAPI_nr_a2x_auth_parseFromJSON failed [nr_a2x_services_auth]");
        goto end;
    }
    }

    lte_a2x_services_auth = cJSON_GetObjectItemCaseSensitive(a2x_contextJSON, "lteA2xServicesAuth");
    if (lte_a2x_services_auth) {
    lte_a2x_services_auth_local_nonprim = OpenAPI_lte_a2x_auth_parseFromJSON(lte_a2x_services_auth);
    if (!lte_a2x_services_auth_local_nonprim) {
        ogs_error("OpenAPI_lte_a2x_auth_parseFromJSON failed [lte_a2x_services_auth]");
        goto end;
    }
    }

    nr_ue_sidelink_ambr = cJSON_GetObjectItemCaseSensitive(a2x_contextJSON, "nrUeSidelinkAmbr");
    if (nr_ue_sidelink_ambr) {
    if (!cJSON_IsString(nr_ue_sidelink_ambr) && !cJSON_IsNull(nr_ue_sidelink_ambr)) {
        ogs_error("OpenAPI_a2x_context_parseFromJSON() failed [nr_ue_sidelink_ambr]");
        goto end;
    }
    }

    lte_ue_sidelink_ambr = cJSON_GetObjectItemCaseSensitive(a2x_contextJSON, "lteUeSidelinkAmbr");
    if (lte_ue_sidelink_ambr) {
    if (!cJSON_IsString(lte_ue_sidelink_ambr) && !cJSON_IsNull(lte_ue_sidelink_ambr)) {
        ogs_error("OpenAPI_a2x_context_parseFromJSON() failed [lte_ue_sidelink_ambr]");
        goto end;
    }
    }

    pc5_qo_s_para = cJSON_GetObjectItemCaseSensitive(a2x_contextJSON, "pc5QoSPara");
    if (pc5_qo_s_para) {
    pc5_qo_s_para_local_nonprim = OpenAPI_pc5_qo_s_para_parseFromJSON(pc5_qo_s_para);
    if (!pc5_qo_s_para_local_nonprim) {
        ogs_error("OpenAPI_pc5_qo_s_para_parseFromJSON failed [pc5_qo_s_para]");
        goto end;
    }
    }

    a2x_context_local_var = OpenAPI_a2x_context_create (
        nr_a2x_services_auth ? nr_a2x_services_auth_local_nonprim : NULL,
        lte_a2x_services_auth ? lte_a2x_services_auth_local_nonprim : NULL,
        nr_ue_sidelink_ambr && !cJSON_IsNull(nr_ue_sidelink_ambr) ? ogs_strdup(nr_ue_sidelink_ambr->valuestring) : NULL,
        lte_ue_sidelink_ambr && !cJSON_IsNull(lte_ue_sidelink_ambr) ? ogs_strdup(lte_ue_sidelink_ambr->valuestring) : NULL,
        pc5_qo_s_para ? pc5_qo_s_para_local_nonprim : NULL
    );

    return a2x_context_local_var;
end:
    if (nr_a2x_services_auth_local_nonprim) {
        OpenAPI_nr_a2x_auth_free(nr_a2x_services_auth_local_nonprim);
        nr_a2x_services_auth_local_nonprim = NULL;
    }
    if (lte_a2x_services_auth_local_nonprim) {
        OpenAPI_lte_a2x_auth_free(lte_a2x_services_auth_local_nonprim);
        lte_a2x_services_auth_local_nonprim = NULL;
    }
    if (pc5_qo_s_para_local_nonprim) {
        OpenAPI_pc5_qo_s_para_free(pc5_qo_s_para_local_nonprim);
        pc5_qo_s_para_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_a2x_context_t *OpenAPI_a2x_context_copy(OpenAPI_a2x_context_t *dst, OpenAPI_a2x_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_a2x_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_a2x_context_convertToJSON() failed");
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

    OpenAPI_a2x_context_free(dst);
    dst = OpenAPI_a2x_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

