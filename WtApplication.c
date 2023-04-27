#include "WtApplication.h"

static struct
{
    unsigned char read_index : 7;
    unsigned char read_mirror : 1;
    unsigned char write_index : 7;
    unsigned char write_mirror : 1;
    unsigned int async_flag;
    unsigned char buffer[WT_APPLICATION_COUNT(wt_application_async_table)];
} _queue = {0, 0, 0, 0, 0};

wt_application_context_type wt_application_context = {
    WT_APPLICATION_COUNT(wt_application_async_table), // async_count
    WT_APPLICATION_STATE_UNKNOWN,                     // current_state
    0,                                                // transition_count
    WT_APPLICATION_NULL                               // transition_table
};

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
        while (_queue.read_index == _queue.write_index && _queue.read_mirror == _queue.write_mirror)
        {
            WT_APPLICATION_SET_BIT(_queue.async_flag, 0);
            os_wait1(K_SIG);
            WT_APPLICATION_CLR_BIT(_queue.async_flag, 0);
        }
        // 2.Pop.
        t = _queue.buffer[_queue.read_index];
        if (_queue.read_index == wt_application_context.async_count - 1)
        {
            _queue.read_mirror = ~_queue.read_mirror;
            _queue.read_index = 0;
        }
        else
        {
            _queue.read_index++;
        }
        // 3. Invoke.
        if (wt_application_async_table[t - 1])
        {
            wt_application_async_table[t - 1]();
        }
        // 4.Notify.
        WT_APPLICATION_CLR_BIT(_queue.async_flag, t);
        os_send_signal(t);
    }
}

void wt_application_await(void)
{
    // 1.Push.
    _queue.buffer[_queue.write_index] = os_running_task_id();
    if (_queue.write_index == wt_application_context.async_count - 1)
    {
        _queue.write_mirror = ~_queue.write_mirror;
        _queue.write_index = 0;
    }
    else
    {
        _queue.write_index++;
    }
    // 2.Notify.
    if (WT_APPLICATION_GET_BIT(_queue.async_flag, 0))
    {
        os_send_signal(0);
    }
    // 3.Wait.
    WT_APPLICATION_SET_BIT(_queue.async_flag, os_running_task_id());
    do
    {
        os_wait1(K_SIG);
    } while (WT_APPLICATION_GET_BIT(_queue.async_flag, os_running_task_id()));
}

void wt_application_change(unsigned char transition_index, char initial_state)
{
    if (transition_index == 3)
    {
        wt_application_context.current_state = initial_state;
        wt_application_context.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table3);
        wt_application_context.transition_table = wt_application_transition_table3;
    }
    else if (transition_index == 2)
    {
        wt_application_context.current_state = initial_state;
        wt_application_context.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table2);
        wt_application_context.transition_table = wt_application_transition_table2;
    }
    else if (transition_index == 1)
    {
        wt_application_context.current_state = initial_state;
        wt_application_context.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table1);
        wt_application_context.transition_table = wt_application_transition_table1;
    }
    else
    {
        wt_application_context.current_state = initial_state;
        wt_application_context.transition_count = WT_APPLICATION_COUNT(wt_application_transition_table0);
        wt_application_context.transition_table = wt_application_transition_table0;
    }
}

unsigned char wt_application_raise(char event)
{
    unsigned char i, ret = 0;
    if (wt_application_context.transition_table)
    {
        for (i = 0; i < wt_application_context.transition_count; i++)
        {
            if (wt_application_context.transition_table[i].current == wt_application_context.current_state)
            {
                if (wt_application_context.transition_table[i].event == event)
                {
                    wt_application_context.current_state = wt_application_context.transition_table[i].next;
                    if (wt_application_context.transition_table[i].action)
                    {
                        wt_application_context.transition_table[i].action();
                    }
                    ret = 1;
                    break;
                }
            }
        }
    }
    return ret;
}
