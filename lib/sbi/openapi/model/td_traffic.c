
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "td_traffic.h"

OpenAPI_td_traffic_t *OpenAPI_td_traffic_create(
    OpenAPI_list_t *pdu_ses_traf_reqs,
    bool is_ul_vol,
    long ul_vol,
    bool is_dl_vol,
    long dl_vol,
    bool is_all_vol,
    long all_vol,
    bool is_ul_num_of_pkt,
    int ul_num_of_pkt,
    bool is_dl_num_of_pkt,
    int dl_num_of_pkt,
    bool is_all_num_of_pkt,
    int all_num_of_pkt
)
{
    OpenAPI_td_traffic_t *td_traffic_local_var = ogs_malloc(sizeof(OpenAPI_td_traffic_t));
    ogs_assert(td_traffic_local_var);

    td_traffic_local_var->pdu_ses_traf_reqs = pdu_ses_traf_reqs;
    td_traffic_local_var->is_ul_vol = is_ul_vol;
    td_traffic_local_var->ul_vol = ul_vol;
    td_traffic_local_var->is_dl_vol = is_dl_vol;
    td_traffic_local_var->dl_vol = dl_vol;
    td_traffic_local_var->is_all_vol = is_all_vol;
    td_traffic_local_var->all_vol = all_vol;
    td_traffic_local_var->is_ul_num_of_pkt = is_ul_num_of_pkt;
    td_traffic_local_var->ul_num_of_pkt = ul_num_of_pkt;
    td_traffic_local_var->is_dl_num_of_pkt = is_dl_num_of_pkt;
    td_traffic_local_var->dl_num_of_pkt = dl_num_of_pkt;
    td_traffic_local_var->is_all_num_of_pkt = is_all_num_of_pkt;
    td_traffic_local_var->all_num_of_pkt = all_num_of_pkt;

    return td_traffic_local_var;
}

void OpenAPI_td_traffic_free(OpenAPI_td_traffic_t *td_traffic)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == td_traffic) {
        return;
    }
    if (td_traffic->pdu_ses_traf_reqs) {
        OpenAPI_list_for_each(td_traffic->pdu_ses_traf_reqs, node) {
            OpenAPI_pdu_ses_traffic_req_free(node->data);
        }
        OpenAPI_list_free(td_traffic->pdu_ses_traf_reqs);
        td_traffic->pdu_ses_traf_reqs = NULL;
    }
    ogs_free(td_traffic);
}

cJSON *OpenAPI_td_traffic_convertToJSON(OpenAPI_td_traffic_t *td_traffic)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (td_traffic == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [TdTraffic]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (td_traffic->pdu_ses_traf_reqs) {
    cJSON *pdu_ses_traf_reqsList = cJSON_AddArrayToObject(item, "pduSesTrafReqs");
    if (pdu_ses_traf_reqsList == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [pdu_ses_traf_reqs]");
        goto end;
    }
    OpenAPI_list_for_each(td_traffic->pdu_ses_traf_reqs, node) {
        cJSON *itemLocal = OpenAPI_pdu_ses_traffic_req_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_td_traffic_convertToJSON() failed [pdu_ses_traf_reqs]");
            goto end;
        }
        cJSON_AddItemToArray(pdu_ses_traf_reqsList, itemLocal);
    }
    }

    if (td_traffic->is_ul_vol) {
    if (cJSON_AddNumberToObject(item, "ulVol", td_traffic->ul_vol) == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [ul_vol]");
        goto end;
    }
    }

    if (td_traffic->is_dl_vol) {
    if (cJSON_AddNumberToObject(item, "dlVol", td_traffic->dl_vol) == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [dl_vol]");
        goto end;
    }
    }

    if (td_traffic->is_all_vol) {
    if (cJSON_AddNumberToObject(item, "allVol", td_traffic->all_vol) == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [all_vol]");
        goto end;
    }
    }

    if (td_traffic->is_ul_num_of_pkt) {
    if (cJSON_AddNumberToObject(item, "ulNumOfPkt", td_traffic->ul_num_of_pkt) == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [ul_num_of_pkt]");
        goto end;
    }
    }

    if (td_traffic->is_dl_num_of_pkt) {
    if (cJSON_AddNumberToObject(item, "dlNumOfPkt", td_traffic->dl_num_of_pkt) == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [dl_num_of_pkt]");
        goto end;
    }
    }

    if (td_traffic->is_all_num_of_pkt) {
    if (cJSON_AddNumberToObject(item, "allNumOfPkt", td_traffic->all_num_of_pkt) == NULL) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed [all_num_of_pkt]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_td_traffic_t *OpenAPI_td_traffic_parseFromJSON(cJSON *td_trafficJSON)
{
    OpenAPI_td_traffic_t *td_traffic_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_ses_traf_reqs = NULL;
    OpenAPI_list_t *pdu_ses_traf_reqsList = NULL;
    cJSON *ul_vol = NULL;
    cJSON *dl_vol = NULL;
    cJSON *all_vol = NULL;
    cJSON *ul_num_of_pkt = NULL;
    cJSON *dl_num_of_pkt = NULL;
    cJSON *all_num_of_pkt = NULL;
    pdu_ses_traf_reqs = cJSON_GetObjectItemCaseSensitive(td_trafficJSON, "pduSesTrafReqs");
    if (pdu_ses_traf_reqs) {
        cJSON *pdu_ses_traf_reqs_local = NULL;
        if (!cJSON_IsArray(pdu_ses_traf_reqs)) {
            ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [pdu_ses_traf_reqs]");
            goto end;
        }

        pdu_ses_traf_reqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_ses_traf_reqs_local, pdu_ses_traf_reqs) {
            if (!cJSON_IsObject(pdu_ses_traf_reqs_local)) {
                ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [pdu_ses_traf_reqs]");
                goto end;
            }
            OpenAPI_pdu_ses_traffic_req_t *pdu_ses_traf_reqsItem = OpenAPI_pdu_ses_traffic_req_parseFromJSON(pdu_ses_traf_reqs_local);
            if (!pdu_ses_traf_reqsItem) {
                ogs_error("No pdu_ses_traf_reqsItem");
                goto end;
            }
            OpenAPI_list_add(pdu_ses_traf_reqsList, pdu_ses_traf_reqsItem);
        }
    }

    ul_vol = cJSON_GetObjectItemCaseSensitive(td_trafficJSON, "ulVol");
    if (ul_vol) {
    if (!cJSON_IsNumber(ul_vol)) {
        ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [ul_vol]");
        goto end;
    }
    }

    dl_vol = cJSON_GetObjectItemCaseSensitive(td_trafficJSON, "dlVol");
    if (dl_vol) {
    if (!cJSON_IsNumber(dl_vol)) {
        ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [dl_vol]");
        goto end;
    }
    }

    all_vol = cJSON_GetObjectItemCaseSensitive(td_trafficJSON, "allVol");
    if (all_vol) {
    if (!cJSON_IsNumber(all_vol)) {
        ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [all_vol]");
        goto end;
    }
    }

    ul_num_of_pkt = cJSON_GetObjectItemCaseSensitive(td_trafficJSON, "ulNumOfPkt");
    if (ul_num_of_pkt) {
    if (!cJSON_IsNumber(ul_num_of_pkt)) {
        ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [ul_num_of_pkt]");
        goto end;
    }
    }

    dl_num_of_pkt = cJSON_GetObjectItemCaseSensitive(td_trafficJSON, "dlNumOfPkt");
    if (dl_num_of_pkt) {
    if (!cJSON_IsNumber(dl_num_of_pkt)) {
        ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [dl_num_of_pkt]");
        goto end;
    }
    }

    all_num_of_pkt = cJSON_GetObjectItemCaseSensitive(td_trafficJSON, "allNumOfPkt");
    if (all_num_of_pkt) {
    if (!cJSON_IsNumber(all_num_of_pkt)) {
        ogs_error("OpenAPI_td_traffic_parseFromJSON() failed [all_num_of_pkt]");
        goto end;
    }
    }

    td_traffic_local_var = OpenAPI_td_traffic_create (
        pdu_ses_traf_reqs ? pdu_ses_traf_reqsList : NULL,
        ul_vol ? true : false,
        ul_vol ? ul_vol->valuedouble : 0,
        dl_vol ? true : false,
        dl_vol ? dl_vol->valuedouble : 0,
        all_vol ? true : false,
        all_vol ? all_vol->valuedouble : 0,
        ul_num_of_pkt ? true : false,
        ul_num_of_pkt ? ul_num_of_pkt->valuedouble : 0,
        dl_num_of_pkt ? true : false,
        dl_num_of_pkt ? dl_num_of_pkt->valuedouble : 0,
        all_num_of_pkt ? true : false,
        all_num_of_pkt ? all_num_of_pkt->valuedouble : 0
    );

    return td_traffic_local_var;
end:
    if (pdu_ses_traf_reqsList) {
        OpenAPI_list_for_each(pdu_ses_traf_reqsList, node) {
            OpenAPI_pdu_ses_traffic_req_free(node->data);
        }
        OpenAPI_list_free(pdu_ses_traf_reqsList);
        pdu_ses_traf_reqsList = NULL;
    }
    return NULL;
}

OpenAPI_td_traffic_t *OpenAPI_td_traffic_copy(OpenAPI_td_traffic_t *dst, OpenAPI_td_traffic_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_td_traffic_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_td_traffic_convertToJSON() failed");
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

    OpenAPI_td_traffic_free(dst);
    dst = OpenAPI_td_traffic_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

