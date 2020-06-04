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

void ogs_fsm_init(void *sm, void *event)
{
    ogs_fsm_t *s = sm;
    fsm_event_t *e = event;

    if (s->init != NULL) {
        (*s->init)(s, e);
        if (s->init != s->state) {
            if (e) {
                e->id = OGS_FSM_ENTRY_SIG;
                (*s->state)(s, e);
            } else {
                (*s->state)(s, &entry_event);
            }
        }
    }
}

void ogs_fsm_dispatch(void *sm, void *event)
{
    ogs_fsm_t *s = sm;
    fsm_event_t *e = event;
    ogs_fsm_handler_t tmp = s->state;

    if (e)
        (*tmp)(s, e);

    if (s->state != tmp) {
        if (e) {
            e->id = OGS_FSM_EXIT_SIG;
            (*tmp)(s, e);
        } else {
            (*tmp)(s, &exit_event);
        }
        if (e) {
            e->id = OGS_FSM_ENTRY_SIG;
            (*s->state)(s, e);
        } else {
            (*s->state)(s, &entry_event);
        }
    }
}

void ogs_fsm_fini(void *sm, void *event)
{
    ogs_fsm_t *s = sm;
    fsm_event_t *e = event;

    if (s->fini != s->state) {
        if (e) {
            e->id = OGS_FSM_EXIT_SIG;
            (*s->state)(s, e);
        } else {
            (*s->state)(s, &exit_event);
        }
    }

    if (s->fini != NULL) {
        (*s->fini)(s, e);
    }

    s->state = s->init;
}
