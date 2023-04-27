#include "WtApplication.h"

void test_initialize(void)
{
    os_create_task(1);
    wt_application_change(2, 'a');
}

void test_async(void)
{
    wt_application_raise('a');
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
    }
}