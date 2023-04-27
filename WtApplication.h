/*
 * This file is an application launcher based on RTX-51 TINY Real-Time Operating
 * System, and it supports state machine architecture. The launcher occupies task #0,
 * so user tasks must be numbered from #1 to #15.
 *
 * Date			Author						Notes
 * 2023/03/04	feallee@hotmail.com			First implementation.
 * 2023/04/26   feallee@hotmail.com         Streamline business logic.
 */

#ifndef __WT_APPLICATION_H_
#define __WT_APPLICATION_H_

/// @brief Get the y-th bit value of x.
#define WT_APPLICATION_GET_BIT(x, y) (((x) >> (y)) & 1)

/// @brief Set the y-th bit value of x.
#define WT_APPLICATION_SET_BIT(x, y) ((x) |= (1UL << (y)))

/// @brief Clear the y-th bit value of x.
#define WT_APPLICATION_CLR_BIT(x, y) ((x) &= ~(1UL << (y)))

/// @brief Get the count of the array.
#define WT_APPLICATION_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

/// @brief Indicates an unknown state and is not allowed when defining a state by the user.
#define WT_APPLICATION_STATE_UNKNOWN (0xff)

/// @brief Indicates an unknown event and is not allowed when defining a event by the user.
#define WT_APPLICATION_EVENT_UNKNOWN (0xff)

/// @brief Represents a null pointer.
#define WT_APPLICATION_NULL ((void *)0)

/// @brief The transition model of state machine.
typedef struct
{
	/// @brief The current state
	char current;
	/// @brief Event with the current state.
	char event;
	/// @brief Next state with the event.
	char next;
	/// @brief Action with the event.
	void(code *action)(void);
} wt_application_transition_type;

/// @brief Await invoke async function.
void wt_application_await(void);

/// @brief Change state transition table.
/// @param transition_index Transition table index(0-3).
/// @param initial_state Initial state of state machine.
void wt_application_change(unsigned char transition_index, char initial_state);

/// @brief Raise an event to the state machine.
/// @return Returns the state transition flag(0 = Fail, not 0 = Success).
unsigned char wt_application_raise(char event);

#include <RTX51TNY.H>
///
/// todo... Include head files here.
///
#include "test.h"

/// @brief Context type of the application.
typedef struct
{
	/// @brief Count of async function table.
	unsigned char async_count;
	/// @brief The current state of the state machine.
	char current_state;
	/// @brief Count of state transition table.
	unsigned char transition_count;
	/// @brief The current transition table of state machine.
	wt_application_transition_type *transition_table;
	///
	/// todo... Define other context fields here.
	///
} wt_application_context_type;

/// @brief Get context of the application.
extern wt_application_context_type wt_application_context;

static code void(code *wt_application_initialize_table[])(void) =
	{
		///
		/// todo... Register initialize functions here.
		///
		test_initialize,

};

static code void(code *wt_application_async_table[])(void) =
	{
		/*
		 * Note:
		 * 1.Each user task corresponds to an async function.If the task does not have an async function, use WT_APPLICATION_NULL instead.
		 * 2.Maximum count:15.
		 * 3.Table index = Task id - 1.
		 */
		///
		/// todo... Register async functions here.
		///
		test_async, // table index #0 = task #1
					// table index #1 = task #2
					// table index #2 = task #3
					// table index #3 = task #4
					// table index #4 = task #5
					// table index #5 = task #6
					// table index #6 = task #7
					// table index #7 = task #8
					// table index #8 = task #9
					// table index #9 = task #10
					// table index #10 = task #11
					// table index #11 = task #12
					// table index #12 = task #13
					// table index #13 = task #14
					// table index #14 = task #15
};

static code wt_application_transition_type wt_application_transition_table0[] =
	{
		///
		/// todo... Define transitions here.
		///
		{WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, test_action},
};

static code wt_application_transition_type wt_application_transition_table1[] =
	{
		///
		/// todo... Define transitions here.
		///
		{WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_NULL},
};

static code wt_application_transition_type wt_application_transition_table2[] =
	{
		///
		/// todo... Define transitions here.
		///
		{WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_NULL},
};

static code wt_application_transition_type wt_application_transition_table3[] =
	{
		///
		/// todo... Define transitions here.
		///
		{WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_EVENT_UNKNOWN, WT_APPLICATION_STATE_UNKNOWN, WT_APPLICATION_NULL},
};
#endif