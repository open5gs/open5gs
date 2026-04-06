
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "header_handling_action_request_1.h"

OpenAPI_header_handling_action_request_1_t *OpenAPI_header_handling_action_request_1_create(
    char *h_hndlg_ctrl_ref,
    OpenAPI_list_t *h_hndlg_action,
    char *h_info,
    char *h_val,
    OpenAPI_header_handling_cond_e h_hndlg_cond,
    OpenAPI_header_handling_reporting_1_t *h_hndlg_rep
)
{
    OpenAPI_header_handling_action_request_1_t *header_handling_action_request_1_local_var = ogs_malloc(sizeof(OpenAPI_header_handling_action_request_1_t));
    ogs_assert(header_handling_action_request_1_local_var);

    header_handling_action_request_1_local_var->h_hndlg_ctrl_ref = h_hndlg_ctrl_ref;
    header_handling_action_request_1_local_var->h_hndlg_action = h_hndlg_action;
    header_handling_action_request_1_local_var->h_info = h_info;
    header_handling_action_request_1_local_var->h_val = h_val;
    header_handling_action_request_1_local_var->h_hndlg_cond = h_hndlg_cond;
    header_handling_action_request_1_local_var->h_hndlg_rep = h_hndlg_rep;

    return header_handling_action_request_1_local_var;
}

void OpenAPI_header_handling_action_request_1_free(OpenAPI_header_handling_action_request_1_t *header_handling_action_request_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == header_handling_action_request_1) {
        return;
    }
    if (header_handling_action_request_1->h_hndlg_ctrl_ref) {
        ogs_free(header_handling_action_request_1->h_hndlg_ctrl_ref);
        header_handling_action_request_1->h_hndlg_ctrl_ref = NULL;
    }
    if (header_handling_action_request_1->h_hndlg_action) {
        OpenAPI_list_free(header_handling_action_request_1->h_hndlg_action);
        header_handling_action_request_1->h_hndlg_action = NULL;
    }
    if (header_handling_action_request_1->h_info) {
        ogs_free(header_handling_action_request_1->h_info);
        header_handling_action_request_1->h_info = NULL;
    }
    if (header_handling_action_request_1->h_val) {
        ogs_free(header_handling_action_request_1->h_val);
        header_handling_action_request_1->h_val = NULL;
    }
    if (header_handling_action_request_1->h_hndlg_rep) {
        OpenAPI_header_handling_reporting_1_free(header_handling_action_request_1->h_hndlg_rep);
        header_handling_action_request_1->h_hndlg_rep = NULL;
    }
    ogs_free(header_handling_action_request_1);
}

cJSON *OpenAPI_header_handling_action_request_1_convertToJSON(OpenAPI_header_handling_action_request_1_t *header_handling_action_request_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (header_handling_action_request_1 == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [HeaderHandlingActionRequest_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (header_handling_action_request_1->h_hndlg_ctrl_ref) {
    if (cJSON_AddStringToObject(item, "hHndlgCtrlRef", header_handling_action_request_1->h_hndlg_ctrl_ref) == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_hndlg_ctrl_ref]");
        goto end;
    }
    }

    if (header_handling_action_request_1->h_hndlg_action != OpenAPI_header_handling_action_NULL) {
    cJSON *h_hndlg_actionList = cJSON_AddArrayToObject(item, "hHndlgAction");
    if (h_hndlg_actionList == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_hndlg_action]");
        goto end;
    }
    OpenAPI_list_for_each(header_handling_action_request_1->h_hndlg_action, node) {
        if (cJSON_AddStringToObject(h_hndlg_actionList, "", OpenAPI_header_handling_action_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_hndlg_action]");
            goto end;
        }
    }
    }

    if (header_handling_action_request_1->h_info) {
    if (cJSON_AddStringToObject(item, "hInfo", header_handling_action_request_1->h_info) == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_info]");
        goto end;
    }
    }

    if (header_handling_action_request_1->h_val) {
    if (cJSON_AddStringToObject(item, "hVal", header_handling_action_request_1->h_val) == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_val]");
        goto end;
    }
    }

    if (header_handling_action_request_1->h_hndlg_cond != OpenAPI_header_handling_cond_NULL) {
    if (cJSON_AddStringToObject(item, "hHndlgCond", OpenAPI_header_handling_cond_ToString(header_handling_action_request_1->h_hndlg_cond)) == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_hndlg_cond]");
        goto end;
    }
    }

    if (header_handling_action_request_1->h_hndlg_rep) {
    cJSON *h_hndlg_rep_local_JSON = OpenAPI_header_handling_reporting_1_convertToJSON(header_handling_action_request_1->h_hndlg_rep);
    if (h_hndlg_rep_local_JSON == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_hndlg_rep]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hHndlgRep", h_hndlg_rep_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed [h_hndlg_rep]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_header_handling_action_request_1_t *OpenAPI_header_handling_action_request_1_parseFromJSON(cJSON *header_handling_action_request_1JSON)
{
    OpenAPI_header_handling_action_request_1_t *header_handling_action_request_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_hndlg_ctrl_ref = NULL;
    cJSON *h_hndlg_action = NULL;
    OpenAPI_list_t *h_hndlg_actionList = NULL;
    cJSON *h_info = NULL;
    cJSON *h_val = NULL;
    cJSON *h_hndlg_cond = NULL;
    OpenAPI_header_handling_cond_e h_hndlg_condVariable = 0;
    cJSON *h_hndlg_rep = NULL;
    OpenAPI_header_handling_reporting_1_t *h_hndlg_rep_local_nonprim = NULL;
    h_hndlg_ctrl_ref = cJSON_GetObjectItemCaseSensitive(header_handling_action_request_1JSON, "hHndlgCtrlRef");
    if (h_hndlg_ctrl_ref) {
    if (!cJSON_IsString(h_hndlg_ctrl_ref) && !cJSON_IsNull(h_hndlg_ctrl_ref)) {
        ogs_error("OpenAPI_header_handling_action_request_1_parseFromJSON() failed [h_hndlg_ctrl_ref]");
        goto end;
    }
    }

    h_hndlg_action = cJSON_GetObjectItemCaseSensitive(header_handling_action_request_1JSON, "hHndlgAction");
    if (h_hndlg_action) {
        cJSON *h_hndlg_action_local = NULL;
        if (!cJSON_IsArray(h_hndlg_action)) {
            ogs_error("OpenAPI_header_handling_action_request_1_parseFromJSON() failed [h_hndlg_action]");
            goto end;
        }

        h_hndlg_actionList = OpenAPI_list_create();

        cJSON_ArrayForEach(h_hndlg_action_local, h_hndlg_action) {
            OpenAPI_header_handling_action_e localEnum = OpenAPI_header_handling_action_NULL;
            if (!cJSON_IsString(h_hndlg_action_local)) {
                ogs_error("OpenAPI_header_handling_action_request_1_parseFromJSON() failed [h_hndlg_action]");
                goto end;
            }
            localEnum = OpenAPI_header_handling_action_FromString(h_hndlg_action_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"h_hndlg_action\" is not supported. Ignoring it ...",
                         h_hndlg_action_local->valuestring);
            } else {
                OpenAPI_list_add(h_hndlg_actionList, (void *)localEnum);
            }
        }
        if (h_hndlg_actionList->count == 0) {
            ogs_error("OpenAPI_header_handling_action_request_1_parseFromJSON() failed: Expected h_hndlg_actionList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    h_info = cJSON_GetObjectItemCaseSensitive(header_handling_action_request_1JSON, "hInfo");
    if (h_info) {
    if (!cJSON_IsString(h_info) && !cJSON_IsNull(h_info)) {
        ogs_error("OpenAPI_header_handling_action_request_1_parseFromJSON() failed [h_info]");
        goto end;
    }
    }

    h_val = cJSON_GetObjectItemCaseSensitive(header_handling_action_request_1JSON, "hVal");
    if (h_val) {
    if (!cJSON_IsString(h_val) && !cJSON_IsNull(h_val)) {
        ogs_error("OpenAPI_header_handling_action_request_1_parseFromJSON() failed [h_val]");
        goto end;
    }
    }

    h_hndlg_cond = cJSON_GetObjectItemCaseSensitive(header_handling_action_request_1JSON, "hHndlgCond");
    if (h_hndlg_cond) {
    if (!cJSON_IsString(h_hndlg_cond)) {
        ogs_error("OpenAPI_header_handling_action_request_1_parseFromJSON() failed [h_hndlg_cond]");
        goto end;
    }
    h_hndlg_condVariable = OpenAPI_header_handling_cond_FromString(h_hndlg_cond->valuestring);
    }

    h_hndlg_rep = cJSON_GetObjectItemCaseSensitive(header_handling_action_request_1JSON, "hHndlgRep");
    if (h_hndlg_rep) {
    h_hndlg_rep_local_nonprim = OpenAPI_header_handling_reporting_1_parseFromJSON(h_hndlg_rep);
    if (!h_hndlg_rep_local_nonprim) {
        ogs_error("OpenAPI_header_handling_reporting_1_parseFromJSON failed [h_hndlg_rep]");
        goto end;
    }
    }

    header_handling_action_request_1_local_var = OpenAPI_header_handling_action_request_1_create (
        h_hndlg_ctrl_ref && !cJSON_IsNull(h_hndlg_ctrl_ref) ? ogs_strdup(h_hndlg_ctrl_ref->valuestring) : NULL,
        h_hndlg_action ? h_hndlg_actionList : NULL,
        h_info && !cJSON_IsNull(h_info) ? ogs_strdup(h_info->valuestring) : NULL,
        h_val && !cJSON_IsNull(h_val) ? ogs_strdup(h_val->valuestring) : NULL,
        h_hndlg_cond ? h_hndlg_condVariable : 0,
        h_hndlg_rep ? h_hndlg_rep_local_nonprim : NULL
    );

    return header_handling_action_request_1_local_var;
end:
    if (h_hndlg_actionList) {
        OpenAPI_list_free(h_hndlg_actionList);
        h_hndlg_actionList = NULL;
    }
    if (h_hndlg_rep_local_nonprim) {
        OpenAPI_header_handling_reporting_1_free(h_hndlg_rep_local_nonprim);
        h_hndlg_rep_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_header_handling_action_request_1_t *OpenAPI_header_handling_action_request_1_copy(OpenAPI_header_handling_action_request_1_t *dst, OpenAPI_header_handling_action_request_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_header_handling_action_request_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_header_handling_action_request_1_convertToJSON() failed");
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

    OpenAPI_header_handling_action_request_1_free(dst);
    dst = OpenAPI_header_handling_action_request_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

