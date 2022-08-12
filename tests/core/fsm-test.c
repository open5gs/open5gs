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
#include "core/abts.h"

enum bomb_signal_t {
    UP_SIG = OGS_FSM_USER_SIG,
    DOWN_SIG,
    ARM_SIG
};

typedef struct _tick_event_t {
    int id;
} tick_event_t;

typedef struct _bomb_t {
    ogs_fsm_t fsm;
    uint8_t timeout;
    uint8_t code;
    uint8_t defuse;
} bomb_t;

void bomb_initial(bomb_t *s, tick_event_t *e);
void bomb_setting(bomb_t *s, tick_event_t *e);
void bomb_timing(bomb_t *s, tick_event_t *e);

void bomb_initial(bomb_t *s, tick_event_t *e)
{
    s->timeout = 10;
    OGS_FSM_TRAN(s, &bomb_setting);
}

void bomb_setting(bomb_t *s, tick_event_t *e)
{
    switch (e->id) {
    case UP_SIG:
        if (s->timeout < 12) {
            ++s->timeout;
        }
        break;
    case DOWN_SIG:
        if (s->timeout > 8) {
            --s->timeout;
        }
        break;
    case ARM_SIG:
        OGS_FSM_TRAN(s, &bomb_timing);
        break;
    }
}

void bomb_timing(bomb_t *s, tick_event_t *e)
{
    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        s->code = 0;
        break;
    case UP_SIG:
        s->code <<= 1;
        s->code |= 1;
        break;
    case DOWN_SIG:
        s->code <<= 1;
        break;
    case ARM_SIG:
        if (s->code == s->defuse) {
            OGS_FSM_TRAN(s, &bomb_setting);
            break;
        }
    }
}

static void test1_func(abts_case *tc, void *data)
{
    bomb_t bomb;
    tick_event_t tick_event;

    ogs_fsm_init(&bomb, &bomb_initial, 0, 0);
    bomb.defuse = 14;
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 10, bomb.timeout);

    tick_event.id = UP_SIG;
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 11, bomb.timeout);
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 12, bomb.timeout);
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 12, bomb.timeout);

    tick_event.id = DOWN_SIG;
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 11, bomb.timeout);
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 10, bomb.timeout);
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 9, bomb.timeout);
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 8, bomb.timeout);
    ogs_fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 8, bomb.timeout);

    tick_event.id = ARM_SIG;
    ogs_fsm_dispatch((ogs_fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 0, bomb.code);

    tick_event.id = UP_SIG;
    ogs_fsm_dispatch((ogs_fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 1, bomb.code);

    tick_event.id = UP_SIG;
    ogs_fsm_dispatch((ogs_fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 3, bomb.code);

    tick_event.id = UP_SIG;
    ogs_fsm_dispatch((ogs_fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 7, bomb.code);

    tick_event.id = DOWN_SIG;
    ogs_fsm_dispatch((ogs_fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, OGS_FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 14, bomb.code);

    tick_event.id = ARM_SIG;
    ogs_fsm_dispatch((ogs_fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, OGS_FSM_STATE(&bomb));
}

enum alarm_signal_e {
    TICK_SIG = OGS_FSM_USER_SIG,
    ALARM_SET_SIG,
    ALARM_ON_SIG,
    ALARM_OFF_SIG,
    ALARM_SIG,
    CLOCK_12H_SIG,
    CLOCK_24H_SIG,
    TIME_SIG,
    TERMINATE_SIG
};

typedef struct alarm_s {
    ogs_fsm_t       fsm;
    uint32_t  time;
} alarm_t;

typedef struct set_event_s {
    int         id;
    uint8_t   digit;
} set_event_t;

typedef struct time_event_s {
    int         id;
    uint8_t   current_time;
} time_event_t;

void alarm_initial(alarm_t *s, set_event_t *e);
void alarm_off(alarm_t *s, set_event_t *e);
void alarm_on(alarm_t *s, time_event_t *e);

void alarm_initial(alarm_t *s, set_event_t *e)
{
    s->time = 12*60;
    OGS_FSM_TRAN(s, &alarm_off);
}

void alarm_off(alarm_t *s, set_event_t *e)
{
    uint32_t alarm;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        s->time = (s->time/60)*100 + s->time%60;
        break;
    case OGS_FSM_EXIT_SIG:
        s->time = (s->time/100)*60 + s->time%100;
        break;
    case ALARM_ON_SIG:
        OGS_FSM_TRAN(s, &alarm_on);
        break;
    case ALARM_SET_SIG:
        alarm = (10 * s->time
                          + e->digit) % 10000;
        if ((alarm / 100 < 24) && (alarm % 100 < 60)) {
            s->time = alarm;
        }
        else
        {
            s->time = 0;
        }
        break;
    }
}

void alarm_on(alarm_t *s, time_event_t *e)
{
    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case ALARM_SET_SIG:
        break;
    case ALARM_OFF_SIG:
        OGS_FSM_TRAN(s, &alarm_off);
        break;
    }
}

static void test2_func(abts_case *tc, void *data)
{
    alarm_t alarm;
    set_event_t set_event;
    time_event_t time_event;

    ogs_fsm_init(&alarm, &alarm_initial, 0, 0);
    ABTS_PTR_EQUAL(tc, &alarm_off, OGS_FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 1200, alarm.time);

    set_event.id = ALARM_ON_SIG;
    ogs_fsm_dispatch(&alarm, &set_event);
    ABTS_PTR_EQUAL(tc, &alarm_on, OGS_FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 720, alarm.time);

    time_event.id = ALARM_OFF_SIG;
    ogs_fsm_dispatch(&alarm, &time_event);
    ABTS_PTR_EQUAL(tc, &alarm_off, OGS_FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 1200, alarm.time);

    set_event.id = ALARM_SET_SIG;
    set_event.digit = 0;
    ogs_fsm_dispatch(&alarm, &set_event);
    ABTS_PTR_EQUAL(tc, &alarm_off, OGS_FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 2000, alarm.time);
}

abts_suite *test_fsm(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);

    return suite;
}
