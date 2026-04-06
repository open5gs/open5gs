
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_para.h"

OpenAPI_qos_para_t *OpenAPI_qos_para_create(
    char *qos_param_set_id,
    bool is__5qi,
    int _5qi,
    bool is_priority_lvl,
    int priority_lvl,
    OpenAPI_qos_resource_type_e rsc_type,
    bool is_pdb,
    int pdb,
    char *per,
    char *gbr_ul,
    char *gbr_dl,
    char *mbr_ul,
    char *mbr_dl,
    bool is_max_plr_ul,
    int max_plr_ul,
    bool is_max_plr_dl,
    int max_plr_dl,
    bool is_avg_win,
    int avg_win,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol
)
{
    OpenAPI_qos_para_t *qos_para_local_var = ogs_malloc(sizeof(OpenAPI_qos_para_t));
    ogs_assert(qos_para_local_var);

    qos_para_local_var->qos_param_set_id = qos_param_set_id;
    qos_para_local_var->is__5qi = is__5qi;
    qos_para_local_var->_5qi = _5qi;
    qos_para_local_var->is_priority_lvl = is_priority_lvl;
    qos_para_local_var->priority_lvl = priority_lvl;
    qos_para_local_var->rsc_type = rsc_type;
    qos_para_local_var->is_pdb = is_pdb;
    qos_para_local_var->pdb = pdb;
    qos_para_local_var->per = per;
    qos_para_local_var->gbr_ul = gbr_ul;
    qos_para_local_var->gbr_dl = gbr_dl;
    qos_para_local_var->mbr_ul = mbr_ul;
    qos_para_local_var->mbr_dl = mbr_dl;
    qos_para_local_var->is_max_plr_ul = is_max_plr_ul;
    qos_para_local_var->max_plr_ul = max_plr_ul;
    qos_para_local_var->is_max_plr_dl = is_max_plr_dl;
    qos_para_local_var->max_plr_dl = max_plr_dl;
    qos_para_local_var->is_avg_win = is_avg_win;
    qos_para_local_var->avg_win = avg_win;
    qos_para_local_var->is_max_data_burst_vol = is_max_data_burst_vol;
    qos_para_local_var->max_data_burst_vol = max_data_burst_vol;
    qos_para_local_var->is_ext_max_data_burst_vol = is_ext_max_data_burst_vol;
    qos_para_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;

    return qos_para_local_var;
}

void OpenAPI_qos_para_free(OpenAPI_qos_para_t *qos_para)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_para) {
        return;
    }
    if (qos_para->qos_param_set_id) {
        ogs_free(qos_para->qos_param_set_id);
        qos_para->qos_param_set_id = NULL;
    }
    if (qos_para->per) {
        ogs_free(qos_para->per);
        qos_para->per = NULL;
    }
    if (qos_para->gbr_ul) {
        ogs_free(qos_para->gbr_ul);
        qos_para->gbr_ul = NULL;
    }
    if (qos_para->gbr_dl) {
        ogs_free(qos_para->gbr_dl);
        qos_para->gbr_dl = NULL;
    }
    if (qos_para->mbr_ul) {
        ogs_free(qos_para->mbr_ul);
        qos_para->mbr_ul = NULL;
    }
    if (qos_para->mbr_dl) {
        ogs_free(qos_para->mbr_dl);
        qos_para->mbr_dl = NULL;
    }
    ogs_free(qos_para);
}

cJSON *OpenAPI_qos_para_convertToJSON(OpenAPI_qos_para_t *qos_para)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_para == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [QosPara]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_para->qos_param_set_id) {
    if (cJSON_AddStringToObject(item, "qosParamSetId", qos_para->qos_param_set_id) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [qos_param_set_id]");
        goto end;
    }
    }

    if (qos_para->is__5qi) {
    if (cJSON_AddNumberToObject(item, "5qi", qos_para->_5qi) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [_5qi]");
        goto end;
    }
    }

    if (qos_para->is_priority_lvl) {
    if (cJSON_AddNumberToObject(item, "priorityLvl", qos_para->priority_lvl) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [priority_lvl]");
        goto end;
    }
    }

    if (qos_para->rsc_type != OpenAPI_qos_resource_type_NULL) {
    if (cJSON_AddStringToObject(item, "rscType", OpenAPI_qos_resource_type_ToString(qos_para->rsc_type)) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [rsc_type]");
        goto end;
    }
    }

    if (qos_para->is_pdb) {
    if (cJSON_AddNumberToObject(item, "pdb", qos_para->pdb) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [pdb]");
        goto end;
    }
    }

    if (qos_para->per) {
    if (cJSON_AddStringToObject(item, "per", qos_para->per) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [per]");
        goto end;
    }
    }

    if (qos_para->gbr_ul) {
    if (cJSON_AddStringToObject(item, "gbrUl", qos_para->gbr_ul) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [gbr_ul]");
        goto end;
    }
    }

    if (qos_para->gbr_dl) {
    if (cJSON_AddStringToObject(item, "gbrDl", qos_para->gbr_dl) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [gbr_dl]");
        goto end;
    }
    }

    if (qos_para->mbr_ul) {
    if (cJSON_AddStringToObject(item, "mbrUl", qos_para->mbr_ul) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [mbr_ul]");
        goto end;
    }
    }

    if (qos_para->mbr_dl) {
    if (cJSON_AddStringToObject(item, "mbrDl", qos_para->mbr_dl) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [mbr_dl]");
        goto end;
    }
    }

    if (qos_para->is_max_plr_ul) {
    if (cJSON_AddNumberToObject(item, "maxPlrUl", qos_para->max_plr_ul) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [max_plr_ul]");
        goto end;
    }
    }

    if (qos_para->is_max_plr_dl) {
    if (cJSON_AddNumberToObject(item, "maxPlrDl", qos_para->max_plr_dl) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [max_plr_dl]");
        goto end;
    }
    }

    if (qos_para->is_avg_win) {
    if (cJSON_AddNumberToObject(item, "avgWin", qos_para->avg_win) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [avg_win]");
        goto end;
    }
    }

    if (qos_para->is_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "maxDataBurstVol", qos_para->max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    if (qos_para->is_ext_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", qos_para->ext_max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_para_t *OpenAPI_qos_para_parseFromJSON(cJSON *qos_paraJSON)
{
    OpenAPI_qos_para_t *qos_para_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qos_param_set_id = NULL;
    cJSON *_5qi = NULL;
    cJSON *priority_lvl = NULL;
    cJSON *rsc_type = NULL;
    OpenAPI_qos_resource_type_e rsc_typeVariable = 0;
    cJSON *pdb = NULL;
    cJSON *per = NULL;
    cJSON *gbr_ul = NULL;
    cJSON *gbr_dl = NULL;
    cJSON *mbr_ul = NULL;
    cJSON *mbr_dl = NULL;
    cJSON *max_plr_ul = NULL;
    cJSON *max_plr_dl = NULL;
    cJSON *avg_win = NULL;
    cJSON *max_data_burst_vol = NULL;
    cJSON *ext_max_data_burst_vol = NULL;
    qos_param_set_id = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "qosParamSetId");
    if (qos_param_set_id) {
    if (!cJSON_IsString(qos_param_set_id) && !cJSON_IsNull(qos_param_set_id)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [qos_param_set_id]");
        goto end;
    }
    }

    _5qi = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "5qi");
    if (_5qi) {
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [_5qi]");
        goto end;
    }
    }

    priority_lvl = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "priorityLvl");
    if (priority_lvl) {
    if (!cJSON_IsNumber(priority_lvl)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [priority_lvl]");
        goto end;
    }
    }

    rsc_type = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "rscType");
    if (rsc_type) {
    if (!cJSON_IsString(rsc_type)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [rsc_type]");
        goto end;
    }
    rsc_typeVariable = OpenAPI_qos_resource_type_FromString(rsc_type->valuestring);
    }

    pdb = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "pdb");
    if (pdb) {
    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [pdb]");
        goto end;
    }
    }

    per = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "per");
    if (per) {
    if (!cJSON_IsString(per) && !cJSON_IsNull(per)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [per]");
        goto end;
    }
    }

    gbr_ul = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "gbrUl");
    if (gbr_ul) {
    if (!cJSON_IsString(gbr_ul) && !cJSON_IsNull(gbr_ul)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [gbr_ul]");
        goto end;
    }
    }

    gbr_dl = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "gbrDl");
    if (gbr_dl) {
    if (!cJSON_IsString(gbr_dl) && !cJSON_IsNull(gbr_dl)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [gbr_dl]");
        goto end;
    }
    }

    mbr_ul = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "mbrUl");
    if (mbr_ul) {
    if (!cJSON_IsString(mbr_ul) && !cJSON_IsNull(mbr_ul)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [mbr_ul]");
        goto end;
    }
    }

    mbr_dl = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "mbrDl");
    if (mbr_dl) {
    if (!cJSON_IsString(mbr_dl) && !cJSON_IsNull(mbr_dl)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [mbr_dl]");
        goto end;
    }
    }

    max_plr_ul = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "maxPlrUl");
    if (max_plr_ul) {
    if (!cJSON_IsNumber(max_plr_ul)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [max_plr_ul]");
        goto end;
    }
    }

    max_plr_dl = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "maxPlrDl");
    if (max_plr_dl) {
    if (!cJSON_IsNumber(max_plr_dl)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [max_plr_dl]");
        goto end;
    }
    }

    avg_win = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "avgWin");
    if (avg_win) {
    if (!cJSON_IsNumber(avg_win)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [avg_win]");
        goto end;
    }
    }

    max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "maxDataBurstVol");
    if (max_data_burst_vol) {
    if (!cJSON_IsNumber(max_data_burst_vol)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(qos_paraJSON, "extMaxDataBurstVol");
    if (ext_max_data_burst_vol) {
    if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
        ogs_error("OpenAPI_qos_para_parseFromJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

    qos_para_local_var = OpenAPI_qos_para_create (
        qos_param_set_id && !cJSON_IsNull(qos_param_set_id) ? ogs_strdup(qos_param_set_id->valuestring) : NULL,
        _5qi ? true : false,
        _5qi ? _5qi->valuedouble : 0,
        priority_lvl ? true : false,
        priority_lvl ? priority_lvl->valuedouble : 0,
        rsc_type ? rsc_typeVariable : 0,
        pdb ? true : false,
        pdb ? pdb->valuedouble : 0,
        per && !cJSON_IsNull(per) ? ogs_strdup(per->valuestring) : NULL,
        gbr_ul && !cJSON_IsNull(gbr_ul) ? ogs_strdup(gbr_ul->valuestring) : NULL,
        gbr_dl && !cJSON_IsNull(gbr_dl) ? ogs_strdup(gbr_dl->valuestring) : NULL,
        mbr_ul && !cJSON_IsNull(mbr_ul) ? ogs_strdup(mbr_ul->valuestring) : NULL,
        mbr_dl && !cJSON_IsNull(mbr_dl) ? ogs_strdup(mbr_dl->valuestring) : NULL,
        max_plr_ul ? true : false,
        max_plr_ul ? max_plr_ul->valuedouble : 0,
        max_plr_dl ? true : false,
        max_plr_dl ? max_plr_dl->valuedouble : 0,
        avg_win ? true : false,
        avg_win ? avg_win->valuedouble : 0,
        max_data_burst_vol ? true : false,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        ext_max_data_burst_vol ? true : false,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0
    );

    return qos_para_local_var;
end:
    return NULL;
}

OpenAPI_qos_para_t *OpenAPI_qos_para_copy(OpenAPI_qos_para_t *dst, OpenAPI_qos_para_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_para_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_para_convertToJSON() failed");
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

    OpenAPI_qos_para_free(dst);
    dst = OpenAPI_qos_para_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

