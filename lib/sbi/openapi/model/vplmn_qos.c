
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vplmn_qos.h"

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_create(
    bool is__5qi,
    int _5qi,
    OpenAPI_list_t *add5_qi_list,
    OpenAPI_arp_t *arp,
    OpenAPI_ambr_t *session_ambr,
    char *max_fbr_dl,
    char *max_fbr_ul,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    bool is__5qi_pl,
    int _5qi_pl
)
{
    OpenAPI_vplmn_qos_t *vplmn_qos_local_var = ogs_malloc(sizeof(OpenAPI_vplmn_qos_t));
    ogs_assert(vplmn_qos_local_var);

    vplmn_qos_local_var->is__5qi = is__5qi;
    vplmn_qos_local_var->_5qi = _5qi;
    vplmn_qos_local_var->add5_qi_list = add5_qi_list;
    vplmn_qos_local_var->arp = arp;
    vplmn_qos_local_var->session_ambr = session_ambr;
    vplmn_qos_local_var->max_fbr_dl = max_fbr_dl;
    vplmn_qos_local_var->max_fbr_ul = max_fbr_ul;
    vplmn_qos_local_var->gua_fbr_dl = gua_fbr_dl;
    vplmn_qos_local_var->gua_fbr_ul = gua_fbr_ul;
    vplmn_qos_local_var->is__5qi_pl = is__5qi_pl;
    vplmn_qos_local_var->_5qi_pl = _5qi_pl;

    return vplmn_qos_local_var;
}

void OpenAPI_vplmn_qos_free(OpenAPI_vplmn_qos_t *vplmn_qos)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vplmn_qos) {
        return;
    }
    if (vplmn_qos->add5_qi_list) {
        OpenAPI_list_for_each(vplmn_qos->add5_qi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(vplmn_qos->add5_qi_list);
        vplmn_qos->add5_qi_list = NULL;
    }
    if (vplmn_qos->arp) {
        OpenAPI_arp_free(vplmn_qos->arp);
        vplmn_qos->arp = NULL;
    }
    if (vplmn_qos->session_ambr) {
        OpenAPI_ambr_free(vplmn_qos->session_ambr);
        vplmn_qos->session_ambr = NULL;
    }
    if (vplmn_qos->max_fbr_dl) {
        ogs_free(vplmn_qos->max_fbr_dl);
        vplmn_qos->max_fbr_dl = NULL;
    }
    if (vplmn_qos->max_fbr_ul) {
        ogs_free(vplmn_qos->max_fbr_ul);
        vplmn_qos->max_fbr_ul = NULL;
    }
    if (vplmn_qos->gua_fbr_dl) {
        ogs_free(vplmn_qos->gua_fbr_dl);
        vplmn_qos->gua_fbr_dl = NULL;
    }
    if (vplmn_qos->gua_fbr_ul) {
        ogs_free(vplmn_qos->gua_fbr_ul);
        vplmn_qos->gua_fbr_ul = NULL;
    }
    ogs_free(vplmn_qos);
}

cJSON *OpenAPI_vplmn_qos_convertToJSON(OpenAPI_vplmn_qos_t *vplmn_qos)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vplmn_qos == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [VplmnQos]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vplmn_qos->is__5qi) {
    if (cJSON_AddNumberToObject(item, "5qi", vplmn_qos->_5qi) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [_5qi]");
        goto end;
    }
    }

    if (vplmn_qos->add5_qi_list) {
    cJSON *add5_qi_listList = cJSON_AddArrayToObject(item, "add5QiList");
    if (add5_qi_listList == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [add5_qi_list]");
        goto end;
    }
    OpenAPI_list_for_each(vplmn_qos->add5_qi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [add5_qi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(add5_qi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [add5_qi_list]");
            goto end;
        }
    }
    }

    if (vplmn_qos->arp) {
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(vplmn_qos->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [arp]");
        goto end;
    }
    }

    if (vplmn_qos->session_ambr) {
    cJSON *session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(vplmn_qos->session_ambr);
    if (session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessionAmbr", session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [session_ambr]");
        goto end;
    }
    }

    if (vplmn_qos->max_fbr_dl) {
    if (cJSON_AddStringToObject(item, "maxFbrDl", vplmn_qos->max_fbr_dl) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [max_fbr_dl]");
        goto end;
    }
    }

    if (vplmn_qos->max_fbr_ul) {
    if (cJSON_AddStringToObject(item, "maxFbrUl", vplmn_qos->max_fbr_ul) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [max_fbr_ul]");
        goto end;
    }
    }

    if (vplmn_qos->gua_fbr_dl) {
    if (cJSON_AddStringToObject(item, "guaFbrDl", vplmn_qos->gua_fbr_dl) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    if (vplmn_qos->gua_fbr_ul) {
    if (cJSON_AddStringToObject(item, "guaFbrUl", vplmn_qos->gua_fbr_ul) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

    if (vplmn_qos->is__5qi_pl) {
    if (cJSON_AddNumberToObject(item, "5qiPL", vplmn_qos->_5qi_pl) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [_5qi_pl]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_parseFromJSON(cJSON *vplmn_qosJSON)
{
    OpenAPI_vplmn_qos_t *vplmn_qos_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_5qi = NULL;
    cJSON *add5_qi_list = NULL;
    OpenAPI_list_t *add5_qi_listList = NULL;
    cJSON *arp = NULL;
    OpenAPI_arp_t *arp_local_nonprim = NULL;
    cJSON *session_ambr = NULL;
    OpenAPI_ambr_t *session_ambr_local_nonprim = NULL;
    cJSON *max_fbr_dl = NULL;
    cJSON *max_fbr_ul = NULL;
    cJSON *gua_fbr_dl = NULL;
    cJSON *gua_fbr_ul = NULL;
    cJSON *_5qi_pl = NULL;
    _5qi = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "5qi");
    if (_5qi) {
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [_5qi]");
        goto end;
    }
    }

    add5_qi_list = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "add5QiList");
    if (add5_qi_list) {
        cJSON *add5_qi_list_local = NULL;
        if (!cJSON_IsArray(add5_qi_list)) {
            ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [add5_qi_list]");
            goto end;
        }

        add5_qi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(add5_qi_list_local, add5_qi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(add5_qi_list_local)) {
                ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [add5_qi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [add5_qi_list]");
                goto end;
            }
            *localDouble = add5_qi_list_local->valuedouble;
            OpenAPI_list_add(add5_qi_listList, localDouble);
        }
    }

    arp = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "arp");
    if (arp) {
    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);
    if (!arp_local_nonprim) {
        ogs_error("OpenAPI_arp_parseFromJSON failed [arp]");
        goto end;
    }
    }

    session_ambr = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "sessionAmbr");
    if (session_ambr) {
    session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(session_ambr);
    if (!session_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [session_ambr]");
        goto end;
    }
    }

    max_fbr_dl = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "maxFbrDl");
    if (max_fbr_dl) {
    if (!cJSON_IsString(max_fbr_dl) && !cJSON_IsNull(max_fbr_dl)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [max_fbr_dl]");
        goto end;
    }
    }

    max_fbr_ul = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "maxFbrUl");
    if (max_fbr_ul) {
    if (!cJSON_IsString(max_fbr_ul) && !cJSON_IsNull(max_fbr_ul)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [max_fbr_ul]");
        goto end;
    }
    }

    gua_fbr_dl = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "guaFbrDl");
    if (gua_fbr_dl) {
    if (!cJSON_IsString(gua_fbr_dl) && !cJSON_IsNull(gua_fbr_dl)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    gua_fbr_ul = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "guaFbrUl");
    if (gua_fbr_ul) {
    if (!cJSON_IsString(gua_fbr_ul) && !cJSON_IsNull(gua_fbr_ul)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

    _5qi_pl = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "5qiPL");
    if (_5qi_pl) {
    if (!cJSON_IsNumber(_5qi_pl)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [_5qi_pl]");
        goto end;
    }
    }

    vplmn_qos_local_var = OpenAPI_vplmn_qos_create (
        _5qi ? true : false,
        _5qi ? _5qi->valuedouble : 0,
        add5_qi_list ? add5_qi_listList : NULL,
        arp ? arp_local_nonprim : NULL,
        session_ambr ? session_ambr_local_nonprim : NULL,
        max_fbr_dl && !cJSON_IsNull(max_fbr_dl) ? ogs_strdup(max_fbr_dl->valuestring) : NULL,
        max_fbr_ul && !cJSON_IsNull(max_fbr_ul) ? ogs_strdup(max_fbr_ul->valuestring) : NULL,
        gua_fbr_dl && !cJSON_IsNull(gua_fbr_dl) ? ogs_strdup(gua_fbr_dl->valuestring) : NULL,
        gua_fbr_ul && !cJSON_IsNull(gua_fbr_ul) ? ogs_strdup(gua_fbr_ul->valuestring) : NULL,
        _5qi_pl ? true : false,
        _5qi_pl ? _5qi_pl->valuedouble : 0
    );

    return vplmn_qos_local_var;
end:
    if (add5_qi_listList) {
        OpenAPI_list_for_each(add5_qi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(add5_qi_listList);
        add5_qi_listList = NULL;
    }
    if (arp_local_nonprim) {
        OpenAPI_arp_free(arp_local_nonprim);
        arp_local_nonprim = NULL;
    }
    if (session_ambr_local_nonprim) {
        OpenAPI_ambr_free(session_ambr_local_nonprim);
        session_ambr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_copy(OpenAPI_vplmn_qos_t *dst, OpenAPI_vplmn_qos_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vplmn_qos_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed");
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

    OpenAPI_vplmn_qos_free(dst);
    dst = OpenAPI_vplmn_qos_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

