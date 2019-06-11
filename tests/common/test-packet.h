#ifndef TEST_PACKET_H
#define TEST_PACKET_H

#include "asn1c/s1ap-message.h"

#ifdef __cplusplus
extern "C" {
#endif

int testpacket_init();
int testpacket_final();

ogs_socknode_t *testenb_s1ap_client(const char *ipstr);
ogs_pkbuf_t *testenb_s1ap_read(ogs_socknode_t *node);
int testenb_s1ap_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf);
void testenb_s1ap_close(ogs_socknode_t *node);

ogs_socknode_t *testenb_gtpu_server(const char *ipstr);
ogs_pkbuf_t *testenb_gtpu_read(ogs_socknode_t *node);
int testenb_gtpu_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf);
void testenb_gtpu_close(ogs_socknode_t *node);

int tests1ap_build_setup_req(
        ogs_pkbuf_t **pkbuf, S1AP_ENB_ID_PR present, uint32_t enb_id,
        int tac, uint16_t mcc, uint16_t mnc, uint16_t mnc_len);
int tests1ap_build_initial_ue_msg(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_identity_response(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_authentication_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_authentication_failure(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_security_mode_complete(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_esm_information_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_ue_capability_info_indication(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_initial_context_setup_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t ebi, uint32_t teid, const char *ipstr);
int tests1ap_build_initial_context_setup_failure(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_attach_complete(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_emm_status(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_detach_request(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_ue_context_release_request(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_ue_context_release_complete(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_service_request(ogs_pkbuf_t **pkbuf,
        uint32_t enb_ue_s1ap_id, uint8_t seq,
        uint16_t mac, uint32_t m_tmsi);
int tests1ap_build_tau_request(ogs_pkbuf_t **pkbuf, int i,
    uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id, uint8_t active_flag,
    uint32_t m_tmsi, uint8_t seq, uint32_t mac, uint8_t *knas_int);
int tests1ap_build_pdn_connectivity_request(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_pdn_disconnectivity_request(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_e_rab_setup_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t ebi, uint32_t teid, const char *ipstr);
int tests1ap_build_e_rab_modify_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_e_rab_release_response(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_activate_default_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_activate_dedicated_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_modify_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_deactivate_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_path_switch_request(
        ogs_pkbuf_t **pkbuf, int target, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        int num_of_bearer, uint8_t ebi, uint32_t teid,
        const char *ipstr1, const char *ipstr2);
int tests1ap_build_handover_required(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_failure(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_request_ack(
        ogs_pkbuf_t **pkbuf, int target,
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        int num_of_bearer, uint8_t ebi, uint32_t teid,
        const char *ipstr1, const char *ipstr2);
int tests1ap_build_handover_request_ack_static(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_enb_status_transfer(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_enb_configuration_transfer(
        ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_notify(ogs_pkbuf_t **pkbuf, int i);
int tests1ap_build_handover_cancel(ogs_pkbuf_t **pkbuf, int i);

int tests1ap_build_s1_reset(ogs_pkbuf_t **pkbuf, int i);

int testgtpu_build_ping(ogs_pkbuf_t **sendbuf,
        const char *src_ip, const char *dst_ip);
int testgtpu_build_slacc_rs(ogs_pkbuf_t **sendbuf, int i);

#ifdef __cplusplus
}
#endif

#endif /* TEST_PACKET_H */
