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

#include "ogs-core.h"

typedef struct fsm_event_s {
    int id;
} fsm_event_t;

static fsm_event_t entry_event = {
    OGS_FSM_ENTRY_SIG,
};
static fsm_event_t exit_event = {
    OGS_FSM_EXIT_SIG,
};

const char *OGS_FSM_NAME_INIT_SIG = "INIT";
const char *OGS_FSM_NAME_ENTRY_SIG = "ENTRY";
const char *OGS_FSM_NAME_EXIT_SIG = "EXIT";

static void fsm_entry(ogs_fsm_t *sm, ogs_fsm_handler_t state, fsm_event_t *e)
{
    ogs_assert(sm);
    ogs_assert(state);

    if (e) {
        e->id = OGS_FSM_ENTRY_SIG;
        (*state)(sm, e);
    } else {
        (*state)(sm, &entry_event);
    }
}

static void fsm_exit(ogs_fsm_t *sm, ogs_fsm_handler_t state, fsm_event_t *e)
{
    ogs_assert(sm);
    ogs_assert(state);

    if (e) {
        e->id = OGS_FSM_EXIT_SIG;
        (*state)(sm, e);
    } else {
        (*state)(sm, &exit_event);
    }
}

static void fsm_change(
        ogs_fsm_t *sm,
        ogs_fsm_handler_t oldstate,
        ogs_fsm_handler_t newstate,
        fsm_event_t *e)
{
    ogs_assert(sm);
    ogs_assert(oldstate);
    ogs_assert(newstate);

    fsm_exit(sm, oldstate, e);
    fsm_entry(sm, newstate, e);
}

void ogs_fsm_init(void *fsm, void *init, void *fini, void *event)
{
    ogs_fsm_t *sm = fsm;
    fsm_event_t *e = event;

    ogs_assert(sm);

    sm->init = sm->state = init;
    sm->fini = fini;

    if (sm->init) {
        (*sm->init)(sm, e);

        if (sm->init != sm->state) {
            ogs_assert(sm->state);
            fsm_entry(sm, sm->state, e);
        }
    }
}

void ogs_fsm_tran(void *fsm, void *state, void *event)
{
    ogs_fsm_t *sm = fsm;
    fsm_event_t *e = event;
    ogs_fsm_handler_t tmp = NULL;

    ogs_assert(sm);

    tmp = sm->state;
    ogs_assert(tmp);

    sm->state = state;
    ogs_assert(sm->state);

    if (sm->state != tmp)
        fsm_change(fsm, tmp, sm->state, e);
}

void ogs_fsm_dispatch(void *fsm, void *event)
{
    ogs_fsm_t *sm = fsm;
    fsm_event_t *e = event;
    ogs_fsm_handler_t tmp = NULL;

    ogs_assert(sm);

    tmp = sm->state;
    ogs_assert(tmp);

    if (e)
        (*tmp)(sm, e);

    if (sm->state != tmp)
        fsm_change(fsm, tmp, sm->state, e);
}

void ogs_fsm_fini(void *fsm, void *event)
{
    ogs_fsm_t *sm = fsm;
    fsm_event_t *e = event;

    ogs_assert(sm);

    if (sm->fini != sm->state) {
        ogs_assert(sm->state);
        fsm_exit(sm, sm->state, e);

        if (sm->fini)
            (*sm->fini)(sm, e);
    }

    sm->init = sm->state = sm->fini = NULL;
}
