#include "core_fsm.h"
#include "testutil.h"

enum bomb_signal_t {
    UP_SIG = FSM_USER_SIG,
    DOWN_SIG,
    ARM_SIG
};

typedef struct _tick_event_t {
    fsm_event_t event;
} tick_event_t;

typedef struct _bomb_t {
    fsm_t fsm;
    c_uint8_t timeout;
    c_uint8_t code;
    c_uint8_t defuse;
} bomb_t;

void bomb_initial(bomb_t *s, tick_event_t *e);
void bomb_setting(bomb_t *s, tick_event_t *e);
void bomb_timing(bomb_t *s, tick_event_t *e);

void bomb_create(bomb_t *s, uint8_t defuse)
{
    fsm_create(&s->fsm, &bomb_initial, 0);
    s->defuse = defuse;
}

void bomb_initial(bomb_t *s, tick_event_t *e)
{
    s->timeout = 10;
    FSM_TRAN(s, &bomb_setting);
}

void bomb_setting(bomb_t *s, tick_event_t *e)
{
    switch (e->event)
    {
        case UP_SIG:
        {
            if (s->timeout < 12)
            {
                ++s->timeout;
            }
            break;
        }
        case DOWN_SIG: {
            if (s->timeout > 8)
            {
                --s->timeout;
            }
            break;
        }
        case ARM_SIG:
        {
            FSM_TRAN(s, &bomb_timing);
            break;
        }
    }
}

void bomb_timing(bomb_t *s, tick_event_t *e)
{
    switch (e->event)
    {
        case FSM_ENTRY_SIG:
        {
            s->code = 0;
            break;
        }
        case UP_SIG:
        {
            s->code <<= 1;
            s->code |= 1;
            break;
        }
        case DOWN_SIG:
        {
            s->code <<= 1;
            break;
        }
        case ARM_SIG:
        {
            if (s->code == s->defuse)
            {
                FSM_TRAN(s, &bomb_setting);
                break;
            }
        }
    }
}

static void fsm_test1(abts_case *tc, void *data)
{
    bomb_t bomb;
    tick_event_t tick_event;

    bomb_create(&bomb, 14);

    fsm_init(&bomb, 0);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 10, bomb.timeout);

    tick_event.event = UP_SIG;
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 11, bomb.timeout);
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 12, bomb.timeout);
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 12, bomb.timeout);

    tick_event.event = DOWN_SIG;
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 11, bomb.timeout);
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 10, bomb.timeout);
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 9, bomb.timeout);
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 8, bomb.timeout);
    fsm_dispatch(&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 8, bomb.timeout);

    tick_event.event = ARM_SIG;
    fsm_dispatch((fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 0, bomb.code);

    tick_event.event = UP_SIG;
    fsm_dispatch((fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 1, bomb.code);

    tick_event.event = UP_SIG;
    fsm_dispatch((fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 3, bomb.code);

    tick_event.event = UP_SIG;
    fsm_dispatch((fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 7, bomb.code);

    tick_event.event = DOWN_SIG;
    fsm_dispatch((fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_timing, FSM_STATE(&bomb));
    ABTS_INT_EQUAL(tc, 14, bomb.code);

    tick_event.event = ARM_SIG;
    fsm_dispatch((fsm_t *)&bomb, &tick_event);
    ABTS_PTR_EQUAL(tc, &bomb_setting, FSM_STATE(&bomb));
}

enum alarm_signal_t {
    TICK_SIG = FSM_USER_SIG,
    ALARM_SET_SIG,
    ALARM_ON_SIG,
    ALARM_OFF_SIG,
    ALARM_SIG,
    CLOCK_12H_SIG,
    CLOCK_24H_SIG,
    TIME_SIG,
    TERMINATE_SIG
};

typedef struct _alarm_t {
    fsm_t        fsm;
    c_uint32_t   time;
} alarm_t;

typedef struct _set_event_t {
    fsm_event_t  event;
    c_uint8_t    digit;
} set_event_t;

typedef struct _time_event_t {
    fsm_event_t  event;
    c_uint8_t    current_time;
} time_event_t;

void alarm_initial(alarm_t *s, set_event_t *e);
void alarm_off(alarm_t *s, set_event_t *e);
void alarm_on(alarm_t *s, time_event_t *e);

void alarm_initial(alarm_t *s, set_event_t *e)
{
    s->time = 12*60;
    FSM_TRAN(s, &alarm_off);
}

void alarm_off(alarm_t *s, set_event_t *e)
{
    switch (e->event)
    {
        case FSM_ENTRY_SIG:
        {
            s->time = (s->time/60)*100 + s->time%60;
            break;
        }
        case FSM_EXIT_SIG:
        {
            s->time = (s->time/100)*60 + s->time%100;
            break;
        }
        case ALARM_ON_SIG:
        {
            FSM_TRAN(s, &alarm_on);
            break;
        }
        case ALARM_SET_SIG:
        {
            c_uint32_t alarm = (10 * s->time
                              + e->digit) % 10000;
            if ((alarm / 100 < 24) && (alarm % 100 < 60))
            {
                s->time = alarm;
            }
            else
            {
                s->time = 0;
            }
            break;
        }
    }
}

void alarm_on(alarm_t *s, time_event_t *e)
{
    switch (e->event)
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case ALARM_SET_SIG:
        {
            break;
        }
        case ALARM_OFF_SIG:
        {
            FSM_TRAN(s, &alarm_off);
            break;
        }
    }
}

static void fsm_test2(abts_case *tc, void *data)
{
    alarm_t alarm;
    set_event_t set_event;
    time_event_t time_event;

    fsm_create(&alarm.fsm, &alarm_initial, 0);

    fsm_init(&alarm, 0);
    ABTS_PTR_EQUAL(tc, &alarm_off, FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 1200, alarm.time);

    set_event.event = ALARM_ON_SIG;
    fsm_dispatch(&alarm, &set_event);
    ABTS_PTR_EQUAL(tc, &alarm_on, FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 720, alarm.time);

    time_event.event = ALARM_OFF_SIG;
    fsm_dispatch(&alarm, &time_event);
    ABTS_PTR_EQUAL(tc, &alarm_off, FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 1200, alarm.time);

    set_event.event = ALARM_SET_SIG;
    set_event.digit = 0;
    fsm_dispatch(&alarm, &set_event);
    ABTS_PTR_EQUAL(tc, &alarm_off, FSM_STATE(&alarm));
    ABTS_INT_EQUAL(tc, 2000, alarm.time);
}

abts_suite *testfsm(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, fsm_test1, NULL);
    abts_run_test(suite, fsm_test2, NULL);

    return suite;
}
