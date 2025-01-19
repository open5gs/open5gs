#include "ogs-app.h"
#include "context.h"

#include "metrics.h"

typedef struct smf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
} smf_metrics_spec_def_t;

/* Helper generic functions: */
static int smf_metrics_init_inst(ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs,
        unsigned int len, unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int smf_metrics_free_inst(ogs_metrics_inst_t **inst,
        unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        ogs_metrics_inst_free(inst[i]);
    memset(inst, 0, sizeof(inst[0]) * len);
    return OGS_OK;
}

static int smf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, smf_metrics_spec_def_t *src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val, src[i].num_labels, src[i].labels,
                NULL);
    }
    return OGS_OK;
}

/* GLOBAL */
ogs_metrics_spec_t *smf_metrics_spec_global[_SMF_METR_GLOB_MAX];
ogs_metrics_inst_t *smf_metrics_inst_global[_SMF_METR_GLOB_MAX];
smf_metrics_spec_def_t smf_metrics_spec_def_global[_SMF_METR_GLOB_MAX] = {
/* Global Counters: */
[SMF_METR_GLOB_CTR_GTP_NEW_NODE_FAILED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gtp_new_node_failed",
    .description = "Unable to allocate new GTP (peer) Node",
},
[SMF_METR_GLOB_CTR_GN_RX_PARSE_FAILED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gn_rx_parse_failed",
    .description = "Received GTPv1C messages discarded due to parsing failure",
},
[SMF_METR_GLOB_CTR_GN_RX_CREATEPDPCTXREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gn_rx_createpdpcontextreq",
    .description = "Received GTPv1C CreatePDPContextRequest messages",
},
[SMF_METR_GLOB_CTR_GN_RX_DELETEPDPCTXREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "gn_rx_deletepdpcontextreq",
    .description = "Received GTPv1C DeletePDPContextRequest messages",
},
[SMF_METR_GLOB_CTR_S5C_RX_PARSE_FAILED] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "s5c_rx_parse_failed",
    .description = "Received GTPv2C messages discarded due to parsing failure",
},
[SMF_METR_GLOB_CTR_S5C_RX_CREATESESSIONREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "s5c_rx_createsession",
    .description = "Received GTPv2C CreateSessionRequest messages",
},
[SMF_METR_GLOB_CTR_S5C_RX_DELETESESSIONREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "s5c_rx_deletesession",
    .description = "Received GTPv2C DeleteSessionRequest messages",
},
[SMF_METR_GLOB_CTR_SM_N4SESSIONESTABREQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_smffunction_sm_n4sessionestabreq",
    .description = "Number of requested N4 session establishments evidented by SMF",
},
[SMF_METR_GLOB_CTR_SM_N4SESSIONREPORT] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_smffunction_sm_n4sessionreport",
    .description = "Number of requested N4 session reports evidented by SMF",
},
[SMF_METR_GLOB_CTR_SM_N4SESSIONREPORTSUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_smffunction_sm_n4sessionreportsucc",
    .description = "Number of successful N4 session reports evidented by SMF",
},
/* Global Gauges: */
[SMF_METR_GLOB_GAUGE_UES_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "ues_active",
    .description = "Active User Equipments",
},
[SMF_METR_GLOB_GAUGE_BEARERS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "bearers_active",
    .description = "Active Bearers",
},
[SMF_METR_GLOB_GAUGE_GTP1_PDPCTXS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gtp1_pdpctxs_active",
    .description = "Active GTPv1 PDP Contexts (GGSN)",
},
[SMF_METR_GLOB_GAUGE_GTP2_SESSIONS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gtp2_sessions_active",
    .description = "Active GTPv2 Sessions (PGW)",
},
[SMF_METR_GLOB_GAUGE_GTP_PEERS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "gtp_peers_active",
    .description = "Active GTP peers",
},
[SMF_METR_GLOB_GAUGE_PFCP_SESSIONS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "pfcp_sessions_active",
    .description = "Active PFCP Sessions",
},
[SMF_METR_GLOB_GAUGE_PFCP_PEERS_ACTIVE] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "pfcp_peers_active",
    .description = "Active PFCP peers",
},
};
int smf_metrics_init_inst_global(void)
{
    return smf_metrics_init_inst(smf_metrics_inst_global, smf_metrics_spec_global,
                _SMF_METR_GLOB_MAX, 0, NULL);
}
int smf_metrics_free_inst_global(void)
{
    return smf_metrics_free_inst(smf_metrics_inst_global, _SMF_METR_GLOB_MAX);
}

/* GTP NODE: */
const char *labels_gtp_node[] = {
    "addr"
};
#define SMF_METR_GTP_NODE_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_gtp_node), \
        .labels = labels_gtp_node, \
    },
ogs_metrics_spec_t *smf_metrics_spec_gtp_node[_SMF_METR_GTP_NODE_MAX];
smf_metrics_spec_def_t smf_metrics_spec_def_gtp_node[_SMF_METR_GTP_NODE_MAX] = {
/* Global Counters: */
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_GN_RX_PARSE_FAILED,
    "gtp_node_gn_rx_parse_failed",
    "Received GTPv1C messages discarded due to parsing failure")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_GN_RX_CREATEPDPCTXREQ,
    "gtp_node_gn_rx_createpdpcontextreq",
    "Received GTPv1C CreatePDPContextRequest messages")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_GN_RX_DELETEPDPCTXREQ,
    "gtp_node_gn_rx_deletepdpcontextreq",
    "Received GTPv1C DeletePDPContextRequest messages")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_S5C_RX_PARSE_FAILED,
    "gtp_node_s5c_rx_parse_failed",
    "Received GTPv2C messages discarded due to parsing failure")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_S5C_RX_CREATESESSIONREQ,
    "gtp_node_s5c_rx_createsession",
    "Received GTPv2C CreateSessionRequest messages")
SMF_METR_GTP_NODE_CTR_ENTRY(
    SMF_METR_GTP_NODE_CTR_S5C_RX_DELETESESSIONREQ,
    "gtp_node_s5c_rx_deletesession",
    "Received GTPv2C DeleteSessionRequest messages")
};
int smf_metrics_init_inst_gtp_node(ogs_metrics_inst_t **inst, const char *addr)
{
    return smf_metrics_init_inst(inst,
                smf_metrics_spec_gtp_node, _SMF_METR_GTP_NODE_MAX,
                smf_metrics_spec_def_gtp_node->num_labels, (const char *[]){ addr });
}
int smf_metrics_free_inst_gtp_node(ogs_metrics_inst_t **inst)
{
    return smf_metrics_free_inst(inst, _SMF_METR_GTP_NODE_MAX);
}

/* BY SLICE */
const char *labels_slice[] = {
    "plmnid",
    "snssai"
};

#define SMF_METR_BY_SLICE_GAUGE_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_GAUGE, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_slice), \
        .labels = labels_slice, \
    },
#define SMF_METR_BY_SLICE_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_slice), \
        .labels = labels_slice, \
    },
ogs_metrics_spec_t *smf_metrics_spec_by_slice[_SMF_METR_BY_SLICE_MAX];
ogs_hash_t *metrics_hash_by_slice = NULL;   /* hash table for SLICE labels */
smf_metrics_spec_def_t smf_metrics_spec_def_by_slice[_SMF_METR_BY_SLICE_MAX] = {
/* Gauges: */
SMF_METR_BY_SLICE_GAUGE_ENTRY(
    SMF_METR_GAUGE_SM_SESSIONNBR,
    "fivegs_smffunction_sm_sessionnbr",
    "Active Sessions")
SMF_METR_BY_SLICE_CTR_ENTRY(
    SMF_METR_CTR_SM_PDUSESSIONCREATIONREQ,
    "fivegs_smffunction_sm_pdusessioncreationreq",
    "Number of PDU sessions requested to be created by the SMF")
SMF_METR_BY_SLICE_CTR_ENTRY(
    SMF_METR_CTR_SM_PDUSESSIONCREATIONSUCC,
    "fivegs_smffunction_sm_pdusessioncreationsucc",
    "Number of PDU sessions successfully created by the SMF")
};
void smf_metrics_init_by_slice(void);
int smf_metrics_free_inst_by_slice(ogs_metrics_inst_t **inst);
typedef struct smf_metric_key_by_slice_s {
    ogs_plmn_id_t               plmn_id;
    ogs_s_nssai_t               snssai;
    smf_metric_type_by_slice_t  t;
} smf_metric_key_by_slice_t;

void smf_metrics_init_by_slice(void)
{
    metrics_hash_by_slice = ogs_hash_make();
    ogs_assert(metrics_hash_by_slice);
}

void smf_metrics_inst_by_slice_add(ogs_plmn_id_t *plmn,
        ogs_s_nssai_t *snssai, smf_metric_type_by_slice_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    smf_metric_key_by_slice_t *slice_key;

    slice_key = ogs_calloc(1, sizeof(*slice_key));
    ogs_assert(slice_key);

    if (plmn) {
        slice_key->plmn_id = *plmn;
    }

    if (snssai) {
        slice_key->snssai = *snssai;
    } else {
        slice_key->snssai.sst = 0;
        slice_key->snssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    }

    slice_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_slice,
            slice_key, sizeof(*slice_key));

    if (!metrics) {
        char plmn_id[OGS_PLMNIDSTRLEN] = "";
        char *s_nssai = NULL;

        if (plmn) {
            ogs_plmn_id_to_string(plmn, plmn_id);
        }

        if (snssai) {
            s_nssai = ogs_sbi_s_nssai_to_string(snssai);
        } else {
            s_nssai = ogs_strdup("");
        }

        metrics = ogs_metrics_inst_new(smf_metrics_spec_by_slice[t],
                smf_metrics_spec_def_by_slice->num_labels,
                (const char *[]){ plmn_id, s_nssai });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_slice,
                slice_key, sizeof(*slice_key), metrics);

        if (s_nssai)
            ogs_free(s_nssai);
    } else {
        ogs_free(slice_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int smf_metrics_free_inst_by_slice(ogs_metrics_inst_t **inst)
{
    return smf_metrics_free_inst(inst, _SMF_METR_BY_SLICE_MAX);
}

/* BY SLICE and 5QI */
const char *labels_5qi[] = {
    "plmnid",
    "snssai",
    "fiveqi"
};

#define SMF_METR_BY_5QI_GAUGE_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_GAUGE, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_5qi), \
        .labels = labels_5qi, \
    },
ogs_metrics_spec_t *smf_metrics_spec_by_5qi[_SMF_METR_BY_5QI_MAX];
ogs_hash_t *metrics_hash_by_5qi = NULL;   /* hash table for 5QI label */
smf_metrics_spec_def_t smf_metrics_spec_def_by_5qi[_SMF_METR_BY_5QI_MAX] = {
/* Gauges: */
SMF_METR_BY_5QI_GAUGE_ENTRY(
    SMF_METR_GAUGE_SM_QOSFLOWNBR,
    "fivegs_smffunction_sm_qos_flow_nbr",
    "Number of QoS flows at the SMF")
};
void smf_metrics_init_by_5qi(void);
int smf_metrics_free_inst_by_5qi(ogs_metrics_inst_t **inst);
typedef struct smf_metric_key_by_5qi_s {
    ogs_plmn_id_t               plmn_id;
    ogs_s_nssai_t               snssai;
    uint8_t                     fiveqi;
    smf_metric_type_by_5qi_t    t;
} smf_metric_key_by_5qi_t;

void smf_metrics_init_by_5qi(void)
{
    metrics_hash_by_5qi = ogs_hash_make();
    ogs_assert(metrics_hash_by_5qi);
}
void smf_metrics_inst_by_5qi_add(ogs_plmn_id_t *plmn,
        ogs_s_nssai_t *snssai, uint8_t fiveqi,
        smf_metric_type_by_5qi_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    smf_metric_key_by_5qi_t *fiveqi_key;

    fiveqi_key = ogs_calloc(1, sizeof(*fiveqi_key));
    ogs_assert(fiveqi_key);

    if (plmn) {
        fiveqi_key->plmn_id = *plmn;
    }

    if (snssai) {
        fiveqi_key->snssai = *snssai;
    } else {
        fiveqi_key->snssai.sst = 0;
        fiveqi_key->snssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    }

    fiveqi_key->fiveqi = fiveqi;
    fiveqi_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_5qi,
            fiveqi_key, sizeof(*fiveqi_key));

    if (!metrics) {
        char plmn_id[OGS_PLMNIDSTRLEN] = "";
        char *s_nssai = NULL;
        char fiveqi_str[4];

        if (plmn) {
            ogs_plmn_id_to_string(plmn, plmn_id);
        }

        if (snssai) {
            s_nssai = ogs_sbi_s_nssai_to_string(snssai);
        } else {
            s_nssai = ogs_strdup("");
        }

        ogs_snprintf(fiveqi_str, sizeof(fiveqi_str), "%d", fiveqi);

        metrics = ogs_metrics_inst_new(smf_metrics_spec_by_5qi[t],
                smf_metrics_spec_def_by_5qi->num_labels,
                (const char *[]){ plmn_id, s_nssai, fiveqi_str });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_5qi,
                fiveqi_key, sizeof(*fiveqi_key), metrics);

        if (s_nssai)
            ogs_free(s_nssai);
    } else {
        ogs_free(fiveqi_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int smf_metrics_free_inst_by_5qi(ogs_metrics_inst_t **inst)
{
    return smf_metrics_free_inst(inst, _SMF_METR_BY_5QI_MAX);
}

/* BY CAUSE */
const char *labels_cause[] = {
    "cause"
};

#define SMF_METR_BY_CAUSE_CTR_ENTRY(_id, _name, _desc) \
    [_id] = { \
        .type = OGS_METRICS_METRIC_TYPE_COUNTER, \
        .name = _name, \
        .description = _desc, \
        .num_labels = OGS_ARRAY_SIZE(labels_cause), \
        .labels = labels_cause, \
    },
ogs_metrics_spec_t *smf_metrics_spec_by_cause[_SMF_METR_BY_CAUSE_MAX];
ogs_hash_t *metrics_hash_by_cause = NULL;   /* hash table for CAUSE labels */
smf_metrics_spec_def_t smf_metrics_spec_def_by_cause[_SMF_METR_BY_CAUSE_MAX] = {
/* Counters: */
SMF_METR_BY_CAUSE_CTR_ENTRY(
    SMF_METR_CTR_SM_N4SESSIONESTABFAIL,
    "fivegs_smffunction_sm_n4sessionestabfail",
    "Number of failed N4 session establishments evidented by SMF")
SMF_METR_BY_CAUSE_CTR_ENTRY(
    SMF_METR_CTR_SM_PDUSESSIONCREATIONFAIL,
    "fivegs_smffunction_sm_pdusessioncreationfail",
    "Number of PDU sessions failed to be created by the SMF")
};
void smf_metrics_init_by_cause(void);
int smf_metrics_free_inst_by_cause(ogs_metrics_inst_t **inst);
typedef struct smf_metric_key_by_cause_s {
    int                         cause;
    smf_metric_type_by_cause_t  t;
} smf_metric_key_by_cause_t;

void smf_metrics_init_by_cause(void)
{
    metrics_hash_by_cause = ogs_hash_make();
    ogs_assert(metrics_hash_by_cause);
}

void smf_metrics_inst_by_cause_add(int cause,
        smf_metric_type_by_cause_t t, int val)
{
    ogs_metrics_inst_t *metrics = NULL;
    smf_metric_key_by_cause_t *cause_key;

    cause_key = ogs_calloc(1, sizeof(*cause_key));
    ogs_assert(cause_key);

    cause_key->cause = cause;
    cause_key->t = t;

    metrics = ogs_hash_get(metrics_hash_by_cause,
            cause_key, sizeof(*cause_key));

    if (!metrics) {
        char cause_str[4];
        ogs_snprintf(cause_str, sizeof(cause_str), "%d", cause);

        metrics = ogs_metrics_inst_new(smf_metrics_spec_by_cause[t],
                smf_metrics_spec_def_by_cause->num_labels,
                (const char *[]){ cause_str });

        ogs_assert(metrics);
        ogs_hash_set(metrics_hash_by_cause,
                cause_key, sizeof(*cause_key), metrics);
    } else {
        ogs_free(cause_key);
    }

    ogs_metrics_inst_add(metrics, val);
}

int smf_metrics_free_inst_by_cause(ogs_metrics_inst_t **inst)
{
    return smf_metrics_free_inst(inst, _SMF_METR_BY_CAUSE_MAX);
}

void smf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_init();

    smf_metrics_init_spec(ctx, smf_metrics_spec_global, smf_metrics_spec_def_global,
            _SMF_METR_GLOB_MAX);

    smf_metrics_init_spec(ctx, smf_metrics_spec_gtp_node, smf_metrics_spec_def_gtp_node,
            _SMF_METR_GTP_NODE_MAX);

    smf_metrics_init_spec(ctx, smf_metrics_spec_by_slice,
            smf_metrics_spec_def_by_slice, _SMF_METR_BY_SLICE_MAX);
    smf_metrics_init_spec(ctx, smf_metrics_spec_by_5qi,
            smf_metrics_spec_def_by_5qi, _SMF_METR_BY_5QI_MAX);
    smf_metrics_init_spec(ctx, smf_metrics_spec_by_cause,
            smf_metrics_spec_def_by_cause, _SMF_METR_BY_CAUSE_MAX);

    smf_metrics_init_inst_global();
    smf_metrics_init_by_slice();
    smf_metrics_init_by_5qi();
    smf_metrics_init_by_cause();
}

void smf_metrics_final(void)
{
    ogs_hash_index_t *hi;

    if (metrics_hash_by_slice) {
        for (hi = ogs_hash_first(metrics_hash_by_slice); hi; hi = ogs_hash_next(hi)) {
            smf_metric_key_by_slice_t *key =
                (smf_metric_key_by_slice_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_slice, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric itself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_slice);
    }
    if (metrics_hash_by_5qi) {
        for (hi = ogs_hash_first(metrics_hash_by_5qi); hi; hi = ogs_hash_next(hi)) {
            smf_metric_key_by_5qi_t *key =
                (smf_metric_key_by_5qi_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_5qi, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric itself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_5qi);
    }
    if (metrics_hash_by_cause) {
        for (hi = ogs_hash_first(metrics_hash_by_cause); hi; hi = ogs_hash_next(hi)) {
            smf_metric_key_by_cause_t *key =
                (smf_metric_key_by_cause_t *)ogs_hash_this_key(hi);
            //void *val = ogs_hash_this_val(hi);

            ogs_hash_set(metrics_hash_by_cause, key, sizeof(*key), NULL);

            ogs_free(key);
            /* don't free val (metric itself) -
             * it will be free'd by ogs_metrics_context_final() */
            //ogs_free(val);
        }
        ogs_hash_destroy(metrics_hash_by_cause);
    }

    ogs_metrics_context_final();
}
