#include "WtApplication.h"

/// @brief Each user task corresponds to an async function.If the task does not have an async function, use NULL instead.
static code const void(code *_async_table[])(void) =
    {
        ///
        /// todo... Register async functions here.
        ///
        test_async, // Table row index #0 = Task id #1, Maximum count 15, Table row index = Task id - 1.
                    // Table row index #1 = Task id #2
                    // Table row index #2 = Task id #3
                    // Table row index #3 = Task id #4
                    // Table row index #4 = Task id #5
                    // Table row index #5 = Task id #6
                    // Table row index #6 = Task id #7
                    // Table row index #7 = Task id #8
                    // Table row index #8 = Task id #9
                    // Table row index #9 = Task id #10
                    // Table row index #10 = Task id #11
                    // Table row index #11 = Task id #12
                    // Table row index #12 = Task id #13
                    // Table row index #13 = Task id #14
                    // Table row index #14 = Task id #15
};

/// @brief The transition model of the state machine.
typedef struct
{
    /// @brief The current state.
    char current;
    /// @brief Event with the current state.
    char event;
    /// @brief Next state with the event.
    char next;
    /// @brief Action with the event.
    void(code *action)(void);
} _transition_type;

static code const _transition_type _transition_table0[] =
    {
        ///
        /// todo... Define transitions here. Maximum count 64.
        ///
        {WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, test_action},
};

static code const _transition_type _transition_table1[] =
    {
        ///
        /// todo... Define transitions here. Maximum count 64.
        ///
        {WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, NULL},
};

static code const _transition_type _transition_table2[] =
    {
        ///
        /// todo... Define transitions here. Maximum count 64.
        ///
        {WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, NULL},
};

static code const _transition_type _transition_table3[] =
    {
        ///
        /// todo... Define transitions here. Maximum count 64.
        ///
        {WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, NULL},
};

static struct
{
    unsigned char read_index : 4;
    unsigned char write_index : 4;
    unsigned char read_mirror : 1;
    unsigned char write_mirror : 1;
    unsigned char transition_count : 6;
    unsigned int async_flag;
    unsigned char buffer[WT_APPLICATION_COUNT(_async_table)];
    char current_state;
    _transition_type *transition_table;
} _app = {0, 0, 0, 0, 0, 0, {0}, WT_APPLICATION_STATE_UNKNOWN, NULL};

static void __main(void) _task_ 0
{
    ///
    /// todo... Register initialize functions here.
    ///
    test_initialize();
    printf(WT_APPLICATION_DESCRIPTION);
    while (1)
    {
        void(code * act)(void);
        unsigned char t;
        // 1.Wait.
        while (_app.read_index == _app.write_index && _app.read_mirror == _app.write_mirror)
        {
            WT_APPLICATION_SET_BIT(_app.async_flag, 0, 1);
            os_wait1(K_SIG);
            WT_APPLICATION_SET_BIT(_app.async_flag, 0, 0);
        }
        // 2.Dequeue.
        t = _app.buffer[_app.read_index];
        if (_app.read_index == WT_APPLICATION_COUNT(_async_table) - 1)
        {
            _app.read_mirror = ~_app.read_mirror;
            _app.read_index = 0;
        }
        else
        {
            _app.read_index++;
        }
        // 3. Invoke.
        act = _async_table[t - 1];
        if (act)
        {
            act();
        }
        WT_APPLICATION_SET_BIT(_app.async_flag, t, 0);
        os_send_signal(t);
    }
}

void wt_application_await(void)
{
    // 1.Enqueue.
    _app.buffer[_app.write_index] = os_running_task_id();
    if (_app.write_index == WT_APPLICATION_COUNT(_async_table) - 1)
    {
        _app.write_mirror = ~_app.write_mirror;
        _app.write_index = 0;
    }
    else
    {
        _app.write_index++;
    }
    // 2.Notify.
    if (WT_APPLICATION_GET_BIT(_app.async_flag, 0))
    {
        os_send_signal(0);
    }
    // 3.Wait.
    WT_APPLICATION_SET_BIT(_app.async_flag, os_running_task_id(), 1);
    do
    {
        os_wait1(K_SIG);
    } while (WT_APPLICATION_GET_BIT(_app.async_flag, os_running_task_id()));
}

void wt_application_start(unsigned char transition_index, char initial_state)
{
    if (transition_index == 3)
    {
        _app.transition_count = WT_APPLICATION_COUNT(_transition_table3);
        _app.transition_table = _transition_table3;
    }
    else if (transition_index == 2)
    {
        _app.transition_count = WT_APPLICATION_COUNT(_transition_table2);
        _app.transition_table = _transition_table2;
    }
    else if (transition_index == 1)
    {
        _app.transition_count = WT_APPLICATION_COUNT(_transition_table1);
        _app.transition_table = _transition_table1;
    }
    else
    {
        _app.transition_count = WT_APPLICATION_COUNT(_transition_table0);
        _app.transition_table = _transition_table0;
    }
    _app.current_state = initial_state;
}

unsigned char wt_application_raise(char event)
{
    unsigned char i, ret = 0;
    _transition_type *tran;
    if (_app.transition_table)
    {
        for (i = 0; i < _app.transition_count; i++)
        {
            tran = &_app.transition_table[i];
            if (tran->current == _app.current_state)
            {
                if (tran->event == event)
                {                    
                    _app.current_state = tran->next;
                    if (tran->action)
                    {
                        tran->action();
                    }
                    ret = 1;
                    break;
                }
            }
        }
    }
    return ret;
}

char wt_application_get_state(void)
{
    return _app.current_state;
}
