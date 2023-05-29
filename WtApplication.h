/*
 * This file is an application launcher based on RTX-51 TINY Real-Time Operating
 * System, and it supports state machine architecture. The launcher occupies task #0,
 * so user tasks must be numbered from #1 to #15.
 *
 * Date			Author						Notes
 * 2023/03/04	feallee@hotmail.com			First implementation.
 * 2023/04/26   feallee@hotmail.com         Streamline business logic.
 * 2023/05/27   feallee@hotmail.com         Streamline business logic.
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
#define WT_DEVICE_DESCRIPTION "**************************************************************************\r\n" \
							  "  _      ________  This is an application launcher based on RTX-51 TINY   \r\n" \ 
							  " | | /| / /_  __/  real-time operating system, and it supports state      \r\n" \
							  " | |/ |/ / / /     machine architecture.                                  \r\n" \
							  " |__/|__/ /_/                                                             \r\n" \
							  "                   Home: https://weas.top.                                \r\n" \
							  "                   Issues: https://github.com/feallee.                    \r\n" \
							  "                                                                          \r\n" \
							  "                   Powered by feallee@hotmail.com.                        \r\n" \
							  "--------------------------------------------------------------------------\r\n"

/// @brief Indicates an unknown state and is not allowed when defining a state by the user.
#define WT_APPLICATION_STATE_UNKNOWN (-1)

/// @brief Indicates an unknown event and is not allowed when defining a event by the user.
#define WT_APPLICATION_EVENT_UNKNOWN (-1)

/// @brief Get the y-th bit value of x.
#define WT_APPLICATION_GET_BIT(x, y) (((x) >> (y)) & 1)

/// @brief Set the y-th bit value of x.
#define WT_APPLICATION_SET_BIT(x, y) ((x) |= (1UL << (y)))

/// @brief Clear the y-th bit value of x.
#define WT_APPLICATION_CLR_BIT(x, y) ((x) &= ~(1UL << (y)))

/// @brief Get the count of the array.
#define WT_APPLICATION_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

/// @brief Show the application description.
void wt_application_show(void);

/// @brief Await invoke async function.
void wt_application_await(void);

/// @brief Start the state machine.
/// @param transition_index Transition table index(0-3).
/// @param initial_state Initial state of the state machine.
void wt_application_start(unsigned char transition_index, char initial_state);

/// @brief Raise an event to the state machine.
/// @return Returns the state transition flag(0 = Fail, not 0 = Success).
unsigned char wt_application_raise(char event);

/// @brief Get current state of the state machine.
char wt_application_get_state(void);

/// @brief Waiting for an RTX-51 event.
/// @param type RTX-51 event type:K_SIG, K_TMO, K_IVL, K_SIG | K_TMO, K_IVL | K_TMO.
/// @param count Repeat count.
/// @param ticks timeout in hardware-timer ticks.
/// @return return RTX-51 event:NOT_OK, TMO_EVENT, SIG_EVENT, RDY_EVENT.
extern unsigned char wt_application_wait(unsigned char type, unsigned int count, unsigned char ticks);
#endif