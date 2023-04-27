#include "WtApplication.h"

static struct
{
    unsigned char read_index : 7;
    unsigned char read_mirror : 1;
    unsigned char write_index : 7;
    unsigned char write_mirror : 1;
    unsigned int async_flag;
    unsigned int transition_count;
    char current_state;
    wt_application_transition_type *transition_table;
    unsigned char buffer[WT_APPLICATION_COUNT(wt_application_async_table)];
} _app = {0, 0, 0, 0, 0, 0, WT_APPLICATION_STATE_UNKNOWN, NULL};

void wt_application_job(void) _task_ 0
{
    unsigned char t;
    for (t = 0; t < WT_APPLICATION_COUNT(wt_application_initialize_table); t++)
    {
        if (wt_application_initialize_table[t])
        {
            wt_application_initialize_table[t]();
        }
    }
    while (1)
    {
        // 1.Wait.
        while (_app.read_index == _app.write_index && _app.read_mirror == _app.write_mirror)
        {
            WT_APPLICATION_SET_BIT(_app.async_flag, 0);
            os_wait1(K_SIG);
            WT_APPLICATION_CLR_BIT(_app.async_flag, 0);
        }
        // 2.Pop.
        t = _app.buffer[_app.read_index];
        if (_app.read_index == WT_APPLICATION_COUNT(wt_application_async_table) - 1)
        {
            _app.read_mirror = ~_app.read_mirror;
            _app.read_index = 0;
        }
        else
        {
            _app.read_index++;
        }
        // 3. Invoke.
        if (wt_application_async_table[t - 1])
        {
            wt_application_async_table[t - 1]();
        }
        // 4.Notify.
        WT_APPLICATION_CLR_BIT(_app.async_flag, t);
        os_send_signal(t);
    }
}

void wt_application_await(void)
{
    // 1.Push.
    _app.buffer[_app.write_index] = os_running_task_id();
    if (_app.write_index == WT_APPLICATION_COUNT(wt_application_async_table) - 1)
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
    WT_APPLICATION_SET_BIT(_app.async_flag, os_running_task_id());
    do
    {
        os_wait1(K_SIG);
    } while (WT_APPLICATION_GET_BIT(_app.async_flag, os_running_task_id()));
}

void wt_application_start(unsigned char transition_index, char initial_state)
{
    if (transition_index == 3)
    {
        _app.current_state = initial_state;
        _app.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table3);
        _app.transition_table = wt_application_transition_table3;
    }
    else if (transition_index == 2)
    {
        _app.current_state = initial_state;
        _app.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table2);
        _app.transition_table = wt_application_transition_table2;
    }
    else if (transition_index == 1)
    {
        _app.current_state = initial_state;
        _app.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table1);
        _app.transition_table = wt_application_transition_table1;
    }
    else
    {
        _app.current_state = initial_state;
        _app.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table0);
        _app.transition_table = wt_application_transition_table0;
    }
}

unsigned char wt_application_raise(char event)
{
    unsigned char i, ret = 0;
    if (_app.transition_table)
    {
        for (i = 0; i < _app.transition_count; i++)
        {
            if (_app.transition_table[i].current == _app.current_state)
            {
                if (_app.transition_table[i].event == event)
                {
                    _app.current_state = _app.transition_table[i].next;
                    if (_app.transition_table[i].action)
                    {
                        _app.transition_table[i].action();
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
