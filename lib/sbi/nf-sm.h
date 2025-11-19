/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(OGS_SBI_INSIDE) && !defined(OGS_SBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SBI_NF_SM_H
#define OGS_SBI_NF_SM_H

#ifdef __cplusplus
extern "C" {
#endif

void ogs_sbi_nf_fsm_init(ogs_sbi_nf_instance_t *nf_instance);
void ogs_sbi_nf_fsm_fini(ogs_sbi_nf_instance_t *nf_instance);

void ogs_sbi_nf_state_initial(ogs_fsm_t *s, ogs_event_t *e);
void ogs_sbi_nf_state_final(ogs_fsm_t *s, ogs_event_t *e);
void ogs_sbi_nf_state_will_register(ogs_fsm_t *s, ogs_event_t *e);
void ogs_sbi_nf_state_registered(ogs_fsm_t *s, ogs_event_t *e);
void ogs_sbi_nf_state_de_registered(ogs_fsm_t *s, ogs_event_t *e);
void ogs_sbi_nf_state_exception(ogs_fsm_t *s, ogs_event_t *e);

#define ogs_sbi_sm_debug(__e) \
    ogs_debug("%s(): %s", __func__, ogs_event_get_name(__e))

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_NF_SM_H */
