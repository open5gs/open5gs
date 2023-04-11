
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "global_ran_node_id_1.h"

OpenAPI_global_ran_node_id_1_t *OpenAPI_global_ran_node_id_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *n3_iwf_id,
    OpenAPI_gnb_id_t *g_nb_id,
    char *nge_nb_id,
    char *wagf_id,
    char *tngf_id,
    char *nid,
    char *e_nb_id
)
{
    OpenAPI_global_ran_node_id_1_t *global_ran_node_id_1_local_var = ogs_malloc(sizeof(OpenAPI_global_ran_node_id_1_t));
    ogs_assert(global_ran_node_id_1_local_var);

    global_ran_node_id_1_local_var->plmn_id = plmn_id;
    global_ran_node_id_1_local_var->n3_iwf_id = n3_iwf_id;
    global_ran_node_id_1_local_var->g_nb_id = g_nb_id;
    global_ran_node_id_1_local_var->nge_nb_id = nge_nb_id;
    global_ran_node_id_1_local_var->wagf_id = wagf_id;
    global_ran_node_id_1_local_var->tngf_id = tngf_id;
    global_ran_node_id_1_local_var->nid = nid;
    global_ran_node_id_1_local_var->e_nb_id = e_nb_id;

    return global_ran_node_id_1_local_var;
}

void OpenAPI_global_ran_node_id_1_free(OpenAPI_global_ran_node_id_1_t *global_ran_node_id_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == global_ran_node_id_1) {
        return;
    }
    if (global_ran_node_id_1->plmn_id) {
        OpenAPI_plmn_id_1_free(global_ran_node_id_1->plmn_id);
        global_ran_node_id_1->plmn_id = NULL;
    }
    if (global_ran_node_id_1->n3_iwf_id) {
        ogs_free(global_ran_node_id_1->n3_iwf_id);
        global_ran_node_id_1->n3_iwf_id = NULL;
    }
    if (global_ran_node_id_1->g_nb_id) {
        OpenAPI_gnb_id_free(global_ran_node_id_1->g_nb_id);
        global_ran_node_id_1->g_nb_id = NULL;
    }
    if (global_ran_node_id_1->nge_nb_id) {
        ogs_free(global_ran_node_id_1->nge_nb_id);
        global_ran_node_id_1->nge_nb_id = NULL;
    }
    if (global_ran_node_id_1->wagf_id) {
        ogs_free(global_ran_node_id_1->wagf_id);
        global_ran_node_id_1->wagf_id = NULL;
    }
    if (global_ran_node_id_1->tngf_id) {
        ogs_free(global_ran_node_id_1->tngf_id);
        global_ran_node_id_1->tngf_id = NULL;
    }
    if (global_ran_node_id_1->nid) {
        ogs_free(global_ran_node_id_1->nid);
        global_ran_node_id_1->nid = NULL;
    }
    if (global_ran_node_id_1->e_nb_id) {
        ogs_free(global_ran_node_id_1->e_nb_id);
        global_ran_node_id_1->e_nb_id = NULL;
    }
    ogs_free(global_ran_node_id_1);
}

cJSON *OpenAPI_global_ran_node_id_1_convertToJSON(OpenAPI_global_ran_node_id_1_t *global_ran_node_id_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (global_ran_node_id_1 == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [GlobalRanNodeId_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!global_ran_node_id_1->plmn_id) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(global_ran_node_id_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (global_ran_node_id_1->n3_iwf_id) {
    if (cJSON_AddStringToObject(item, "n3IwfId", global_ran_node_id_1->n3_iwf_id) == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [n3_iwf_id]");
        goto end;
    }
    }

    if (global_ran_node_id_1->g_nb_id) {
    cJSON *g_nb_id_local_JSON = OpenAPI_gnb_id_convertToJSON(global_ran_node_id_1->g_nb_id);
    if (g_nb_id_local_JSON == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [g_nb_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "gNbId", g_nb_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [g_nb_id]");
        goto end;
    }
    }

    if (global_ran_node_id_1->nge_nb_id) {
    if (cJSON_AddStringToObject(item, "ngeNbId", global_ran_node_id_1->nge_nb_id) == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [nge_nb_id]");
        goto end;
    }
    }

    if (global_ran_node_id_1->wagf_id) {
    if (cJSON_AddStringToObject(item, "wagfId", global_ran_node_id_1->wagf_id) == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [wagf_id]");
        goto end;
    }
    }

    if (global_ran_node_id_1->tngf_id) {
    if (cJSON_AddStringToObject(item, "tngfId", global_ran_node_id_1->tngf_id) == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [tngf_id]");
        goto end;
    }
    }

    if (global_ran_node_id_1->nid) {
    if (cJSON_AddStringToObject(item, "nid", global_ran_node_id_1->nid) == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [nid]");
        goto end;
    }
    }

    if (global_ran_node_id_1->e_nb_id) {
    if (cJSON_AddStringToObject(item, "eNbId", global_ran_node_id_1->e_nb_id) == NULL) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed [e_nb_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_global_ran_node_id_1_t *OpenAPI_global_ran_node_id_1_parseFromJSON(cJSON *global_ran_node_id_1JSON)
{
    OpenAPI_global_ran_node_id_1_t *global_ran_node_id_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    cJSON *n3_iwf_id = NULL;
    cJSON *g_nb_id = NULL;
    OpenAPI_gnb_id_t *g_nb_id_local_nonprim = NULL;
    cJSON *nge_nb_id = NULL;
    cJSON *wagf_id = NULL;
    cJSON *tngf_id = NULL;
    cJSON *nid = NULL;
    cJSON *e_nb_id = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_global_ran_node_id_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON failed [plmn_id]");
        goto end;
    }

    n3_iwf_id = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "n3IwfId");
    if (n3_iwf_id) {
    if (!cJSON_IsString(n3_iwf_id) && !cJSON_IsNull(n3_iwf_id)) {
        ogs_error("OpenAPI_global_ran_node_id_1_parseFromJSON() failed [n3_iwf_id]");
        goto end;
    }
    }

    g_nb_id = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "gNbId");
    if (g_nb_id) {
    g_nb_id_local_nonprim = OpenAPI_gnb_id_parseFromJSON(g_nb_id);
    if (!g_nb_id_local_nonprim) {
        ogs_error("OpenAPI_gnb_id_parseFromJSON failed [g_nb_id]");
        goto end;
    }
    }

    nge_nb_id = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "ngeNbId");
    if (nge_nb_id) {
    if (!cJSON_IsString(nge_nb_id) && !cJSON_IsNull(nge_nb_id)) {
        ogs_error("OpenAPI_global_ran_node_id_1_parseFromJSON() failed [nge_nb_id]");
        goto end;
    }
    }

    wagf_id = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "wagfId");
    if (wagf_id) {
    if (!cJSON_IsString(wagf_id) && !cJSON_IsNull(wagf_id)) {
        ogs_error("OpenAPI_global_ran_node_id_1_parseFromJSON() failed [wagf_id]");
        goto end;
    }
    }

    tngf_id = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "tngfId");
    if (tngf_id) {
    if (!cJSON_IsString(tngf_id) && !cJSON_IsNull(tngf_id)) {
        ogs_error("OpenAPI_global_ran_node_id_1_parseFromJSON() failed [tngf_id]");
        goto end;
    }
    }

    nid = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_global_ran_node_id_1_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    e_nb_id = cJSON_GetObjectItemCaseSensitive(global_ran_node_id_1JSON, "eNbId");
    if (e_nb_id) {
    if (!cJSON_IsString(e_nb_id) && !cJSON_IsNull(e_nb_id)) {
        ogs_error("OpenAPI_global_ran_node_id_1_parseFromJSON() failed [e_nb_id]");
        goto end;
    }
    }

    global_ran_node_id_1_local_var = OpenAPI_global_ran_node_id_1_create (
        plmn_id_local_nonprim,
        n3_iwf_id && !cJSON_IsNull(n3_iwf_id) ? ogs_strdup(n3_iwf_id->valuestring) : NULL,
        g_nb_id ? g_nb_id_local_nonprim : NULL,
        nge_nb_id && !cJSON_IsNull(nge_nb_id) ? ogs_strdup(nge_nb_id->valuestring) : NULL,
        wagf_id && !cJSON_IsNull(wagf_id) ? ogs_strdup(wagf_id->valuestring) : NULL,
        tngf_id && !cJSON_IsNull(tngf_id) ? ogs_strdup(tngf_id->valuestring) : NULL,
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL,
        e_nb_id && !cJSON_IsNull(e_nb_id) ? ogs_strdup(e_nb_id->valuestring) : NULL
    );

    return global_ran_node_id_1_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_1_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (g_nb_id_local_nonprim) {
        OpenAPI_gnb_id_free(g_nb_id_local_nonprim);
        g_nb_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_global_ran_node_id_1_t *OpenAPI_global_ran_node_id_1_copy(OpenAPI_global_ran_node_id_1_t *dst, OpenAPI_global_ran_node_id_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_global_ran_node_id_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_global_ran_node_id_1_convertToJSON() failed");
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

    OpenAPI_global_ran_node_id_1_free(dst);
    dst = OpenAPI_global_ran_node_id_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

