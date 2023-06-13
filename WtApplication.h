/*
 * This file is an application launcher based on RTX-51 TINY Real-Time Operating
 * System, and it supports state machine architecture. The launcher occupies task #0,
 * so user tasks must be numbered from #1 to #15.
 *
 * Date			Author						Notes
 * 2023/03/04	feallee@hotmail.com			First implementation.
 * 2023/04/26   feallee@hotmail.com         Streamline business logic.
 * 2023/05/27   feallee@hotmail.com         Streamline business logic.
 * 2023/06/13   feallee@hotmail.com         Streamline business logic.
 */

#ifndef __WT_APPLICATION_H_
#define __WT_APPLICATION_H_

#include <STDDEF.H>
#include <RTX51TNY.H>
///
/// todo... Include head files here.
///
#include <STDIO.H>
#include "test.h"

///
/// todo... Define the application description.
///
#define WT_APPLICATION_DESCRIPTION "**************************************************************************\r\n" \
								   " ___      ________ This is an application launcher based on RTX-51 TINY   \r\n" \
								   " __ | /| / /_  __/ real-time operating system, and it supports state      \r\n" \
								   " __ |/ |/ / / /    machine architecture.                                  \r\n" \
								   " ____/|__/ /_/                                                            \r\n" \
								   "                   Home: https://weas.top.                                \r\n" \
								   "                   Issues: https://github.com/feallee.                    \r\n" \
								   "                                                                          \r\n" \
								   "                   Powered by feallee@hotmail.com.                        \r\n" \
								   "--------------------------------------------------------------------------\r\n"

/// @brief Indicates an unknown state and is not allowed when defining a state by the user.
#define WT_APPLICATION_STATE_UNKNOWN EOF // todo... User defined.

/// @brief Indicates an unknown event and is not allowed when defining a event by the user.
#define WT_APPLICATION_EVENT_UNKNOWN EOF // todo... User defined.

//------------------------------------------------------------------------------
//----------------- !!! End of User Configuration Part    !!! ------------------
//----------------- !!! Do not modify code sections below !!! ------------------
//------------------------------------------------------------------------------

/// @brief Get the value of the n-th bit value of x.
#define WT_APPLICATION_GET_BIT(X, N) (((X) >> (N)) & 1)

/// @brief Set the value of the n-th bit of x to v.
#define WT_APPLICATION_SET_BIT(X, N, V) ((X) = (V) ? (X) | (1UL << (N)) : (X) & (~(1UL << (N))))

/// @brief Get the count of the array.
#define WT_APPLICATION_COUNT(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

/// @brief Await invoke async function.
void wt_application_await(void);

/// @brief Change transition table index of the state machine.
/// @param transition_index Transition table index(0-3).
/// @param initial_state Initial state of the state machine.
void wt_application_change(unsigned char transition_index, char initial_state);

/// @brief Raise an event to the state machine.
/// @return Returns the state transition flag(0 = Fail, not 0 = Success).
unsigned char wt_application_raise(char event);

/// @brief Get current state of the state machine.
char wt_application_get_state(void);
#endif