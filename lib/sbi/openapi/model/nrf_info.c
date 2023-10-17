
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info.h"

OpenAPI_nrf_info_t *OpenAPI_nrf_info_create(
    OpenAPI_list_t* served_udr_info,
    OpenAPI_list_t* served_udr_info_list,
    OpenAPI_list_t* served_udm_info,
    OpenAPI_list_t* served_udm_info_list,
    OpenAPI_list_t* served_ausf_info,
    OpenAPI_list_t* served_ausf_info_list,
    OpenAPI_list_t* served_amf_info,
    OpenAPI_list_t* served_amf_info_list,
    OpenAPI_list_t* served_smf_info,
    OpenAPI_list_t* served_smf_info_list,
    OpenAPI_list_t* served_upf_info,
    OpenAPI_list_t* served_upf_info_list,
    OpenAPI_list_t* served_pcf_info,
    OpenAPI_list_t* served_pcf_info_list,
    OpenAPI_list_t* served_bsf_info,
    OpenAPI_list_t* served_bsf_info_list,
    OpenAPI_list_t* served_chf_info,
    OpenAPI_list_t* served_chf_info_list,
    OpenAPI_list_t* served_nef_info,
    OpenAPI_list_t* served_nwdaf_info,
    OpenAPI_list_t* served_nwdaf_info_list,
    OpenAPI_list_t* served_pcscf_info_list,
    OpenAPI_list_t* served_gmlc_info,
    OpenAPI_list_t* served_lmf_info,
    OpenAPI_list_t* served_nf_info,
    OpenAPI_list_t* served_hss_info_list,
    OpenAPI_list_t* served_udsf_info,
    OpenAPI_list_t* served_udsf_info_list,
    OpenAPI_list_t* served_scp_info_list,
    OpenAPI_list_t* served_sepp_info_list,
    OpenAPI_list_t* served_aanf_info_list,
    OpenAPI_list_t* served5g_ddnmf_info,
    OpenAPI_list_t* served_mfaf_info_list,
    OpenAPI_list_t* served_easdf_info_list,
    OpenAPI_list_t* served_dccf_info_list,
    OpenAPI_list_t* served_mb_smf_info_list,
    OpenAPI_list_t* served_tsctsf_info_list,
    OpenAPI_list_t* served_mb_upf_info_list,
    OpenAPI_list_t* served_trust_af_info,
    OpenAPI_list_t* served_nssaaf_info
)
{
    OpenAPI_nrf_info_t *nrf_info_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_t));
    ogs_assert(nrf_info_local_var);

    nrf_info_local_var->served_udr_info = served_udr_info;
    nrf_info_local_var->served_udr_info_list = served_udr_info_list;
    nrf_info_local_var->served_udm_info = served_udm_info;
    nrf_info_local_var->served_udm_info_list = served_udm_info_list;
    nrf_info_local_var->served_ausf_info = served_ausf_info;
    nrf_info_local_var->served_ausf_info_list = served_ausf_info_list;
    nrf_info_local_var->served_amf_info = served_amf_info;
    nrf_info_local_var->served_amf_info_list = served_amf_info_list;
    nrf_info_local_var->served_smf_info = served_smf_info;
    nrf_info_local_var->served_smf_info_list = served_smf_info_list;
    nrf_info_local_var->served_upf_info = served_upf_info;
    nrf_info_local_var->served_upf_info_list = served_upf_info_list;
    nrf_info_local_var->served_pcf_info = served_pcf_info;
    nrf_info_local_var->served_pcf_info_list = served_pcf_info_list;
    nrf_info_local_var->served_bsf_info = served_bsf_info;
    nrf_info_local_var->served_bsf_info_list = served_bsf_info_list;
    nrf_info_local_var->served_chf_info = served_chf_info;
    nrf_info_local_var->served_chf_info_list = served_chf_info_list;
    nrf_info_local_var->served_nef_info = served_nef_info;
    nrf_info_local_var->served_nwdaf_info = served_nwdaf_info;
    nrf_info_local_var->served_nwdaf_info_list = served_nwdaf_info_list;
    nrf_info_local_var->served_pcscf_info_list = served_pcscf_info_list;
    nrf_info_local_var->served_gmlc_info = served_gmlc_info;
    nrf_info_local_var->served_lmf_info = served_lmf_info;
    nrf_info_local_var->served_nf_info = served_nf_info;
    nrf_info_local_var->served_hss_info_list = served_hss_info_list;
    nrf_info_local_var->served_udsf_info = served_udsf_info;
    nrf_info_local_var->served_udsf_info_list = served_udsf_info_list;
    nrf_info_local_var->served_scp_info_list = served_scp_info_list;
    nrf_info_local_var->served_sepp_info_list = served_sepp_info_list;
    nrf_info_local_var->served_aanf_info_list = served_aanf_info_list;
    nrf_info_local_var->served5g_ddnmf_info = served5g_ddnmf_info;
    nrf_info_local_var->served_mfaf_info_list = served_mfaf_info_list;
    nrf_info_local_var->served_easdf_info_list = served_easdf_info_list;
    nrf_info_local_var->served_dccf_info_list = served_dccf_info_list;
    nrf_info_local_var->served_mb_smf_info_list = served_mb_smf_info_list;
    nrf_info_local_var->served_tsctsf_info_list = served_tsctsf_info_list;
    nrf_info_local_var->served_mb_upf_info_list = served_mb_upf_info_list;
    nrf_info_local_var->served_trust_af_info = served_trust_af_info;
    nrf_info_local_var->served_nssaaf_info = served_nssaaf_info;

    return nrf_info_local_var;
}

void OpenAPI_nrf_info_free(OpenAPI_nrf_info_t *nrf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info) {
        return;
    }
    if (nrf_info->served_udr_info) {
        OpenAPI_list_for_each(nrf_info->served_udr_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_udr_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_udr_info);
        nrf_info->served_udr_info = NULL;
    }
    if (nrf_info->served_udr_info_list) {
        OpenAPI_list_for_each(nrf_info->served_udr_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_udr_info_list);
        nrf_info->served_udr_info_list = NULL;
    }
    if (nrf_info->served_udm_info) {
        OpenAPI_list_for_each(nrf_info->served_udm_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_udm_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_udm_info);
        nrf_info->served_udm_info = NULL;
    }
    if (nrf_info->served_udm_info_list) {
        OpenAPI_list_for_each(nrf_info->served_udm_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_udm_info_list);
        nrf_info->served_udm_info_list = NULL;
    }
    if (nrf_info->served_ausf_info) {
        OpenAPI_list_for_each(nrf_info->served_ausf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_ausf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_ausf_info);
        nrf_info->served_ausf_info = NULL;
    }
    if (nrf_info->served_ausf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_ausf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_ausf_info_list);
        nrf_info->served_ausf_info_list = NULL;
    }
    if (nrf_info->served_amf_info) {
        OpenAPI_list_for_each(nrf_info->served_amf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_amf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_amf_info);
        nrf_info->served_amf_info = NULL;
    }
    if (nrf_info->served_amf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_amf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_amf_info_list);
        nrf_info->served_amf_info_list = NULL;
    }
    if (nrf_info->served_smf_info) {
        OpenAPI_list_for_each(nrf_info->served_smf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_smf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_smf_info);
        nrf_info->served_smf_info = NULL;
    }
    if (nrf_info->served_smf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_smf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_smf_info_list);
        nrf_info->served_smf_info_list = NULL;
    }
    if (nrf_info->served_upf_info) {
        OpenAPI_list_for_each(nrf_info->served_upf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_upf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_upf_info);
        nrf_info->served_upf_info = NULL;
    }
    if (nrf_info->served_upf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_upf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_upf_info_list);
        nrf_info->served_upf_info_list = NULL;
    }
    if (nrf_info->served_pcf_info) {
        OpenAPI_list_for_each(nrf_info->served_pcf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_pcf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_pcf_info);
        nrf_info->served_pcf_info = NULL;
    }
    if (nrf_info->served_pcf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_pcf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_pcf_info_list);
        nrf_info->served_pcf_info_list = NULL;
    }
    if (nrf_info->served_bsf_info) {
        OpenAPI_list_for_each(nrf_info->served_bsf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_bsf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_bsf_info);
        nrf_info->served_bsf_info = NULL;
    }
    if (nrf_info->served_bsf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_bsf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_bsf_info_list);
        nrf_info->served_bsf_info_list = NULL;
    }
    if (nrf_info->served_chf_info) {
        OpenAPI_list_for_each(nrf_info->served_chf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_chf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_chf_info);
        nrf_info->served_chf_info = NULL;
    }
    if (nrf_info->served_chf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_chf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_chf_info_list);
        nrf_info->served_chf_info_list = NULL;
    }
    if (nrf_info->served_nef_info) {
        OpenAPI_list_for_each(nrf_info->served_nef_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_nef_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_nef_info);
        nrf_info->served_nef_info = NULL;
    }
    if (nrf_info->served_nwdaf_info) {
        OpenAPI_list_for_each(nrf_info->served_nwdaf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_nwdaf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_nwdaf_info);
        nrf_info->served_nwdaf_info = NULL;
    }
    if (nrf_info->served_nwdaf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_nwdaf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_nwdaf_info_list);
        nrf_info->served_nwdaf_info_list = NULL;
    }
    if (nrf_info->served_pcscf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_pcscf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_pcscf_info_list);
        nrf_info->served_pcscf_info_list = NULL;
    }
    if (nrf_info->served_gmlc_info) {
        OpenAPI_list_for_each(nrf_info->served_gmlc_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_gmlc_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_gmlc_info);
        nrf_info->served_gmlc_info = NULL;
    }
    if (nrf_info->served_lmf_info) {
        OpenAPI_list_for_each(nrf_info->served_lmf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_lmf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_lmf_info);
        nrf_info->served_lmf_info = NULL;
    }
    if (nrf_info->served_nf_info) {
        OpenAPI_list_for_each(nrf_info->served_nf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_nf_info);
        nrf_info->served_nf_info = NULL;
    }
    if (nrf_info->served_hss_info_list) {
        OpenAPI_list_for_each(nrf_info->served_hss_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_hss_info_list);
        nrf_info->served_hss_info_list = NULL;
    }
    if (nrf_info->served_udsf_info) {
        OpenAPI_list_for_each(nrf_info->served_udsf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_udsf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_udsf_info);
        nrf_info->served_udsf_info = NULL;
    }
    if (nrf_info->served_udsf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_udsf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_udsf_info_list);
        nrf_info->served_udsf_info_list = NULL;
    }
    if (nrf_info->served_scp_info_list) {
        OpenAPI_list_for_each(nrf_info->served_scp_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_scp_info_list_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_scp_info_list);
        nrf_info->served_scp_info_list = NULL;
    }
    if (nrf_info->served_sepp_info_list) {
        OpenAPI_list_for_each(nrf_info->served_sepp_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_sepp_info_list_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_sepp_info_list);
        nrf_info->served_sepp_info_list = NULL;
    }
    if (nrf_info->served_aanf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_aanf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_aanf_info_list);
        nrf_info->served_aanf_info_list = NULL;
    }
    if (nrf_info->served5g_ddnmf_info) {
        OpenAPI_list_for_each(nrf_info->served5g_ddnmf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_model_5_g_ddnmf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served5g_ddnmf_info);
        nrf_info->served5g_ddnmf_info = NULL;
    }
    if (nrf_info->served_mfaf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_mfaf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_mfaf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_mfaf_info_list);
        nrf_info->served_mfaf_info_list = NULL;
    }
    if (nrf_info->served_easdf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_easdf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_easdf_info_list);
        nrf_info->served_easdf_info_list = NULL;
    }
    if (nrf_info->served_dccf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_dccf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_dccf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_dccf_info_list);
        nrf_info->served_dccf_info_list = NULL;
    }
    if (nrf_info->served_mb_smf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_mb_smf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_mb_smf_info_list);
        nrf_info->served_mb_smf_info_list = NULL;
    }
    if (nrf_info->served_tsctsf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_tsctsf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_tsctsf_info_list);
        nrf_info->served_tsctsf_info_list = NULL;
    }
    if (nrf_info->served_mb_upf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_mb_upf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_mb_upf_info_list);
        nrf_info->served_mb_upf_info_list = NULL;
    }
    if (nrf_info->served_trust_af_info) {
        OpenAPI_list_for_each(nrf_info->served_trust_af_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_trust_af_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_trust_af_info);
        nrf_info->served_trust_af_info = NULL;
    }
    if (nrf_info->served_nssaaf_info) {
        OpenAPI_list_for_each(nrf_info->served_nssaaf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nssaaf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info->served_nssaaf_info);
        nrf_info->served_nssaaf_info = NULL;
    }
    ogs_free(nrf_info);
}

cJSON *OpenAPI_nrf_info_convertToJSON(OpenAPI_nrf_info_t *nrf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [NrfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info->served_udr_info) {
    cJSON *served_udr_info = cJSON_AddObjectToObject(item, "servedUdrInfo");
    if (served_udr_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udr_info]");
        goto end;
    }
    cJSON *localMapObject = served_udr_info;
    if (nrf_info->served_udr_info) {
        OpenAPI_list_for_each(nrf_info->served_udr_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udr_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udr_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_udr_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_udr_info_list) {
    cJSON *served_udr_info_list = cJSON_AddObjectToObject(item, "servedUdrInfoList");
    if (served_udr_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udr_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_udr_info_list;
    if (nrf_info->served_udr_info_list) {
        OpenAPI_list_for_each(nrf_info->served_udr_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udr_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udr_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_udr_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_udm_info) {
    cJSON *served_udm_info = cJSON_AddObjectToObject(item, "servedUdmInfo");
    if (served_udm_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udm_info]");
        goto end;
    }
    cJSON *localMapObject = served_udm_info;
    if (nrf_info->served_udm_info) {
        OpenAPI_list_for_each(nrf_info->served_udm_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udm_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udm_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_udm_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_udm_info_list) {
    cJSON *served_udm_info_list = cJSON_AddObjectToObject(item, "servedUdmInfoList");
    if (served_udm_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udm_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_udm_info_list;
    if (nrf_info->served_udm_info_list) {
        OpenAPI_list_for_each(nrf_info->served_udm_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udm_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udm_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_udm_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_ausf_info) {
    cJSON *served_ausf_info = cJSON_AddObjectToObject(item, "servedAusfInfo");
    if (served_ausf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_ausf_info]");
        goto end;
    }
    cJSON *localMapObject = served_ausf_info;
    if (nrf_info->served_ausf_info) {
        OpenAPI_list_for_each(nrf_info->served_ausf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_ausf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_ausf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_ausf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_ausf_info_list) {
    cJSON *served_ausf_info_list = cJSON_AddObjectToObject(item, "servedAusfInfoList");
    if (served_ausf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_ausf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_ausf_info_list;
    if (nrf_info->served_ausf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_ausf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_ausf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_ausf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_ausf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_amf_info) {
    cJSON *served_amf_info = cJSON_AddObjectToObject(item, "servedAmfInfo");
    if (served_amf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_amf_info]");
        goto end;
    }
    cJSON *localMapObject = served_amf_info;
    if (nrf_info->served_amf_info) {
        OpenAPI_list_for_each(nrf_info->served_amf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_amf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_amf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_amf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_amf_info_list) {
    cJSON *served_amf_info_list = cJSON_AddObjectToObject(item, "servedAmfInfoList");
    if (served_amf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_amf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_amf_info_list;
    if (nrf_info->served_amf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_amf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_amf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_amf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_amf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_smf_info) {
    cJSON *served_smf_info = cJSON_AddObjectToObject(item, "servedSmfInfo");
    if (served_smf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_smf_info]");
        goto end;
    }
    cJSON *localMapObject = served_smf_info;
    if (nrf_info->served_smf_info) {
        OpenAPI_list_for_each(nrf_info->served_smf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_smf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_smf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_smf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_smf_info_list) {
    cJSON *served_smf_info_list = cJSON_AddObjectToObject(item, "servedSmfInfoList");
    if (served_smf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_smf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_smf_info_list;
    if (nrf_info->served_smf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_smf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_smf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_smf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_smf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_upf_info) {
    cJSON *served_upf_info = cJSON_AddObjectToObject(item, "servedUpfInfo");
    if (served_upf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_upf_info]");
        goto end;
    }
    cJSON *localMapObject = served_upf_info;
    if (nrf_info->served_upf_info) {
        OpenAPI_list_for_each(nrf_info->served_upf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_upf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_upf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_upf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_upf_info_list) {
    cJSON *served_upf_info_list = cJSON_AddObjectToObject(item, "servedUpfInfoList");
    if (served_upf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_upf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_upf_info_list;
    if (nrf_info->served_upf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_upf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_upf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_upf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_upf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_pcf_info) {
    cJSON *served_pcf_info = cJSON_AddObjectToObject(item, "servedPcfInfo");
    if (served_pcf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcf_info]");
        goto end;
    }
    cJSON *localMapObject = served_pcf_info;
    if (nrf_info->served_pcf_info) {
        OpenAPI_list_for_each(nrf_info->served_pcf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_pcf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_pcf_info_list) {
    cJSON *served_pcf_info_list = cJSON_AddObjectToObject(item, "servedPcfInfoList");
    if (served_pcf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_pcf_info_list;
    if (nrf_info->served_pcf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_pcf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_pcf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_bsf_info) {
    cJSON *served_bsf_info = cJSON_AddObjectToObject(item, "servedBsfInfo");
    if (served_bsf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_bsf_info]");
        goto end;
    }
    cJSON *localMapObject = served_bsf_info;
    if (nrf_info->served_bsf_info) {
        OpenAPI_list_for_each(nrf_info->served_bsf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_bsf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_bsf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_bsf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_bsf_info_list) {
    cJSON *served_bsf_info_list = cJSON_AddObjectToObject(item, "servedBsfInfoList");
    if (served_bsf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_bsf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_bsf_info_list;
    if (nrf_info->served_bsf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_bsf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_bsf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_bsf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_bsf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_chf_info) {
    cJSON *served_chf_info = cJSON_AddObjectToObject(item, "servedChfInfo");
    if (served_chf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_chf_info]");
        goto end;
    }
    cJSON *localMapObject = served_chf_info;
    if (nrf_info->served_chf_info) {
        OpenAPI_list_for_each(nrf_info->served_chf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_chf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_chf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_chf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_chf_info_list) {
    cJSON *served_chf_info_list = cJSON_AddObjectToObject(item, "servedChfInfoList");
    if (served_chf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_chf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_chf_info_list;
    if (nrf_info->served_chf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_chf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_chf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_chf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_chf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_nef_info) {
    cJSON *served_nef_info = cJSON_AddObjectToObject(item, "servedNefInfo");
    if (served_nef_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nef_info]");
        goto end;
    }
    cJSON *localMapObject = served_nef_info;
    if (nrf_info->served_nef_info) {
        OpenAPI_list_for_each(nrf_info->served_nef_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nef_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nef_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_nef_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_nwdaf_info) {
    cJSON *served_nwdaf_info = cJSON_AddObjectToObject(item, "servedNwdafInfo");
    if (served_nwdaf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nwdaf_info]");
        goto end;
    }
    cJSON *localMapObject = served_nwdaf_info;
    if (nrf_info->served_nwdaf_info) {
        OpenAPI_list_for_each(nrf_info->served_nwdaf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nwdaf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nwdaf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_nwdaf_info_list) {
    cJSON *served_nwdaf_info_list = cJSON_AddObjectToObject(item, "servedNwdafInfoList");
    if (served_nwdaf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nwdaf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_nwdaf_info_list;
    if (nrf_info->served_nwdaf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_nwdaf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nwdaf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nwdaf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nwdaf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_pcscf_info_list) {
    cJSON *served_pcscf_info_list = cJSON_AddObjectToObject(item, "servedPcscfInfoList");
    if (served_pcscf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcscf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_pcscf_info_list;
    if (nrf_info->served_pcscf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_pcscf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcscf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_pcscf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_gmlc_info) {
    cJSON *served_gmlc_info = cJSON_AddObjectToObject(item, "servedGmlcInfo");
    if (served_gmlc_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_gmlc_info]");
        goto end;
    }
    cJSON *localMapObject = served_gmlc_info;
    if (nrf_info->served_gmlc_info) {
        OpenAPI_list_for_each(nrf_info->served_gmlc_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_gmlc_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_gmlc_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_lmf_info) {
    cJSON *served_lmf_info = cJSON_AddObjectToObject(item, "servedLmfInfo");
    if (served_lmf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_lmf_info]");
        goto end;
    }
    cJSON *localMapObject = served_lmf_info;
    if (nrf_info->served_lmf_info) {
        OpenAPI_list_for_each(nrf_info->served_lmf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_lmf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_lmf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_lmf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_nf_info) {
    cJSON *served_nf_info = cJSON_AddObjectToObject(item, "servedNfInfo");
    if (served_nf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nf_info]");
        goto end;
    }
    cJSON *localMapObject = served_nf_info;
    if (nrf_info->served_nf_info) {
        OpenAPI_list_for_each(nrf_info->served_nf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_hss_info_list) {
    cJSON *served_hss_info_list = cJSON_AddObjectToObject(item, "servedHssInfoList");
    if (served_hss_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_hss_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_hss_info_list;
    if (nrf_info->served_hss_info_list) {
        OpenAPI_list_for_each(nrf_info->served_hss_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_hss_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_hss_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_hss_info_list_value_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_udsf_info) {
    cJSON *served_udsf_info = cJSON_AddObjectToObject(item, "servedUdsfInfo");
    if (served_udsf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udsf_info]");
        goto end;
    }
    cJSON *localMapObject = served_udsf_info;
    if (nrf_info->served_udsf_info) {
        OpenAPI_list_for_each(nrf_info->served_udsf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udsf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udsf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_udsf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_udsf_info_list) {
    cJSON *served_udsf_info_list = cJSON_AddObjectToObject(item, "servedUdsfInfoList");
    if (served_udsf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udsf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_udsf_info_list;
    if (nrf_info->served_udsf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_udsf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udsf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_udsf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_udsf_info_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_scp_info_list) {
    cJSON *served_scp_info_list = cJSON_AddObjectToObject(item, "servedScpInfoList");
    if (served_scp_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_scp_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_scp_info_list;
    if (nrf_info->served_scp_info_list) {
        OpenAPI_list_for_each(nrf_info->served_scp_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_scp_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_scp_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_scp_info_list_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_sepp_info_list) {
    cJSON *served_sepp_info_list = cJSON_AddObjectToObject(item, "servedSeppInfoList");
    if (served_sepp_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_sepp_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_sepp_info_list;
    if (nrf_info->served_sepp_info_list) {
        OpenAPI_list_for_each(nrf_info->served_sepp_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_sepp_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_sepp_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_sepp_info_list_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_aanf_info_list) {
    cJSON *served_aanf_info_list = cJSON_AddObjectToObject(item, "servedAanfInfoList");
    if (served_aanf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_aanf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_aanf_info_list;
    if (nrf_info->served_aanf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_aanf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_aanf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_aanf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served5g_ddnmf_info) {
    cJSON *served5g_ddnmf_info = cJSON_AddObjectToObject(item, "served5gDdnmfInfo");
    if (served5g_ddnmf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served5g_ddnmf_info]");
        goto end;
    }
    cJSON *localMapObject = served5g_ddnmf_info;
    if (nrf_info->served5g_ddnmf_info) {
        OpenAPI_list_for_each(nrf_info->served5g_ddnmf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served5g_ddnmf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served5g_ddnmf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_model_5_g_ddnmf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_mfaf_info_list) {
    cJSON *served_mfaf_info_list = cJSON_AddObjectToObject(item, "servedMfafInfoList");
    if (served_mfaf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mfaf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_mfaf_info_list;
    if (nrf_info->served_mfaf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_mfaf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mfaf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mfaf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_mfaf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_easdf_info_list) {
    cJSON *served_easdf_info_list = cJSON_AddObjectToObject(item, "servedEasdfInfoList");
    if (served_easdf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_easdf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_easdf_info_list;
    if (nrf_info->served_easdf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_easdf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_easdf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_easdf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_easdf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_dccf_info_list) {
    cJSON *served_dccf_info_list = cJSON_AddObjectToObject(item, "servedDccfInfoList");
    if (served_dccf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_dccf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_dccf_info_list;
    if (nrf_info->served_dccf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_dccf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_dccf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_dccf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_dccf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_mb_smf_info_list) {
    cJSON *served_mb_smf_info_list = cJSON_AddObjectToObject(item, "servedMbSmfInfoList");
    if (served_mb_smf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mb_smf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_mb_smf_info_list;
    if (nrf_info->served_mb_smf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_mb_smf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mb_smf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mb_smf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nrf_info_served_mb_smf_info_list_value_value_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_tsctsf_info_list) {
    cJSON *served_tsctsf_info_list = cJSON_AddObjectToObject(item, "servedTsctsfInfoList");
    if (served_tsctsf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_tsctsf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_tsctsf_info_list;
    if (nrf_info->served_tsctsf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_tsctsf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_tsctsf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_tsctsf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_tsctsf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_mb_upf_info_list) {
    cJSON *served_mb_upf_info_list = cJSON_AddObjectToObject(item, "servedMbUpfInfoList");
    if (served_mb_upf_info_list == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mb_upf_info_list]");
        goto end;
    }
    cJSON *localMapObject = served_mb_upf_info_list;
    if (nrf_info->served_mb_upf_info_list) {
        OpenAPI_list_for_each(nrf_info->served_mb_upf_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mb_upf_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_mb_upf_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_mb_upf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_trust_af_info) {
    cJSON *served_trust_af_info = cJSON_AddObjectToObject(item, "servedTrustAfInfo");
    if (served_trust_af_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_trust_af_info]");
        goto end;
    }
    cJSON *localMapObject = served_trust_af_info;
    if (nrf_info->served_trust_af_info) {
        OpenAPI_list_for_each(nrf_info->served_trust_af_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_trust_af_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_trust_af_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_trust_af_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nrf_info->served_nssaaf_info) {
    cJSON *served_nssaaf_info = cJSON_AddObjectToObject(item, "servedNssaafInfo");
    if (served_nssaaf_info == NULL) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nssaaf_info]");
        goto end;
    }
    cJSON *localMapObject = served_nssaaf_info;
    if (nrf_info->served_nssaaf_info) {
        OpenAPI_list_for_each(nrf_info->served_nssaaf_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nssaaf_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [served_nssaaf_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nssaaf_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_t *OpenAPI_nrf_info_parseFromJSON(cJSON *nrf_infoJSON)
{
    OpenAPI_nrf_info_t *nrf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *served_udr_info = NULL;
    OpenAPI_list_t *served_udr_infoList = NULL;
    cJSON *served_udr_info_list = NULL;
    OpenAPI_list_t *served_udr_info_listList = NULL;
    cJSON *served_udm_info = NULL;
    OpenAPI_list_t *served_udm_infoList = NULL;
    cJSON *served_udm_info_list = NULL;
    OpenAPI_list_t *served_udm_info_listList = NULL;
    cJSON *served_ausf_info = NULL;
    OpenAPI_list_t *served_ausf_infoList = NULL;
    cJSON *served_ausf_info_list = NULL;
    OpenAPI_list_t *served_ausf_info_listList = NULL;
    cJSON *served_amf_info = NULL;
    OpenAPI_list_t *served_amf_infoList = NULL;
    cJSON *served_amf_info_list = NULL;
    OpenAPI_list_t *served_amf_info_listList = NULL;
    cJSON *served_smf_info = NULL;
    OpenAPI_list_t *served_smf_infoList = NULL;
    cJSON *served_smf_info_list = NULL;
    OpenAPI_list_t *served_smf_info_listList = NULL;
    cJSON *served_upf_info = NULL;
    OpenAPI_list_t *served_upf_infoList = NULL;
    cJSON *served_upf_info_list = NULL;
    OpenAPI_list_t *served_upf_info_listList = NULL;
    cJSON *served_pcf_info = NULL;
    OpenAPI_list_t *served_pcf_infoList = NULL;
    cJSON *served_pcf_info_list = NULL;
    OpenAPI_list_t *served_pcf_info_listList = NULL;
    cJSON *served_bsf_info = NULL;
    OpenAPI_list_t *served_bsf_infoList = NULL;
    cJSON *served_bsf_info_list = NULL;
    OpenAPI_list_t *served_bsf_info_listList = NULL;
    cJSON *served_chf_info = NULL;
    OpenAPI_list_t *served_chf_infoList = NULL;
    cJSON *served_chf_info_list = NULL;
    OpenAPI_list_t *served_chf_info_listList = NULL;
    cJSON *served_nef_info = NULL;
    OpenAPI_list_t *served_nef_infoList = NULL;
    cJSON *served_nwdaf_info = NULL;
    OpenAPI_list_t *served_nwdaf_infoList = NULL;
    cJSON *served_nwdaf_info_list = NULL;
    OpenAPI_list_t *served_nwdaf_info_listList = NULL;
    cJSON *served_pcscf_info_list = NULL;
    OpenAPI_list_t *served_pcscf_info_listList = NULL;
    cJSON *served_gmlc_info = NULL;
    OpenAPI_list_t *served_gmlc_infoList = NULL;
    cJSON *served_lmf_info = NULL;
    OpenAPI_list_t *served_lmf_infoList = NULL;
    cJSON *served_nf_info = NULL;
    OpenAPI_list_t *served_nf_infoList = NULL;
    cJSON *served_hss_info_list = NULL;
    OpenAPI_list_t *served_hss_info_listList = NULL;
    cJSON *served_udsf_info = NULL;
    OpenAPI_list_t *served_udsf_infoList = NULL;
    cJSON *served_udsf_info_list = NULL;
    OpenAPI_list_t *served_udsf_info_listList = NULL;
    cJSON *served_scp_info_list = NULL;
    OpenAPI_list_t *served_scp_info_listList = NULL;
    cJSON *served_sepp_info_list = NULL;
    OpenAPI_list_t *served_sepp_info_listList = NULL;
    cJSON *served_aanf_info_list = NULL;
    OpenAPI_list_t *served_aanf_info_listList = NULL;
    cJSON *served5g_ddnmf_info = NULL;
    OpenAPI_list_t *served5g_ddnmf_infoList = NULL;
    cJSON *served_mfaf_info_list = NULL;
    OpenAPI_list_t *served_mfaf_info_listList = NULL;
    cJSON *served_easdf_info_list = NULL;
    OpenAPI_list_t *served_easdf_info_listList = NULL;
    cJSON *served_dccf_info_list = NULL;
    OpenAPI_list_t *served_dccf_info_listList = NULL;
    cJSON *served_mb_smf_info_list = NULL;
    OpenAPI_list_t *served_mb_smf_info_listList = NULL;
    cJSON *served_tsctsf_info_list = NULL;
    OpenAPI_list_t *served_tsctsf_info_listList = NULL;
    cJSON *served_mb_upf_info_list = NULL;
    OpenAPI_list_t *served_mb_upf_info_listList = NULL;
    cJSON *served_trust_af_info = NULL;
    OpenAPI_list_t *served_trust_af_infoList = NULL;
    cJSON *served_nssaaf_info = NULL;
    OpenAPI_list_t *served_nssaaf_infoList = NULL;
    served_udr_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUdrInfo");
    if (served_udr_info) {
        cJSON *served_udr_info_local_map = NULL;
        if (!cJSON_IsObject(served_udr_info) && !cJSON_IsNull(served_udr_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_udr_info]");
            goto end;
        }
        if (cJSON_IsObject(served_udr_info)) {
            served_udr_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_udr_info_local_map, served_udr_info) {
                cJSON *localMapObject = served_udr_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_udr_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_udr_infoList, localMapKeyPair);
            }
        }
    }

    served_udr_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUdrInfoList");
    if (served_udr_info_list) {
        cJSON *served_udr_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_udr_info_list) && !cJSON_IsNull(served_udr_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_udr_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_udr_info_list)) {
            served_udr_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_udr_info_list_local_map, served_udr_info_list) {
                cJSON *localMapObject = served_udr_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_udr_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_udr_info_listList, localMapKeyPair);
            }
        }
    }

    served_udm_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUdmInfo");
    if (served_udm_info) {
        cJSON *served_udm_info_local_map = NULL;
        if (!cJSON_IsObject(served_udm_info) && !cJSON_IsNull(served_udm_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_udm_info]");
            goto end;
        }
        if (cJSON_IsObject(served_udm_info)) {
            served_udm_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_udm_info_local_map, served_udm_info) {
                cJSON *localMapObject = served_udm_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_udm_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_udm_infoList, localMapKeyPair);
            }
        }
    }

    served_udm_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUdmInfoList");
    if (served_udm_info_list) {
        cJSON *served_udm_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_udm_info_list) && !cJSON_IsNull(served_udm_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_udm_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_udm_info_list)) {
            served_udm_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_udm_info_list_local_map, served_udm_info_list) {
                cJSON *localMapObject = served_udm_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_udm_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_udm_info_listList, localMapKeyPair);
            }
        }
    }

    served_ausf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedAusfInfo");
    if (served_ausf_info) {
        cJSON *served_ausf_info_local_map = NULL;
        if (!cJSON_IsObject(served_ausf_info) && !cJSON_IsNull(served_ausf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_ausf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_ausf_info)) {
            served_ausf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_ausf_info_local_map, served_ausf_info) {
                cJSON *localMapObject = served_ausf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_ausf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_ausf_infoList, localMapKeyPair);
            }
        }
    }

    served_ausf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedAusfInfoList");
    if (served_ausf_info_list) {
        cJSON *served_ausf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_ausf_info_list) && !cJSON_IsNull(served_ausf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_ausf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_ausf_info_list)) {
            served_ausf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_ausf_info_list_local_map, served_ausf_info_list) {
                cJSON *localMapObject = served_ausf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_ausf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_ausf_info_listList, localMapKeyPair);
            }
        }
    }

    served_amf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedAmfInfo");
    if (served_amf_info) {
        cJSON *served_amf_info_local_map = NULL;
        if (!cJSON_IsObject(served_amf_info) && !cJSON_IsNull(served_amf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_amf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_amf_info)) {
            served_amf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_amf_info_local_map, served_amf_info) {
                cJSON *localMapObject = served_amf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_amf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_amf_infoList, localMapKeyPair);
            }
        }
    }

    served_amf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedAmfInfoList");
    if (served_amf_info_list) {
        cJSON *served_amf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_amf_info_list) && !cJSON_IsNull(served_amf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_amf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_amf_info_list)) {
            served_amf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_amf_info_list_local_map, served_amf_info_list) {
                cJSON *localMapObject = served_amf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_amf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_amf_info_listList, localMapKeyPair);
            }
        }
    }

    served_smf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedSmfInfo");
    if (served_smf_info) {
        cJSON *served_smf_info_local_map = NULL;
        if (!cJSON_IsObject(served_smf_info) && !cJSON_IsNull(served_smf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_smf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_smf_info)) {
            served_smf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_smf_info_local_map, served_smf_info) {
                cJSON *localMapObject = served_smf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_smf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_smf_infoList, localMapKeyPair);
            }
        }
    }

    served_smf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedSmfInfoList");
    if (served_smf_info_list) {
        cJSON *served_smf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_smf_info_list) && !cJSON_IsNull(served_smf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_smf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_smf_info_list)) {
            served_smf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_smf_info_list_local_map, served_smf_info_list) {
                cJSON *localMapObject = served_smf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_smf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_smf_info_listList, localMapKeyPair);
            }
        }
    }

    served_upf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUpfInfo");
    if (served_upf_info) {
        cJSON *served_upf_info_local_map = NULL;
        if (!cJSON_IsObject(served_upf_info) && !cJSON_IsNull(served_upf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_upf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_upf_info)) {
            served_upf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_upf_info_local_map, served_upf_info) {
                cJSON *localMapObject = served_upf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_upf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_upf_infoList, localMapKeyPair);
            }
        }
    }

    served_upf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUpfInfoList");
    if (served_upf_info_list) {
        cJSON *served_upf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_upf_info_list) && !cJSON_IsNull(served_upf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_upf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_upf_info_list)) {
            served_upf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_upf_info_list_local_map, served_upf_info_list) {
                cJSON *localMapObject = served_upf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_upf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_upf_info_listList, localMapKeyPair);
            }
        }
    }

    served_pcf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedPcfInfo");
    if (served_pcf_info) {
        cJSON *served_pcf_info_local_map = NULL;
        if (!cJSON_IsObject(served_pcf_info) && !cJSON_IsNull(served_pcf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_pcf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_pcf_info)) {
            served_pcf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_pcf_info_local_map, served_pcf_info) {
                cJSON *localMapObject = served_pcf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_pcf_infoList, localMapKeyPair);
            }
        }
    }

    served_pcf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedPcfInfoList");
    if (served_pcf_info_list) {
        cJSON *served_pcf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_pcf_info_list) && !cJSON_IsNull(served_pcf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_pcf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_pcf_info_list)) {
            served_pcf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_pcf_info_list_local_map, served_pcf_info_list) {
                cJSON *localMapObject = served_pcf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_pcf_info_listList, localMapKeyPair);
            }
        }
    }

    served_bsf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedBsfInfo");
    if (served_bsf_info) {
        cJSON *served_bsf_info_local_map = NULL;
        if (!cJSON_IsObject(served_bsf_info) && !cJSON_IsNull(served_bsf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_bsf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_bsf_info)) {
            served_bsf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_bsf_info_local_map, served_bsf_info) {
                cJSON *localMapObject = served_bsf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_bsf_infoList, localMapKeyPair);
            }
        }
    }

    served_bsf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedBsfInfoList");
    if (served_bsf_info_list) {
        cJSON *served_bsf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_bsf_info_list) && !cJSON_IsNull(served_bsf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_bsf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_bsf_info_list)) {
            served_bsf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_bsf_info_list_local_map, served_bsf_info_list) {
                cJSON *localMapObject = served_bsf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_bsf_info_listList, localMapKeyPair);
            }
        }
    }

    served_chf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedChfInfo");
    if (served_chf_info) {
        cJSON *served_chf_info_local_map = NULL;
        if (!cJSON_IsObject(served_chf_info) && !cJSON_IsNull(served_chf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_chf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_chf_info)) {
            served_chf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_chf_info_local_map, served_chf_info) {
                cJSON *localMapObject = served_chf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_chf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_chf_infoList, localMapKeyPair);
            }
        }
    }

    served_chf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedChfInfoList");
    if (served_chf_info_list) {
        cJSON *served_chf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_chf_info_list) && !cJSON_IsNull(served_chf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_chf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_chf_info_list)) {
            served_chf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_chf_info_list_local_map, served_chf_info_list) {
                cJSON *localMapObject = served_chf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_chf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_chf_info_listList, localMapKeyPair);
            }
        }
    }

    served_nef_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedNefInfo");
    if (served_nef_info) {
        cJSON *served_nef_info_local_map = NULL;
        if (!cJSON_IsObject(served_nef_info) && !cJSON_IsNull(served_nef_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_nef_info]");
            goto end;
        }
        if (cJSON_IsObject(served_nef_info)) {
            served_nef_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_nef_info_local_map, served_nef_info) {
                cJSON *localMapObject = served_nef_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_nef_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_nef_infoList, localMapKeyPair);
            }
        }
    }

    served_nwdaf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedNwdafInfo");
    if (served_nwdaf_info) {
        cJSON *served_nwdaf_info_local_map = NULL;
        if (!cJSON_IsObject(served_nwdaf_info) && !cJSON_IsNull(served_nwdaf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_nwdaf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_nwdaf_info)) {
            served_nwdaf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_nwdaf_info_local_map, served_nwdaf_info) {
                cJSON *localMapObject = served_nwdaf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_nwdaf_infoList, localMapKeyPair);
            }
        }
    }

    served_nwdaf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedNwdafInfoList");
    if (served_nwdaf_info_list) {
        cJSON *served_nwdaf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_nwdaf_info_list) && !cJSON_IsNull(served_nwdaf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_nwdaf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_nwdaf_info_list)) {
            served_nwdaf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_nwdaf_info_list_local_map, served_nwdaf_info_list) {
                cJSON *localMapObject = served_nwdaf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nwdaf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_nwdaf_info_listList, localMapKeyPair);
            }
        }
    }

    served_pcscf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedPcscfInfoList");
    if (served_pcscf_info_list) {
        cJSON *served_pcscf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_pcscf_info_list) && !cJSON_IsNull(served_pcscf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_pcscf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_pcscf_info_list)) {
            served_pcscf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_pcscf_info_list_local_map, served_pcscf_info_list) {
                cJSON *localMapObject = served_pcscf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_pcscf_info_listList, localMapKeyPair);
            }
        }
    }

    served_gmlc_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedGmlcInfo");
    if (served_gmlc_info) {
        cJSON *served_gmlc_info_local_map = NULL;
        if (!cJSON_IsObject(served_gmlc_info) && !cJSON_IsNull(served_gmlc_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_gmlc_info]");
            goto end;
        }
        if (cJSON_IsObject(served_gmlc_info)) {
            served_gmlc_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_gmlc_info_local_map, served_gmlc_info) {
                cJSON *localMapObject = served_gmlc_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_gmlc_infoList, localMapKeyPair);
            }
        }
    }

    served_lmf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedLmfInfo");
    if (served_lmf_info) {
        cJSON *served_lmf_info_local_map = NULL;
        if (!cJSON_IsObject(served_lmf_info) && !cJSON_IsNull(served_lmf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_lmf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_lmf_info)) {
            served_lmf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_lmf_info_local_map, served_lmf_info) {
                cJSON *localMapObject = served_lmf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_lmf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_lmf_infoList, localMapKeyPair);
            }
        }
    }

    served_nf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedNfInfo");
    if (served_nf_info) {
        cJSON *served_nf_info_local_map = NULL;
        if (!cJSON_IsObject(served_nf_info) && !cJSON_IsNull(served_nf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_nf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_nf_info)) {
            served_nf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_nf_info_local_map, served_nf_info) {
                cJSON *localMapObject = served_nf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_nf_infoList, localMapKeyPair);
            }
        }
    }

    served_hss_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedHssInfoList");
    if (served_hss_info_list) {
        cJSON *served_hss_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_hss_info_list) && !cJSON_IsNull(served_hss_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_hss_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_hss_info_list)) {
            served_hss_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_hss_info_list_local_map, served_hss_info_list) {
                cJSON *localMapObject = served_hss_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_hss_info_list_value_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_hss_info_listList, localMapKeyPair);
            }
        }
    }

    served_udsf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUdsfInfo");
    if (served_udsf_info) {
        cJSON *served_udsf_info_local_map = NULL;
        if (!cJSON_IsObject(served_udsf_info) && !cJSON_IsNull(served_udsf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_udsf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_udsf_info)) {
            served_udsf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_udsf_info_local_map, served_udsf_info) {
                cJSON *localMapObject = served_udsf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_udsf_infoList, localMapKeyPair);
            }
        }
    }

    served_udsf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedUdsfInfoList");
    if (served_udsf_info_list) {
        cJSON *served_udsf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_udsf_info_list) && !cJSON_IsNull(served_udsf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_udsf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_udsf_info_list)) {
            served_udsf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_udsf_info_list_local_map, served_udsf_info_list) {
                cJSON *localMapObject = served_udsf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_udsf_info_listList, localMapKeyPair);
            }
        }
    }

    served_scp_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedScpInfoList");
    if (served_scp_info_list) {
        cJSON *served_scp_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_scp_info_list) && !cJSON_IsNull(served_scp_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_scp_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_scp_info_list)) {
            served_scp_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_scp_info_list_local_map, served_scp_info_list) {
                cJSON *localMapObject = served_scp_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_scp_info_list_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_scp_info_listList, localMapKeyPair);
            }
        }
    }

    served_sepp_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedSeppInfoList");
    if (served_sepp_info_list) {
        cJSON *served_sepp_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_sepp_info_list) && !cJSON_IsNull(served_sepp_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_sepp_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_sepp_info_list)) {
            served_sepp_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_sepp_info_list_local_map, served_sepp_info_list) {
                cJSON *localMapObject = served_sepp_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_sepp_info_list_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_sepp_info_listList, localMapKeyPair);
            }
        }
    }

    served_aanf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedAanfInfoList");
    if (served_aanf_info_list) {
        cJSON *served_aanf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_aanf_info_list) && !cJSON_IsNull(served_aanf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_aanf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_aanf_info_list)) {
            served_aanf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_aanf_info_list_local_map, served_aanf_info_list) {
                cJSON *localMapObject = served_aanf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_aanf_info_list_value_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_aanf_info_listList, localMapKeyPair);
            }
        }
    }

    served5g_ddnmf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "served5gDdnmfInfo");
    if (served5g_ddnmf_info) {
        cJSON *served5g_ddnmf_info_local_map = NULL;
        if (!cJSON_IsObject(served5g_ddnmf_info) && !cJSON_IsNull(served5g_ddnmf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served5g_ddnmf_info]");
            goto end;
        }
        if (cJSON_IsObject(served5g_ddnmf_info)) {
            served5g_ddnmf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served5g_ddnmf_info_local_map, served5g_ddnmf_info) {
                cJSON *localMapObject = served5g_ddnmf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_model_5_g_ddnmf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served5g_ddnmf_infoList, localMapKeyPair);
            }
        }
    }

    served_mfaf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedMfafInfoList");
    if (served_mfaf_info_list) {
        cJSON *served_mfaf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_mfaf_info_list) && !cJSON_IsNull(served_mfaf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_mfaf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_mfaf_info_list)) {
            served_mfaf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_mfaf_info_list_local_map, served_mfaf_info_list) {
                cJSON *localMapObject = served_mfaf_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_mfaf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_mfaf_info_listList, localMapKeyPair);
            }
        }
    }

    served_easdf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedEasdfInfoList");
    if (served_easdf_info_list) {
        cJSON *served_easdf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_easdf_info_list) && !cJSON_IsNull(served_easdf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_easdf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_easdf_info_list)) {
            served_easdf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_easdf_info_list_local_map, served_easdf_info_list) {
                cJSON *localMapObject = served_easdf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_easdf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_easdf_info_listList, localMapKeyPair);
            }
        }
    }

    served_dccf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedDccfInfoList");
    if (served_dccf_info_list) {
        cJSON *served_dccf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_dccf_info_list) && !cJSON_IsNull(served_dccf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_dccf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_dccf_info_list)) {
            served_dccf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_dccf_info_list_local_map, served_dccf_info_list) {
                cJSON *localMapObject = served_dccf_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_dccf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_dccf_info_listList, localMapKeyPair);
            }
        }
    }

    served_mb_smf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedMbSmfInfoList");
    if (served_mb_smf_info_list) {
        cJSON *served_mb_smf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_mb_smf_info_list) && !cJSON_IsNull(served_mb_smf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_mb_smf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_mb_smf_info_list)) {
            served_mb_smf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_mb_smf_info_list_local_map, served_mb_smf_info_list) {
                cJSON *localMapObject = served_mb_smf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nrf_info_served_mb_smf_info_list_value_value_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_mb_smf_info_listList, localMapKeyPair);
            }
        }
    }

    served_tsctsf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedTsctsfInfoList");
    if (served_tsctsf_info_list) {
        cJSON *served_tsctsf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_tsctsf_info_list) && !cJSON_IsNull(served_tsctsf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_tsctsf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_tsctsf_info_list)) {
            served_tsctsf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_tsctsf_info_list_local_map, served_tsctsf_info_list) {
                cJSON *localMapObject = served_tsctsf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_tsctsf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_tsctsf_info_listList, localMapKeyPair);
            }
        }
    }

    served_mb_upf_info_list = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedMbUpfInfoList");
    if (served_mb_upf_info_list) {
        cJSON *served_mb_upf_info_list_local_map = NULL;
        if (!cJSON_IsObject(served_mb_upf_info_list) && !cJSON_IsNull(served_mb_upf_info_list)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_mb_upf_info_list]");
            goto end;
        }
        if (cJSON_IsObject(served_mb_upf_info_list)) {
            served_mb_upf_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_mb_upf_info_list_local_map, served_mb_upf_info_list) {
                cJSON *localMapObject = served_mb_upf_info_list_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_mb_upf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_mb_upf_info_listList, localMapKeyPair);
            }
        }
    }

    served_trust_af_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedTrustAfInfo");
    if (served_trust_af_info) {
        cJSON *served_trust_af_info_local_map = NULL;
        if (!cJSON_IsObject(served_trust_af_info) && !cJSON_IsNull(served_trust_af_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_trust_af_info]");
            goto end;
        }
        if (cJSON_IsObject(served_trust_af_info)) {
            served_trust_af_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_trust_af_info_local_map, served_trust_af_info) {
                cJSON *localMapObject = served_trust_af_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_trust_af_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_trust_af_infoList, localMapKeyPair);
            }
        }
    }

    served_nssaaf_info = cJSON_GetObjectItemCaseSensitive(nrf_infoJSON, "servedNssaafInfo");
    if (served_nssaaf_info) {
        cJSON *served_nssaaf_info_local_map = NULL;
        if (!cJSON_IsObject(served_nssaaf_info) && !cJSON_IsNull(served_nssaaf_info)) {
            ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [served_nssaaf_info]");
            goto end;
        }
        if (cJSON_IsObject(served_nssaaf_info)) {
            served_nssaaf_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(served_nssaaf_info_local_map, served_nssaaf_info) {
                cJSON *localMapObject = served_nssaaf_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nssaaf_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(served_nssaaf_infoList, localMapKeyPair);
            }
        }
    }

    nrf_info_local_var = OpenAPI_nrf_info_create (
        served_udr_info ? served_udr_infoList : NULL,
        served_udr_info_list ? served_udr_info_listList : NULL,
        served_udm_info ? served_udm_infoList : NULL,
        served_udm_info_list ? served_udm_info_listList : NULL,
        served_ausf_info ? served_ausf_infoList : NULL,
        served_ausf_info_list ? served_ausf_info_listList : NULL,
        served_amf_info ? served_amf_infoList : NULL,
        served_amf_info_list ? served_amf_info_listList : NULL,
        served_smf_info ? served_smf_infoList : NULL,
        served_smf_info_list ? served_smf_info_listList : NULL,
        served_upf_info ? served_upf_infoList : NULL,
        served_upf_info_list ? served_upf_info_listList : NULL,
        served_pcf_info ? served_pcf_infoList : NULL,
        served_pcf_info_list ? served_pcf_info_listList : NULL,
        served_bsf_info ? served_bsf_infoList : NULL,
        served_bsf_info_list ? served_bsf_info_listList : NULL,
        served_chf_info ? served_chf_infoList : NULL,
        served_chf_info_list ? served_chf_info_listList : NULL,
        served_nef_info ? served_nef_infoList : NULL,
        served_nwdaf_info ? served_nwdaf_infoList : NULL,
        served_nwdaf_info_list ? served_nwdaf_info_listList : NULL,
        served_pcscf_info_list ? served_pcscf_info_listList : NULL,
        served_gmlc_info ? served_gmlc_infoList : NULL,
        served_lmf_info ? served_lmf_infoList : NULL,
        served_nf_info ? served_nf_infoList : NULL,
        served_hss_info_list ? served_hss_info_listList : NULL,
        served_udsf_info ? served_udsf_infoList : NULL,
        served_udsf_info_list ? served_udsf_info_listList : NULL,
        served_scp_info_list ? served_scp_info_listList : NULL,
        served_sepp_info_list ? served_sepp_info_listList : NULL,
        served_aanf_info_list ? served_aanf_info_listList : NULL,
        served5g_ddnmf_info ? served5g_ddnmf_infoList : NULL,
        served_mfaf_info_list ? served_mfaf_info_listList : NULL,
        served_easdf_info_list ? served_easdf_info_listList : NULL,
        served_dccf_info_list ? served_dccf_info_listList : NULL,
        served_mb_smf_info_list ? served_mb_smf_info_listList : NULL,
        served_tsctsf_info_list ? served_tsctsf_info_listList : NULL,
        served_mb_upf_info_list ? served_mb_upf_info_listList : NULL,
        served_trust_af_info ? served_trust_af_infoList : NULL,
        served_nssaaf_info ? served_nssaaf_infoList : NULL
    );

    return nrf_info_local_var;
end:
    if (served_udr_infoList) {
        OpenAPI_list_for_each(served_udr_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_udr_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_udr_infoList);
        served_udr_infoList = NULL;
    }
    if (served_udr_info_listList) {
        OpenAPI_list_for_each(served_udr_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_udr_info_listList);
        served_udr_info_listList = NULL;
    }
    if (served_udm_infoList) {
        OpenAPI_list_for_each(served_udm_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_udm_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_udm_infoList);
        served_udm_infoList = NULL;
    }
    if (served_udm_info_listList) {
        OpenAPI_list_for_each(served_udm_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_udm_info_listList);
        served_udm_info_listList = NULL;
    }
    if (served_ausf_infoList) {
        OpenAPI_list_for_each(served_ausf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_ausf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_ausf_infoList);
        served_ausf_infoList = NULL;
    }
    if (served_ausf_info_listList) {
        OpenAPI_list_for_each(served_ausf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_ausf_info_listList);
        served_ausf_info_listList = NULL;
    }
    if (served_amf_infoList) {
        OpenAPI_list_for_each(served_amf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_amf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_amf_infoList);
        served_amf_infoList = NULL;
    }
    if (served_amf_info_listList) {
        OpenAPI_list_for_each(served_amf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_amf_info_listList);
        served_amf_info_listList = NULL;
    }
    if (served_smf_infoList) {
        OpenAPI_list_for_each(served_smf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_smf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_smf_infoList);
        served_smf_infoList = NULL;
    }
    if (served_smf_info_listList) {
        OpenAPI_list_for_each(served_smf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_smf_info_listList);
        served_smf_info_listList = NULL;
    }
    if (served_upf_infoList) {
        OpenAPI_list_for_each(served_upf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_upf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_upf_infoList);
        served_upf_infoList = NULL;
    }
    if (served_upf_info_listList) {
        OpenAPI_list_for_each(served_upf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_upf_info_listList);
        served_upf_info_listList = NULL;
    }
    if (served_pcf_infoList) {
        OpenAPI_list_for_each(served_pcf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_pcf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_pcf_infoList);
        served_pcf_infoList = NULL;
    }
    if (served_pcf_info_listList) {
        OpenAPI_list_for_each(served_pcf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_pcf_info_listList);
        served_pcf_info_listList = NULL;
    }
    if (served_bsf_infoList) {
        OpenAPI_list_for_each(served_bsf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_bsf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_bsf_infoList);
        served_bsf_infoList = NULL;
    }
    if (served_bsf_info_listList) {
        OpenAPI_list_for_each(served_bsf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_bsf_info_listList);
        served_bsf_info_listList = NULL;
    }
    if (served_chf_infoList) {
        OpenAPI_list_for_each(served_chf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_chf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_chf_infoList);
        served_chf_infoList = NULL;
    }
    if (served_chf_info_listList) {
        OpenAPI_list_for_each(served_chf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_chf_info_listList);
        served_chf_info_listList = NULL;
    }
    if (served_nef_infoList) {
        OpenAPI_list_for_each(served_nef_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_nef_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_nef_infoList);
        served_nef_infoList = NULL;
    }
    if (served_nwdaf_infoList) {
        OpenAPI_list_for_each(served_nwdaf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_nwdaf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_nwdaf_infoList);
        served_nwdaf_infoList = NULL;
    }
    if (served_nwdaf_info_listList) {
        OpenAPI_list_for_each(served_nwdaf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_nwdaf_info_listList);
        served_nwdaf_info_listList = NULL;
    }
    if (served_pcscf_info_listList) {
        OpenAPI_list_for_each(served_pcscf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_pcscf_info_listList);
        served_pcscf_info_listList = NULL;
    }
    if (served_gmlc_infoList) {
        OpenAPI_list_for_each(served_gmlc_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_gmlc_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_gmlc_infoList);
        served_gmlc_infoList = NULL;
    }
    if (served_lmf_infoList) {
        OpenAPI_list_for_each(served_lmf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_lmf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_lmf_infoList);
        served_lmf_infoList = NULL;
    }
    if (served_nf_infoList) {
        OpenAPI_list_for_each(served_nf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_nf_infoList);
        served_nf_infoList = NULL;
    }
    if (served_hss_info_listList) {
        OpenAPI_list_for_each(served_hss_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_hss_info_listList);
        served_hss_info_listList = NULL;
    }
    if (served_udsf_infoList) {
        OpenAPI_list_for_each(served_udsf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_udsf_info_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_udsf_infoList);
        served_udsf_infoList = NULL;
    }
    if (served_udsf_info_listList) {
        OpenAPI_list_for_each(served_udsf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_udsf_info_listList);
        served_udsf_info_listList = NULL;
    }
    if (served_scp_info_listList) {
        OpenAPI_list_for_each(served_scp_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_scp_info_list_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_scp_info_listList);
        served_scp_info_listList = NULL;
    }
    if (served_sepp_info_listList) {
        OpenAPI_list_for_each(served_sepp_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nrf_info_served_sepp_info_list_value_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_sepp_info_listList);
        served_sepp_info_listList = NULL;
    }
    if (served_aanf_info_listList) {
        OpenAPI_list_for_each(served_aanf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_aanf_info_listList);
        served_aanf_info_listList = NULL;
    }
    if (served5g_ddnmf_infoList) {
        OpenAPI_list_for_each(served5g_ddnmf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_model_5_g_ddnmf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served5g_ddnmf_infoList);
        served5g_ddnmf_infoList = NULL;
    }
    if (served_mfaf_info_listList) {
        OpenAPI_list_for_each(served_mfaf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_mfaf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_mfaf_info_listList);
        served_mfaf_info_listList = NULL;
    }
    if (served_easdf_info_listList) {
        OpenAPI_list_for_each(served_easdf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_easdf_info_listList);
        served_easdf_info_listList = NULL;
    }
    if (served_dccf_info_listList) {
        OpenAPI_list_for_each(served_dccf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_dccf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_dccf_info_listList);
        served_dccf_info_listList = NULL;
    }
    if (served_mb_smf_info_listList) {
        OpenAPI_list_for_each(served_mb_smf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_mb_smf_info_listList);
        served_mb_smf_info_listList = NULL;
    }
    if (served_tsctsf_info_listList) {
        OpenAPI_list_for_each(served_tsctsf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_tsctsf_info_listList);
        served_tsctsf_info_listList = NULL;
    }
    if (served_mb_upf_info_listList) {
        OpenAPI_list_for_each(served_mb_upf_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_mb_upf_info_listList);
        served_mb_upf_info_listList = NULL;
    }
    if (served_trust_af_infoList) {
        OpenAPI_list_for_each(served_trust_af_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_trust_af_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_trust_af_infoList);
        served_trust_af_infoList = NULL;
    }
    if (served_nssaaf_infoList) {
        OpenAPI_list_for_each(served_nssaaf_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nssaaf_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(served_nssaaf_infoList);
        served_nssaaf_infoList = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_t *OpenAPI_nrf_info_copy(OpenAPI_nrf_info_t *dst, OpenAPI_nrf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_convertToJSON() failed");
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

    OpenAPI_nrf_info_free(dst);
    dst = OpenAPI_nrf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

