#include "WtApplication.h"

void test_initialize(void)
{
    wt_application_start(2, 'a');
    os_create_task(1);
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