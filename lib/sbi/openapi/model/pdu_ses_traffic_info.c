
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_ses_traffic_info.h"

OpenAPI_pdu_ses_traffic_info_t *OpenAPI_pdu_ses_traffic_info_create(
    OpenAPI_list_t *supis,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *td_match_trafs,
    OpenAPI_list_t *td_unmatch_trafs
)
{
    OpenAPI_pdu_ses_traffic_info_t *pdu_ses_traffic_info_local_var = ogs_malloc(sizeof(OpenAPI_pdu_ses_traffic_info_t));
    ogs_assert(pdu_ses_traffic_info_local_var);

    pdu_ses_traffic_info_local_var->supis = supis;
    pdu_ses_traffic_info_local_var->dnn = dnn;
    pdu_ses_traffic_info_local_var->snssai = snssai;
    pdu_ses_traffic_info_local_var->td_match_trafs = td_match_trafs;
    pdu_ses_traffic_info_local_var->td_unmatch_trafs = td_unmatch_trafs;

    return pdu_ses_traffic_info_local_var;
}

void OpenAPI_pdu_ses_traffic_info_free(OpenAPI_pdu_ses_traffic_info_t *pdu_ses_traffic_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_ses_traffic_info) {
        return;
    }
    if (pdu_ses_traffic_info->supis) {
        OpenAPI_list_for_each(pdu_ses_traffic_info->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_ses_traffic_info->supis);
        pdu_ses_traffic_info->supis = NULL;
    }
    if (pdu_ses_traffic_info->dnn) {
        ogs_free(pdu_ses_traffic_info->dnn);
        pdu_ses_traffic_info->dnn = NULL;
    }
    if (pdu_ses_traffic_info->snssai) {
        OpenAPI_snssai_free(pdu_ses_traffic_info->snssai);
        pdu_ses_traffic_info->snssai = NULL;
    }
    if (pdu_ses_traffic_info->td_match_trafs) {
        OpenAPI_list_for_each(pdu_ses_traffic_info->td_match_trafs, node) {
            OpenAPI_td_traffic_free(node->data);
        }
        OpenAPI_list_free(pdu_ses_traffic_info->td_match_trafs);
        pdu_ses_traffic_info->td_match_trafs = NULL;
    }
    if (pdu_ses_traffic_info->td_unmatch_trafs) {
        OpenAPI_list_for_each(pdu_ses_traffic_info->td_unmatch_trafs, node) {
            OpenAPI_td_traffic_free(node->data);
        }
        OpenAPI_list_free(pdu_ses_traffic_info->td_unmatch_trafs);
        pdu_ses_traffic_info->td_unmatch_trafs = NULL;
    }
    ogs_free(pdu_ses_traffic_info);
}

cJSON *OpenAPI_pdu_ses_traffic_info_convertToJSON(OpenAPI_pdu_ses_traffic_info_t *pdu_ses_traffic_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_ses_traffic_info == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [PduSesTrafficInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_ses_traffic_info->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_ses_traffic_info->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (pdu_ses_traffic_info->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pdu_ses_traffic_info->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (pdu_ses_traffic_info->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_ses_traffic_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (pdu_ses_traffic_info->td_match_trafs) {
    cJSON *td_match_trafsList = cJSON_AddArrayToObject(item, "tdMatchTrafs");
    if (td_match_trafsList == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [td_match_trafs]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_ses_traffic_info->td_match_trafs, node) {
        cJSON *itemLocal = OpenAPI_td_traffic_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [td_match_trafs]");
            goto end;
        }
        cJSON_AddItemToArray(td_match_trafsList, itemLocal);
    }
    }

    if (pdu_ses_traffic_info->td_unmatch_trafs) {
    cJSON *td_unmatch_trafsList = cJSON_AddArrayToObject(item, "tdUnmatchTrafs");
    if (td_unmatch_trafsList == NULL) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [td_unmatch_trafs]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_ses_traffic_info->td_unmatch_trafs, node) {
        cJSON *itemLocal = OpenAPI_td_traffic_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed [td_unmatch_trafs]");
            goto end;
        }
        cJSON_AddItemToArray(td_unmatch_trafsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_pdu_ses_traffic_info_t *OpenAPI_pdu_ses_traffic_info_parseFromJSON(cJSON *pdu_ses_traffic_infoJSON)
{
    OpenAPI_pdu_ses_traffic_info_t *pdu_ses_traffic_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *td_match_trafs = NULL;
    OpenAPI_list_t *td_match_trafsList = NULL;
    cJSON *td_unmatch_trafs = NULL;
    OpenAPI_list_t *td_unmatch_trafsList = NULL;
    supis = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_infoJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_pdu_ses_traffic_info_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_pdu_ses_traffic_info_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_infoJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_infoJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    td_match_trafs = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_infoJSON, "tdMatchTrafs");
    if (td_match_trafs) {
        cJSON *td_match_trafs_local = NULL;
        if (!cJSON_IsArray(td_match_trafs)) {
            ogs_error("OpenAPI_pdu_ses_traffic_info_parseFromJSON() failed [td_match_trafs]");
            goto end;
        }

        td_match_trafsList = OpenAPI_list_create();

        cJSON_ArrayForEach(td_match_trafs_local, td_match_trafs) {
            if (!cJSON_IsObject(td_match_trafs_local)) {
                ogs_error("OpenAPI_pdu_ses_traffic_info_parseFromJSON() failed [td_match_trafs]");
                goto end;
            }
            OpenAPI_td_traffic_t *td_match_trafsItem = OpenAPI_td_traffic_parseFromJSON(td_match_trafs_local);
            if (!td_match_trafsItem) {
                ogs_error("No td_match_trafsItem");
                goto end;
            }
            OpenAPI_list_add(td_match_trafsList, td_match_trafsItem);
        }
    }

    td_unmatch_trafs = cJSON_GetObjectItemCaseSensitive(pdu_ses_traffic_infoJSON, "tdUnmatchTrafs");
    if (td_unmatch_trafs) {
        cJSON *td_unmatch_trafs_local = NULL;
        if (!cJSON_IsArray(td_unmatch_trafs)) {
            ogs_error("OpenAPI_pdu_ses_traffic_info_parseFromJSON() failed [td_unmatch_trafs]");
            goto end;
        }

        td_unmatch_trafsList = OpenAPI_list_create();

        cJSON_ArrayForEach(td_unmatch_trafs_local, td_unmatch_trafs) {
            if (!cJSON_IsObject(td_unmatch_trafs_local)) {
                ogs_error("OpenAPI_pdu_ses_traffic_info_parseFromJSON() failed [td_unmatch_trafs]");
                goto end;
            }
            OpenAPI_td_traffic_t *td_unmatch_trafsItem = OpenAPI_td_traffic_parseFromJSON(td_unmatch_trafs_local);
            if (!td_unmatch_trafsItem) {
                ogs_error("No td_unmatch_trafsItem");
                goto end;
            }
            OpenAPI_list_add(td_unmatch_trafsList, td_unmatch_trafsItem);
        }
    }

    pdu_ses_traffic_info_local_var = OpenAPI_pdu_ses_traffic_info_create (
        supis ? supisList : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        td_match_trafs ? td_match_trafsList : NULL,
        td_unmatch_trafs ? td_unmatch_trafsList : NULL
    );

    return pdu_ses_traffic_info_local_var;
end:
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (td_match_trafsList) {
        OpenAPI_list_for_each(td_match_trafsList, node) {
            OpenAPI_td_traffic_free(node->data);
        }
        OpenAPI_list_free(td_match_trafsList);
        td_match_trafsList = NULL;
    }
    if (td_unmatch_trafsList) {
        OpenAPI_list_for_each(td_unmatch_trafsList, node) {
            OpenAPI_td_traffic_free(node->data);
        }
        OpenAPI_list_free(td_unmatch_trafsList);
        td_unmatch_trafsList = NULL;
    }
    return NULL;
}

OpenAPI_pdu_ses_traffic_info_t *OpenAPI_pdu_ses_traffic_info_copy(OpenAPI_pdu_ses_traffic_info_t *dst, OpenAPI_pdu_ses_traffic_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_ses_traffic_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_ses_traffic_info_convertToJSON() failed");
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

    OpenAPI_pdu_ses_traffic_info_free(dst);
    dst = OpenAPI_pdu_ses_traffic_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

