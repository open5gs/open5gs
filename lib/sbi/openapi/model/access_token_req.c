
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_token_req.h"

char *OpenAPI_grant_typeaccess_token_req_ToString(OpenAPI_access_token_req_grant_type_e grant_type)
{
    const char *grant_typeArray[] =  { "NULL", "client_credentials" };
    size_t sizeofArray = sizeof(grant_typeArray) / sizeof(grant_typeArray[0]);
    if (grant_type < sizeofArray)
        return (char *)grant_typeArray[grant_type];
    else
        return (char *)"Unknown";
}

OpenAPI_access_token_req_grant_type_e OpenAPI_grant_typeaccess_token_req_FromString(char* grant_type)
{
    int stringToReturn = 0;
    const char *grant_typeArray[] =  { "NULL", "client_credentials" };
    size_t sizeofArray = sizeof(grant_typeArray) / sizeof(grant_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(grant_type, grant_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_access_token_req_t *OpenAPI_access_token_req_create(
    OpenAPI_access_token_req_grant_type_e grant_type,
    char *nf_instance_id,
    OpenAPI_nf_type_e nf_type,
    OpenAPI_nf_type_e target_nf_type,
    char *scope,
    char *target_nf_instance_id,
    OpenAPI_plmn_id_t *requester_plmn,
    OpenAPI_list_t *requester_plmn_list,
    OpenAPI_list_t *requester_snssai_list,
    char *requester_fqdn,
    OpenAPI_list_t *requester_snpn_list,
    OpenAPI_plmn_id_t *target_plmn,
    OpenAPI_plmn_id_nid_t *target_snpn,
    OpenAPI_list_t *target_snssai_list,
    OpenAPI_list_t *target_nsi_list,
    char *target_nf_set_id,
    char *target_nf_service_set_id,
    char *hnrf_access_token_uri,
    char *source_nf_instance_id
)
{
    OpenAPI_access_token_req_t *access_token_req_local_var = ogs_malloc(sizeof(OpenAPI_access_token_req_t));
    ogs_assert(access_token_req_local_var);

    access_token_req_local_var->grant_type = grant_type;
    access_token_req_local_var->nf_instance_id = nf_instance_id;
    access_token_req_local_var->nf_type = nf_type;
    access_token_req_local_var->target_nf_type = target_nf_type;
    access_token_req_local_var->scope = scope;
    access_token_req_local_var->target_nf_instance_id = target_nf_instance_id;
    access_token_req_local_var->requester_plmn = requester_plmn;
    access_token_req_local_var->requester_plmn_list = requester_plmn_list;
    access_token_req_local_var->requester_snssai_list = requester_snssai_list;
    access_token_req_local_var->requester_fqdn = requester_fqdn;
    access_token_req_local_var->requester_snpn_list = requester_snpn_list;
    access_token_req_local_var->target_plmn = target_plmn;
    access_token_req_local_var->target_snpn = target_snpn;
    access_token_req_local_var->target_snssai_list = target_snssai_list;
    access_token_req_local_var->target_nsi_list = target_nsi_list;
    access_token_req_local_var->target_nf_set_id = target_nf_set_id;
    access_token_req_local_var->target_nf_service_set_id = target_nf_service_set_id;
    access_token_req_local_var->hnrf_access_token_uri = hnrf_access_token_uri;
    access_token_req_local_var->source_nf_instance_id = source_nf_instance_id;

    return access_token_req_local_var;
}

void OpenAPI_access_token_req_free(OpenAPI_access_token_req_t *access_token_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_token_req) {
        return;
    }
    if (access_token_req->nf_instance_id) {
        ogs_free(access_token_req->nf_instance_id);
        access_token_req->nf_instance_id = NULL;
    }
    if (access_token_req->scope) {
        ogs_free(access_token_req->scope);
        access_token_req->scope = NULL;
    }
    if (access_token_req->target_nf_instance_id) {
        ogs_free(access_token_req->target_nf_instance_id);
        access_token_req->target_nf_instance_id = NULL;
    }
    if (access_token_req->requester_plmn) {
        OpenAPI_plmn_id_free(access_token_req->requester_plmn);
        access_token_req->requester_plmn = NULL;
    }
    if (access_token_req->requester_plmn_list) {
        OpenAPI_list_for_each(access_token_req->requester_plmn_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(access_token_req->requester_plmn_list);
        access_token_req->requester_plmn_list = NULL;
    }
    if (access_token_req->requester_snssai_list) {
        OpenAPI_list_for_each(access_token_req->requester_snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(access_token_req->requester_snssai_list);
        access_token_req->requester_snssai_list = NULL;
    }
    if (access_token_req->requester_fqdn) {
        ogs_free(access_token_req->requester_fqdn);
        access_token_req->requester_fqdn = NULL;
    }
    if (access_token_req->requester_snpn_list) {
        OpenAPI_list_for_each(access_token_req->requester_snpn_list, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(access_token_req->requester_snpn_list);
        access_token_req->requester_snpn_list = NULL;
    }
    if (access_token_req->target_plmn) {
        OpenAPI_plmn_id_free(access_token_req->target_plmn);
        access_token_req->target_plmn = NULL;
    }
    if (access_token_req->target_snpn) {
        OpenAPI_plmn_id_nid_free(access_token_req->target_snpn);
        access_token_req->target_snpn = NULL;
    }
    if (access_token_req->target_snssai_list) {
        OpenAPI_list_for_each(access_token_req->target_snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(access_token_req->target_snssai_list);
        access_token_req->target_snssai_list = NULL;
    }
    if (access_token_req->target_nsi_list) {
        OpenAPI_list_for_each(access_token_req->target_nsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_token_req->target_nsi_list);
        access_token_req->target_nsi_list = NULL;
    }
    if (access_token_req->target_nf_set_id) {
        ogs_free(access_token_req->target_nf_set_id);
        access_token_req->target_nf_set_id = NULL;
    }
    if (access_token_req->target_nf_service_set_id) {
        ogs_free(access_token_req->target_nf_service_set_id);
        access_token_req->target_nf_service_set_id = NULL;
    }
    if (access_token_req->hnrf_access_token_uri) {
        ogs_free(access_token_req->hnrf_access_token_uri);
        access_token_req->hnrf_access_token_uri = NULL;
    }
    if (access_token_req->source_nf_instance_id) {
        ogs_free(access_token_req->source_nf_instance_id);
        access_token_req->source_nf_instance_id = NULL;
    }
    ogs_free(access_token_req);
}

cJSON *OpenAPI_access_token_req_convertToJSON(OpenAPI_access_token_req_t *access_token_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_token_req == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [AccessTokenReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (access_token_req->grant_type == OpenAPI_access_token_req_GRANTTYPE_NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [grant_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "grant_type", OpenAPI_grant_typeaccess_token_req_ToString(access_token_req->grant_type)) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [grant_type]");
        goto end;
    }

    if (!access_token_req->nf_instance_id) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [nf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", access_token_req->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (access_token_req->nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_type_ToString(access_token_req->nf_type)) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [nf_type]");
        goto end;
    }
    }

    if (access_token_req->target_nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "targetNfType", OpenAPI_nf_type_ToString(access_token_req->target_nf_type)) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_nf_type]");
        goto end;
    }
    }

    if (!access_token_req->scope) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [scope]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "scope", access_token_req->scope) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [scope]");
        goto end;
    }

    if (access_token_req->target_nf_instance_id) {
    if (cJSON_AddStringToObject(item, "targetNfInstanceId", access_token_req->target_nf_instance_id) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_nf_instance_id]");
        goto end;
    }
    }

    if (access_token_req->requester_plmn) {
    cJSON *requester_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(access_token_req->requester_plmn);
    if (requester_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "requesterPlmn", requester_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_plmn]");
        goto end;
    }
    }

    if (access_token_req->requester_plmn_list) {
    cJSON *requester_plmn_listList = cJSON_AddArrayToObject(item, "requesterPlmnList");
    if (requester_plmn_listList == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_plmn_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_token_req->requester_plmn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_plmn_list]");
            goto end;
        }
        cJSON_AddItemToArray(requester_plmn_listList, itemLocal);
    }
    }

    if (access_token_req->requester_snssai_list) {
    cJSON *requester_snssai_listList = cJSON_AddArrayToObject(item, "requesterSnssaiList");
    if (requester_snssai_listList == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_token_req->requester_snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(requester_snssai_listList, itemLocal);
    }
    }

    if (access_token_req->requester_fqdn) {
    if (cJSON_AddStringToObject(item, "requesterFqdn", access_token_req->requester_fqdn) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_fqdn]");
        goto end;
    }
    }

    if (access_token_req->requester_snpn_list) {
    cJSON *requester_snpn_listList = cJSON_AddArrayToObject(item, "requesterSnpnList");
    if (requester_snpn_listList == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_snpn_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_token_req->requester_snpn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_token_req_convertToJSON() failed [requester_snpn_list]");
            goto end;
        }
        cJSON_AddItemToArray(requester_snpn_listList, itemLocal);
    }
    }

    if (access_token_req->target_plmn) {
    cJSON *target_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(access_token_req->target_plmn);
    if (target_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetPlmn", target_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_plmn]");
        goto end;
    }
    }

    if (access_token_req->target_snpn) {
    cJSON *target_snpn_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(access_token_req->target_snpn);
    if (target_snpn_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_snpn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetSnpn", target_snpn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_snpn]");
        goto end;
    }
    }

    if (access_token_req->target_snssai_list) {
    cJSON *target_snssai_listList = cJSON_AddArrayToObject(item, "targetSnssaiList");
    if (target_snssai_listList == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_token_req->target_snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(target_snssai_listList, itemLocal);
    }
    }

    if (access_token_req->target_nsi_list) {
    cJSON *target_nsi_listList = cJSON_AddArrayToObject(item, "targetNsiList");
    if (target_nsi_listList == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_nsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_token_req->target_nsi_list, node) {
        if (cJSON_AddStringToObject(target_nsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_nsi_list]");
            goto end;
        }
    }
    }

    if (access_token_req->target_nf_set_id) {
    if (cJSON_AddStringToObject(item, "targetNfSetId", access_token_req->target_nf_set_id) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_nf_set_id]");
        goto end;
    }
    }

    if (access_token_req->target_nf_service_set_id) {
    if (cJSON_AddStringToObject(item, "targetNfServiceSetId", access_token_req->target_nf_service_set_id) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [target_nf_service_set_id]");
        goto end;
    }
    }

    if (access_token_req->hnrf_access_token_uri) {
    if (cJSON_AddStringToObject(item, "hnrfAccessTokenUri", access_token_req->hnrf_access_token_uri) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [hnrf_access_token_uri]");
        goto end;
    }
    }

    if (access_token_req->source_nf_instance_id) {
    if (cJSON_AddStringToObject(item, "sourceNfInstanceId", access_token_req->source_nf_instance_id) == NULL) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed [source_nf_instance_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_access_token_req_t *OpenAPI_access_token_req_parseFromJSON(cJSON *access_token_reqJSON)
{
    OpenAPI_access_token_req_t *access_token_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *grant_type = NULL;
    OpenAPI_access_token_req_grant_type_e grant_typeVariable = 0;
    cJSON *nf_instance_id = NULL;
    cJSON *nf_type = NULL;
    OpenAPI_nf_type_e nf_typeVariable = 0;
    cJSON *target_nf_type = NULL;
    OpenAPI_nf_type_e target_nf_typeVariable = 0;
    cJSON *scope = NULL;
    cJSON *target_nf_instance_id = NULL;
    cJSON *requester_plmn = NULL;
    OpenAPI_plmn_id_t *requester_plmn_local_nonprim = NULL;
    cJSON *requester_plmn_list = NULL;
    OpenAPI_list_t *requester_plmn_listList = NULL;
    cJSON *requester_snssai_list = NULL;
    OpenAPI_list_t *requester_snssai_listList = NULL;
    cJSON *requester_fqdn = NULL;
    cJSON *requester_snpn_list = NULL;
    OpenAPI_list_t *requester_snpn_listList = NULL;
    cJSON *target_plmn = NULL;
    OpenAPI_plmn_id_t *target_plmn_local_nonprim = NULL;
    cJSON *target_snpn = NULL;
    OpenAPI_plmn_id_nid_t *target_snpn_local_nonprim = NULL;
    cJSON *target_snssai_list = NULL;
    OpenAPI_list_t *target_snssai_listList = NULL;
    cJSON *target_nsi_list = NULL;
    OpenAPI_list_t *target_nsi_listList = NULL;
    cJSON *target_nf_set_id = NULL;
    cJSON *target_nf_service_set_id = NULL;
    cJSON *hnrf_access_token_uri = NULL;
    cJSON *source_nf_instance_id = NULL;
    grant_type = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "grant_type");
    if (!grant_type) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [grant_type]");
        goto end;
    }
    if (!cJSON_IsString(grant_type)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [grant_type]");
        goto end;
    }
    grant_typeVariable = OpenAPI_grant_typeaccess_token_req_FromString(grant_type->valuestring);

    nf_instance_id = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    nf_type = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "nfType");
    if (nf_type) {
    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_nf_type_FromString(nf_type->valuestring);
    }

    target_nf_type = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetNfType");
    if (target_nf_type) {
    if (!cJSON_IsString(target_nf_type)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_nf_type]");
        goto end;
    }
    target_nf_typeVariable = OpenAPI_nf_type_FromString(target_nf_type->valuestring);
    }

    scope = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "scope");
    if (!scope) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [scope]");
        goto end;
    }
    if (!cJSON_IsString(scope)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [scope]");
        goto end;
    }

    target_nf_instance_id = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetNfInstanceId");
    if (target_nf_instance_id) {
    if (!cJSON_IsString(target_nf_instance_id) && !cJSON_IsNull(target_nf_instance_id)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_nf_instance_id]");
        goto end;
    }
    }

    requester_plmn = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "requesterPlmn");
    if (requester_plmn) {
    requester_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(requester_plmn);
    if (!requester_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [requester_plmn]");
        goto end;
    }
    }

    requester_plmn_list = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "requesterPlmnList");
    if (requester_plmn_list) {
        cJSON *requester_plmn_list_local = NULL;
        if (!cJSON_IsArray(requester_plmn_list)) {
            ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [requester_plmn_list]");
            goto end;
        }

        requester_plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(requester_plmn_list_local, requester_plmn_list) {
            if (!cJSON_IsObject(requester_plmn_list_local)) {
                ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [requester_plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *requester_plmn_listItem = OpenAPI_plmn_id_parseFromJSON(requester_plmn_list_local);
            if (!requester_plmn_listItem) {
                ogs_error("No requester_plmn_listItem");
                goto end;
            }
            OpenAPI_list_add(requester_plmn_listList, requester_plmn_listItem);
        }
    }

    requester_snssai_list = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "requesterSnssaiList");
    if (requester_snssai_list) {
        cJSON *requester_snssai_list_local = NULL;
        if (!cJSON_IsArray(requester_snssai_list)) {
            ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [requester_snssai_list]");
            goto end;
        }

        requester_snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(requester_snssai_list_local, requester_snssai_list) {
            if (!cJSON_IsObject(requester_snssai_list_local)) {
                ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [requester_snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *requester_snssai_listItem = OpenAPI_snssai_parseFromJSON(requester_snssai_list_local);
            if (!requester_snssai_listItem) {
                ogs_error("No requester_snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(requester_snssai_listList, requester_snssai_listItem);
        }
    }

    requester_fqdn = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "requesterFqdn");
    if (requester_fqdn) {
    if (!cJSON_IsString(requester_fqdn) && !cJSON_IsNull(requester_fqdn)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [requester_fqdn]");
        goto end;
    }
    }

    requester_snpn_list = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "requesterSnpnList");
    if (requester_snpn_list) {
        cJSON *requester_snpn_list_local = NULL;
        if (!cJSON_IsArray(requester_snpn_list)) {
            ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [requester_snpn_list]");
            goto end;
        }

        requester_snpn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(requester_snpn_list_local, requester_snpn_list) {
            if (!cJSON_IsObject(requester_snpn_list_local)) {
                ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [requester_snpn_list]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *requester_snpn_listItem = OpenAPI_plmn_id_nid_parseFromJSON(requester_snpn_list_local);
            if (!requester_snpn_listItem) {
                ogs_error("No requester_snpn_listItem");
                goto end;
            }
            OpenAPI_list_add(requester_snpn_listList, requester_snpn_listItem);
        }
    }

    target_plmn = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetPlmn");
    if (target_plmn) {
    target_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(target_plmn);
    if (!target_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [target_plmn]");
        goto end;
    }
    }

    target_snpn = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetSnpn");
    if (target_snpn) {
    target_snpn_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(target_snpn);
    if (!target_snpn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [target_snpn]");
        goto end;
    }
    }

    target_snssai_list = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetSnssaiList");
    if (target_snssai_list) {
        cJSON *target_snssai_list_local = NULL;
        if (!cJSON_IsArray(target_snssai_list)) {
            ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_snssai_list]");
            goto end;
        }

        target_snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(target_snssai_list_local, target_snssai_list) {
            if (!cJSON_IsObject(target_snssai_list_local)) {
                ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *target_snssai_listItem = OpenAPI_snssai_parseFromJSON(target_snssai_list_local);
            if (!target_snssai_listItem) {
                ogs_error("No target_snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(target_snssai_listList, target_snssai_listItem);
        }
    }

    target_nsi_list = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetNsiList");
    if (target_nsi_list) {
        cJSON *target_nsi_list_local = NULL;
        if (!cJSON_IsArray(target_nsi_list)) {
            ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_nsi_list]");
            goto end;
        }

        target_nsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(target_nsi_list_local, target_nsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(target_nsi_list_local)) {
                ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_nsi_list]");
                goto end;
            }
            OpenAPI_list_add(target_nsi_listList, ogs_strdup(target_nsi_list_local->valuestring));
        }
    }

    target_nf_set_id = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetNfSetId");
    if (target_nf_set_id) {
    if (!cJSON_IsString(target_nf_set_id) && !cJSON_IsNull(target_nf_set_id)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_nf_set_id]");
        goto end;
    }
    }

    target_nf_service_set_id = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "targetNfServiceSetId");
    if (target_nf_service_set_id) {
    if (!cJSON_IsString(target_nf_service_set_id) && !cJSON_IsNull(target_nf_service_set_id)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [target_nf_service_set_id]");
        goto end;
    }
    }

    hnrf_access_token_uri = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "hnrfAccessTokenUri");
    if (hnrf_access_token_uri) {
    if (!cJSON_IsString(hnrf_access_token_uri) && !cJSON_IsNull(hnrf_access_token_uri)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [hnrf_access_token_uri]");
        goto end;
    }
    }

    source_nf_instance_id = cJSON_GetObjectItemCaseSensitive(access_token_reqJSON, "sourceNfInstanceId");
    if (source_nf_instance_id) {
    if (!cJSON_IsString(source_nf_instance_id) && !cJSON_IsNull(source_nf_instance_id)) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON() failed [source_nf_instance_id]");
        goto end;
    }
    }

    access_token_req_local_var = OpenAPI_access_token_req_create (
        grant_typeVariable,
        ogs_strdup(nf_instance_id->valuestring),
        nf_type ? nf_typeVariable : 0,
        target_nf_type ? target_nf_typeVariable : 0,
        ogs_strdup(scope->valuestring),
        target_nf_instance_id && !cJSON_IsNull(target_nf_instance_id) ? ogs_strdup(target_nf_instance_id->valuestring) : NULL,
        requester_plmn ? requester_plmn_local_nonprim : NULL,
        requester_plmn_list ? requester_plmn_listList : NULL,
        requester_snssai_list ? requester_snssai_listList : NULL,
        requester_fqdn && !cJSON_IsNull(requester_fqdn) ? ogs_strdup(requester_fqdn->valuestring) : NULL,
        requester_snpn_list ? requester_snpn_listList : NULL,
        target_plmn ? target_plmn_local_nonprim : NULL,
        target_snpn ? target_snpn_local_nonprim : NULL,
        target_snssai_list ? target_snssai_listList : NULL,
        target_nsi_list ? target_nsi_listList : NULL,
        target_nf_set_id && !cJSON_IsNull(target_nf_set_id) ? ogs_strdup(target_nf_set_id->valuestring) : NULL,
        target_nf_service_set_id && !cJSON_IsNull(target_nf_service_set_id) ? ogs_strdup(target_nf_service_set_id->valuestring) : NULL,
        hnrf_access_token_uri && !cJSON_IsNull(hnrf_access_token_uri) ? ogs_strdup(hnrf_access_token_uri->valuestring) : NULL,
        source_nf_instance_id && !cJSON_IsNull(source_nf_instance_id) ? ogs_strdup(source_nf_instance_id->valuestring) : NULL
    );

    return access_token_req_local_var;
end:
    if (requester_plmn_local_nonprim) {
        OpenAPI_plmn_id_free(requester_plmn_local_nonprim);
        requester_plmn_local_nonprim = NULL;
    }
    if (requester_plmn_listList) {
        OpenAPI_list_for_each(requester_plmn_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(requester_plmn_listList);
        requester_plmn_listList = NULL;
    }
    if (requester_snssai_listList) {
        OpenAPI_list_for_each(requester_snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(requester_snssai_listList);
        requester_snssai_listList = NULL;
    }
    if (requester_snpn_listList) {
        OpenAPI_list_for_each(requester_snpn_listList, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(requester_snpn_listList);
        requester_snpn_listList = NULL;
    }
    if (target_plmn_local_nonprim) {
        OpenAPI_plmn_id_free(target_plmn_local_nonprim);
        target_plmn_local_nonprim = NULL;
    }
    if (target_snpn_local_nonprim) {
        OpenAPI_plmn_id_nid_free(target_snpn_local_nonprim);
        target_snpn_local_nonprim = NULL;
    }
    if (target_snssai_listList) {
        OpenAPI_list_for_each(target_snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(target_snssai_listList);
        target_snssai_listList = NULL;
    }
    if (target_nsi_listList) {
        OpenAPI_list_for_each(target_nsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(target_nsi_listList);
        target_nsi_listList = NULL;
    }
    return NULL;
}

OpenAPI_access_token_req_t *OpenAPI_access_token_req_copy(OpenAPI_access_token_req_t *dst, OpenAPI_access_token_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_token_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_token_req_convertToJSON() failed");
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

    OpenAPI_access_token_req_free(dst);
    dst = OpenAPI_access_token_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

