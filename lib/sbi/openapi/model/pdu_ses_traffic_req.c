
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_ses_traffic_req.h"

OpenAPI_pdu_ses_traffic_req_t *OpenAPI_pdu_ses_traffic_req_create(
    OpenAPI_list_t *flow_descs,
    char *app_id,
    OpenAPI_list_t *domain_descs
)
{
    OpenAPI_pdu_ses_traffic_req_t *pdu_ses_traffic_req_local_var = ogs_malloc(sizeof(OpenAPI_pdu_ses_traffic_req_t));
    ogs_assert(pdu_ses_traffic_req_local_var);

    pdu_ses_traffic_req_local_var->flow_descs = flow_descs;
    pdu_ses_traffic_req_local_var->app_id = app_id;
    pdu_ses_traffic_req_local_var->domain_descs = domain_descs;

    return pdu_ses_traffic_req_local_var;
}

void OpenAPI_pdu_ses_traffic_req_free(OpenAPI_pdu_ses_traffic_req_t *pdu_ses_traffic_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_ses_traffic_req) {
        return;
    }
    if (pdu_ses_traffic_req->flow_descs) {
        OpenAPI_list_for_each(pdu_ses_traffic_req->flow_descs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_ses_traffic_req->flow_descs);
        pdu_ses_traffic_req->flow_descs = NULL;
    }
    if (pdu_ses_traffic_req->app_id) {
        ogs_free(pdu_ses_traffic_req->app_id);
        pdu_ses_traffic_req->app_id = NULL;
    }
    if (pdu_ses_traffic_req->domain_descs) {
        OpenAPI_list_for_each(pdu_ses_traffic_req->domain_descs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_ses_traffic_req->domain_descs);
        pdu_ses_traffic_req->domain_descs = NULL;
    }
    ogs_free(pdu_ses_traffic_req);
}

cJSON *OpenAPI_pdu_ses_traffic_req_convertToJSON(OpenAPI_pdu_ses_traffic_req_t *pdu_ses_traffic_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_ses_traffic_req == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_req_convertToJSON() failed [PduSesTrafficReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_ses_traffic_req->flow_descs) {
    cJSON *flow_descsList = cJSON_AddArrayToObject(item, "flowDescs");
    if (flow_descsList == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_req_convertToJSON() failed [flow_descs]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_ses_traffic_req->flow_descs, node) {
        if (cJSON_AddStringToObject(flow_descsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_ses_traffic_req_convertToJSON() failed [flow_descs]");
            goto end;
        }
    }
    }

    if (pdu_ses_traffic_req->app_id) {
    if (cJSON_AddStringToObject(item, "appId", pdu_ses_traffic_req->app_id) == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_req_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (pdu_ses_traffic_req->domain_descs) {
    cJSON *domain_descsList = cJSON_AddArrayToObject(item, "domainDescs");
    if (domain_descsList == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_req_convertToJSON() failed [domain_descs]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_ses_traffic_req->domain_descs, node) {
        if (cJSON_AddStringToObject(domain_descsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_ses_traffic_req_convertToJSON() failed [domain_descs]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pdu_ses_traffic_req_t *OpenAPI_pdu_ses_traffic_req_parseFromJSON(cJSON *pdu_ses_traffic_reqJSON)
{
    OpenAPI_pdu_ses_traffic_req_t *pdu_ses_traffic_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flow_descs = NULL;
    OpenAPI_list_t *flow_descsList = NULL;
    cJSON *app_id = NULL;
    cJSON *domain_descs = NULL;
    OpenAPI_list_t *domain_descsList = NULL;
    flow_descs = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_reqJSON, "flowDescs");
    if (flow_descs) {
        cJSON *flow_descs_local = NULL;
        if (!cJSON_IsArray(flow_descs)) {
            ogs_error("OpenAPI_pdu_ses_traffic_req_parseFromJSON() failed [flow_descs]");
            goto end;
        }

        flow_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_descs_local, flow_descs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(flow_descs_local)) {
                ogs_error("OpenAPI_pdu_ses_traffic_req_parseFromJSON() failed [flow_descs]");
                goto end;
            }
            OpenAPI_list_add(flow_descsList, ogs_strdup(flow_descs_local->valuestring));
        }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_reqJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_pdu_ses_traffic_req_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    domain_descs = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_reqJSON, "domainDescs");
    if (domain_descs) {
        cJSON *domain_descs_local = NULL;
        if (!cJSON_IsArray(domain_descs)) {
            ogs_error("OpenAPI_pdu_ses_traffic_req_parseFromJSON() failed [domain_descs]");
            goto end;
        }

        domain_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(domain_descs_local, domain_descs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(domain_descs_local)) {
                ogs_error("OpenAPI_pdu_ses_traffic_req_parseFromJSON() failed [domain_descs]");
                goto end;
            }
            OpenAPI_list_add(domain_descsList, ogs_strdup(domain_descs_local->valuestring));
        }
    }

    pdu_ses_traffic_req_local_var = OpenAPI_pdu_ses_traffic_req_create (
        flow_descs ? flow_descsList : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        domain_descs ? domain_descsList : NULL
    );

    return pdu_ses_traffic_req_local_var;
end:
    if (flow_descsList) {
        OpenAPI_list_for_each(flow_descsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(flow_descsList);
        flow_descsList = NULL;
    }
    if (domain_descsList) {
        OpenAPI_list_for_each(domain_descsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(domain_descsList);
        domain_descsList = NULL;
    }
    return NULL;
}

OpenAPI_pdu_ses_traffic_req_t *OpenAPI_pdu_ses_traffic_req_copy(OpenAPI_pdu_ses_traffic_req_t *dst, OpenAPI_pdu_ses_traffic_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_ses_traffic_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_ses_traffic_req_convertToJSON() failed");
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

    OpenAPI_pdu_ses_traffic_req_free(dst);
    dst = OpenAPI_pdu_ses_traffic_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

