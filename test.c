#include "WtApplication.h"

void test_initialize(void)
{
    wt_application_start(0, WT_APPLICATION_STATE_UNKNOWN);
    os_create_task(1);
}

void test_async(void)
{
    wt_application_raise(WT_APPLICATION_EVENT_UNKNOWN);
}

void test_action(void)
{
    int b = 0;
    b++;
}

void test_job(void) _task_ 1
{
    while (1)
    {
        wt_application_await();
        wt_application_wait(K_TMO, 1, 255);
    }
}