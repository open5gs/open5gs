/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_SBI_TIMER_H
#define OGS_SBI_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

void ogs_timer_nf_instance_registration_interval(void *data);
void ogs_timer_nf_instance_heartbeat_interval(void *data);
void ogs_timer_nf_instance_no_heartbeat(void *data);
void ogs_timer_nf_instance_validity(void *data);
void ogs_timer_subscription_validity(void *data);
void ogs_timer_subscription_patch(void *data);
void ogs_timer_sbi_client_wait_expire(void *data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_TIMER_H */
