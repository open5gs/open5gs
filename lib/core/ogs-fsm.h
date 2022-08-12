/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_FSM_H
#define OGS_FSM_H

#ifdef __cplusplus
extern "C" {
#endif

extern const char *OGS_FSM_NAME_INIT_SIG;
extern const char *OGS_FSM_NAME_ENTRY_SIG;
extern const char *OGS_FSM_NAME_EXIT_SIG;

typedef enum {
    OGS_FSM_ENTRY_SIG,
    OGS_FSM_EXIT_SIG,
    OGS_FSM_USER_SIG
} ogs_fsm_signal_e;

typedef void (*ogs_fsm_handler_t)(void *sm, void *event);

typedef struct _ogs_fsm_t {
    ogs_fsm_handler_t init;
    ogs_fsm_handler_t fini;
    ogs_fsm_handler_t state;
} ogs_fsm_t;

void ogs_fsm_init(void *fsm, void *init, void *fini, void *event);
void ogs_fsm_tran(void *fsm, void *state, void *event);
void ogs_fsm_dispatch(void *fsm, void *event);
void ogs_fsm_fini(void *fsm, void *event);

#define OGS_FSM_TRAN(__s, __target) \
    ((ogs_fsm_t *)__s)->state = (ogs_fsm_handler_t)(__target)

#define OGS_FSM_STATE(__s) \
    (((ogs_fsm_t *)__s)->state)

#define OGS_FSM_CHECK(__s, __f) \
    (OGS_FSM_STATE(__s) == (ogs_fsm_handler_t)__f)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OGS_FSM_H */
