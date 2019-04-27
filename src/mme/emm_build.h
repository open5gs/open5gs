#ifndef __EMM_BUILD_H__
#define __EMM_BUILD_H__

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int emm_build_attach_accept(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue, ogs_pkbuf_t *esmbuf);
int emm_build_attach_reject(
        ogs_pkbuf_t **emmbuf, nas_emm_cause_t emm_cause, ogs_pkbuf_t *esmbuf);

int emm_build_identity_request(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);
int emm_build_security_mode_command(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);

int emm_build_authentication_request(
        ogs_pkbuf_t **emmbuf, e_utran_vector_t *e_utran_vector);
int emm_build_authentication_reject(ogs_pkbuf_t **emmbuf);

int emm_build_detach_accept(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);

int emm_build_tau_accept(ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);
int emm_build_tau_reject(ogs_pkbuf_t **emmbuf, 
        nas_emm_cause_t emm_cause,mme_ue_t *mme_ue);

int emm_build_service_reject(ogs_pkbuf_t **emmbuf, 
        nas_emm_cause_t emm_cause, mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EMM_BUILD_H__ */
