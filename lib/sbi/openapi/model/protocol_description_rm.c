
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "protocol_description_rm.h"

OpenAPI_protocol_description_rm_t *OpenAPI_protocol_description_rm_create(
    OpenAPI_media_transport_proto_e transport_proto,
    bool is_rtp_header_ext_info_null,
    OpenAPI_rtp_header_ext_info_rm_t *rtp_header_ext_info,
    bool is_add_rtp_header_ext_info_null,
    OpenAPI_list_t *add_rtp_header_ext_info,
    bool is_rtp_payload_info_list_null,
    OpenAPI_list_t *rtp_payload_info_list,
    OpenAPI_mri_transfer_method_e mri_transfer_info
)
{
    OpenAPI_protocol_description_rm_t *protocol_description_rm_local_var = ogs_malloc(sizeof(OpenAPI_protocol_description_rm_t));
    ogs_assert(protocol_description_rm_local_var);

    protocol_description_rm_local_var->transport_proto = transport_proto;
    protocol_description_rm_local_var->is_rtp_header_ext_info_null = is_rtp_header_ext_info_null;
    protocol_description_rm_local_var->rtp_header_ext_info = rtp_header_ext_info;
    protocol_description_rm_local_var->is_add_rtp_header_ext_info_null = is_add_rtp_header_ext_info_null;
    protocol_description_rm_local_var->add_rtp_header_ext_info = add_rtp_header_ext_info;
    protocol_description_rm_local_var->is_rtp_payload_info_list_null = is_rtp_payload_info_list_null;
    protocol_description_rm_local_var->rtp_payload_info_list = rtp_payload_info_list;
    protocol_description_rm_local_var->mri_transfer_info = mri_transfer_info;

    return protocol_description_rm_local_var;
}

void OpenAPI_protocol_description_rm_free(OpenAPI_protocol_description_rm_t *protocol_description_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == protocol_description_rm) {
        return;
    }
    if (protocol_description_rm->rtp_header_ext_info) {
        OpenAPI_rtp_header_ext_info_rm_free(protocol_description_rm->rtp_header_ext_info);
        protocol_description_rm->rtp_header_ext_info = NULL;
    }
    if (protocol_description_rm->add_rtp_header_ext_info) {
        OpenAPI_list_for_each(protocol_description_rm->add_rtp_header_ext_info, node) {
            OpenAPI_rtp_header_ext_info_free(node->data);
        }
        OpenAPI_list_free(protocol_description_rm->add_rtp_header_ext_info);
        protocol_description_rm->add_rtp_header_ext_info = NULL;
    }
    if (protocol_description_rm->rtp_payload_info_list) {
        OpenAPI_list_for_each(protocol_description_rm->rtp_payload_info_list, node) {
            OpenAPI_rtp_payload_info_free(node->data);
        }
        OpenAPI_list_free(protocol_description_rm->rtp_payload_info_list);
        protocol_description_rm->rtp_payload_info_list = NULL;
    }
    ogs_free(protocol_description_rm);
}

cJSON *OpenAPI_protocol_description_rm_convertToJSON(OpenAPI_protocol_description_rm_t *protocol_description_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (protocol_description_rm == NULL) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [ProtocolDescriptionRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (protocol_description_rm->transport_proto != OpenAPI_media_transport_proto_NULL) {
    if (cJSON_AddStringToObject(item, "transportProto", OpenAPI_media_transport_proto_ToString(protocol_description_rm->transport_proto)) == NULL) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [transport_proto]");
        goto end;
    }
    }

    if (protocol_description_rm->rtp_header_ext_info) {
    cJSON *rtp_header_ext_info_local_JSON = OpenAPI_rtp_header_ext_info_rm_convertToJSON(protocol_description_rm->rtp_header_ext_info);
    if (rtp_header_ext_info_local_JSON == NULL) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [rtp_header_ext_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rtpHeaderExtInfo", rtp_header_ext_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [rtp_header_ext_info]");
        goto end;
    }
    } else if (protocol_description_rm->is_rtp_header_ext_info_null) {
        if (cJSON_AddNullToObject(item, "rtpHeaderExtInfo") == NULL) {
            ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [rtp_header_ext_info]");
            goto end;
        }
    }

    if (protocol_description_rm->add_rtp_header_ext_info) {
    cJSON *add_rtp_header_ext_infoList = cJSON_AddArrayToObject(item, "addRtpHeaderExtInfo");
    if (add_rtp_header_ext_infoList == NULL) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [add_rtp_header_ext_info]");
        goto end;
    }
    OpenAPI_list_for_each(protocol_description_rm->add_rtp_header_ext_info, node) {
        cJSON *itemLocal = OpenAPI_rtp_header_ext_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [add_rtp_header_ext_info]");
            goto end;
        }
        cJSON_AddItemToArray(add_rtp_header_ext_infoList, itemLocal);
    }
    } else if (protocol_description_rm->is_add_rtp_header_ext_info_null) {
        if (cJSON_AddNullToObject(item, "addRtpHeaderExtInfo") == NULL) {
            ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [add_rtp_header_ext_info]");
            goto end;
        }
    }

    if (protocol_description_rm->rtp_payload_info_list) {
    cJSON *rtp_payload_info_listList = cJSON_AddArrayToObject(item, "rtpPayloadInfoList");
    if (rtp_payload_info_listList == NULL) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [rtp_payload_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(protocol_description_rm->rtp_payload_info_list, node) {
        cJSON *itemLocal = OpenAPI_rtp_payload_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [rtp_payload_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(rtp_payload_info_listList, itemLocal);
    }
    } else if (protocol_description_rm->is_rtp_payload_info_list_null) {
        if (cJSON_AddNullToObject(item, "rtpPayloadInfoList") == NULL) {
            ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [rtp_payload_info_list]");
            goto end;
        }
    }

    if (protocol_description_rm->mri_transfer_info != OpenAPI_mri_transfer_method_NULL) {
    if (cJSON_AddStringToObject(item, "mriTransferInfo", OpenAPI_mri_transfer_method_ToString(protocol_description_rm->mri_transfer_info)) == NULL) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed [mri_transfer_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_protocol_description_rm_t *OpenAPI_protocol_description_rm_parseFromJSON(cJSON *protocol_description_rmJSON)
{
    OpenAPI_protocol_description_rm_t *protocol_description_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *transport_proto = NULL;
    OpenAPI_media_transport_proto_e transport_protoVariable = 0;
    cJSON *rtp_header_ext_info = NULL;
    OpenAPI_rtp_header_ext_info_rm_t *rtp_header_ext_info_local_nonprim = NULL;
    cJSON *add_rtp_header_ext_info = NULL;
    OpenAPI_list_t *add_rtp_header_ext_infoList = NULL;
    cJSON *rtp_payload_info_list = NULL;
    OpenAPI_list_t *rtp_payload_info_listList = NULL;
    cJSON *mri_transfer_info = NULL;
    OpenAPI_mri_transfer_method_e mri_transfer_infoVariable = 0;
    transport_proto = cJSON_GetObjectItemCaseSensitive(protocol_description_rmJSON, "transportProto");
    if (transport_proto) {
    if (!cJSON_IsString(transport_proto)) {
        ogs_error("OpenAPI_protocol_description_rm_parseFromJSON() failed [transport_proto]");
        goto end;
    }
    transport_protoVariable = OpenAPI_media_transport_proto_FromString(transport_proto->valuestring);
    }

    rtp_header_ext_info = cJSON_GetObjectItemCaseSensitive(protocol_description_rmJSON, "rtpHeaderExtInfo");
    if (rtp_header_ext_info) {
    if (!cJSON_IsNull(rtp_header_ext_info)) {
    rtp_header_ext_info_local_nonprim = OpenAPI_rtp_header_ext_info_rm_parseFromJSON(rtp_header_ext_info);
    if (!rtp_header_ext_info_local_nonprim) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_parseFromJSON failed [rtp_header_ext_info]");
        goto end;
    }
    }
    }

    add_rtp_header_ext_info = cJSON_GetObjectItemCaseSensitive(protocol_description_rmJSON, "addRtpHeaderExtInfo");
    if (add_rtp_header_ext_info) {
    if (!cJSON_IsNull(add_rtp_header_ext_info)) {
        cJSON *add_rtp_header_ext_info_local = NULL;
        if (!cJSON_IsArray(add_rtp_header_ext_info)) {
            ogs_error("OpenAPI_protocol_description_rm_parseFromJSON() failed [add_rtp_header_ext_info]");
            goto end;
        }

        add_rtp_header_ext_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_rtp_header_ext_info_local, add_rtp_header_ext_info) {
            if (!cJSON_IsObject(add_rtp_header_ext_info_local)) {
                ogs_error("OpenAPI_protocol_description_rm_parseFromJSON() failed [add_rtp_header_ext_info]");
                goto end;
            }
            OpenAPI_rtp_header_ext_info_t *add_rtp_header_ext_infoItem = OpenAPI_rtp_header_ext_info_parseFromJSON(add_rtp_header_ext_info_local);
            if (!add_rtp_header_ext_infoItem) {
                ogs_error("No add_rtp_header_ext_infoItem");
                goto end;
            }
            OpenAPI_list_add(add_rtp_header_ext_infoList, add_rtp_header_ext_infoItem);
        }
    }
    }

    rtp_payload_info_list = cJSON_GetObjectItemCaseSensitive(protocol_description_rmJSON, "rtpPayloadInfoList");
    if (rtp_payload_info_list) {
    if (!cJSON_IsNull(rtp_payload_info_list)) {
        cJSON *rtp_payload_info_list_local = NULL;
        if (!cJSON_IsArray(rtp_payload_info_list)) {
            ogs_error("OpenAPI_protocol_description_rm_parseFromJSON() failed [rtp_payload_info_list]");
            goto end;
        }

        rtp_payload_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(rtp_payload_info_list_local, rtp_payload_info_list) {
            if (!cJSON_IsObject(rtp_payload_info_list_local)) {
                ogs_error("OpenAPI_protocol_description_rm_parseFromJSON() failed [rtp_payload_info_list]");
                goto end;
            }
            OpenAPI_rtp_payload_info_t *rtp_payload_info_listItem = OpenAPI_rtp_payload_info_parseFromJSON(rtp_payload_info_list_local);
            if (!rtp_payload_info_listItem) {
                ogs_error("No rtp_payload_info_listItem");
                goto end;
            }
            OpenAPI_list_add(rtp_payload_info_listList, rtp_payload_info_listItem);
        }
    }
    }

    mri_transfer_info = cJSON_GetObjectItemCaseSensitive(protocol_description_rmJSON, "mriTransferInfo");
    if (mri_transfer_info) {
    if (!cJSON_IsString(mri_transfer_info)) {
        ogs_error("OpenAPI_protocol_description_rm_parseFromJSON() failed [mri_transfer_info]");
        goto end;
    }
    mri_transfer_infoVariable = OpenAPI_mri_transfer_method_FromString(mri_transfer_info->valuestring);
    }

    protocol_description_rm_local_var = OpenAPI_protocol_description_rm_create (
        transport_proto ? transport_protoVariable : 0,
        rtp_header_ext_info && cJSON_IsNull(rtp_header_ext_info) ? true : false,
        rtp_header_ext_info ? rtp_header_ext_info_local_nonprim : NULL,
        add_rtp_header_ext_info && cJSON_IsNull(add_rtp_header_ext_info) ? true : false,
        add_rtp_header_ext_info ? add_rtp_header_ext_infoList : NULL,
        rtp_payload_info_list && cJSON_IsNull(rtp_payload_info_list) ? true : false,
        rtp_payload_info_list ? rtp_payload_info_listList : NULL,
        mri_transfer_info ? mri_transfer_infoVariable : 0
    );

    return protocol_description_rm_local_var;
end:
    if (rtp_header_ext_info_local_nonprim) {
        OpenAPI_rtp_header_ext_info_rm_free(rtp_header_ext_info_local_nonprim);
        rtp_header_ext_info_local_nonprim = NULL;
    }
    if (add_rtp_header_ext_infoList) {
        OpenAPI_list_for_each(add_rtp_header_ext_infoList, node) {
            OpenAPI_rtp_header_ext_info_free(node->data);
        }
        OpenAPI_list_free(add_rtp_header_ext_infoList);
        add_rtp_header_ext_infoList = NULL;
    }
    if (rtp_payload_info_listList) {
        OpenAPI_list_for_each(rtp_payload_info_listList, node) {
            OpenAPI_rtp_payload_info_free(node->data);
        }
        OpenAPI_list_free(rtp_payload_info_listList);
        rtp_payload_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_protocol_description_rm_t *OpenAPI_protocol_description_rm_copy(OpenAPI_protocol_description_rm_t *dst, OpenAPI_protocol_description_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_protocol_description_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_protocol_description_rm_convertToJSON() failed");
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

    OpenAPI_protocol_description_rm_free(dst);
    dst = OpenAPI_protocol_description_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

