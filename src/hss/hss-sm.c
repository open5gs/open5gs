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

#include "hss-sm.h"
#include "hss-context.h"
#include "hss-event.h"
#include "hss-timer.h"

#define DB_POLLING_TIME ogs_time_from_msec(100)

static ogs_timer_t *t_db_polling = NULL;

void hss_state_initial(ogs_fsm_t *s, hss_event_t *e)
{
    hss_sm_debug(e);

    ogs_assert(s);

#if MONGOC_CHECK_VERSION(1, 9, 0)
    if (hss_self()->use_mongodb_change_stream) {
        ogs_dbi_collection_watch_init();

        t_db_polling = ogs_timer_add(ogs_app()->timer_mgr,
                hss_timer_dbi_poll_change_stream, 0);
        ogs_assert(t_db_polling);
        ogs_timer_start(t_db_polling, DB_POLLING_TIME);

        OGS_FSM_TRAN(s, &hss_state_operational);
    }
#endif
}

void hss_state_final(ogs_fsm_t *s, hss_event_t *e)
{
    hss_sm_debug(e);

    if (t_db_polling)
        ogs_timer_delete(t_db_polling);

    ogs_assert(s);
}

void hss_state_operational(ogs_fsm_t *s, hss_event_t *e)
{
    hss_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        if (t_db_polling) {
            ogs_timer_stop(t_db_polling);
        }
        break;

    case HSS_EVENT_DBI_POLL_TIMER:
        ogs_assert(e);

        switch(e->timer_id) {
        case HSS_TIMER_DBI_POLL_CHANGE_STREAM:
            hss_db_poll_change_stream();
            ogs_timer_start(t_db_polling, DB_POLLING_TIME);
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    case HSS_EVENT_DBI_MESSAGE:
        ogs_assert(e);

        ogs_assert(e->dbi.document);
        hss_handle_change_event(e->dbi.document);

        bson_destroy(e->dbi.document);
        break;

    default:
        ogs_error("No handler for event %s", hss_event_get_name(e));
        break;
    }
}
