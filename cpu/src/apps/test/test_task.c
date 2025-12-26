/*----------------------------------------------------------------------

                     This file is part of Freetribe

                https://github.com/bangcorrupt/freetribe

                                License

                   GNU AFFERO GENERAL PUBLIC LICENSE
                      Version 3, 19 November 2007

                           AGPL-3.0-or-later

 Freetribe is free software: you can redistribute it and/or modify it
under the terms of the GNU Affero General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
                  (at your option) any later version.

     Freetribe is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty
        of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
          See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.

                       Copyright bangcorrupt 2023

----------------------------------------------------------------------*/

/**
 * @file    test_task.c
 *
 * @brief   Run tests in sequence.
 *
 */

/*----- Includes -----------------------------------------------------*/

#include "freetribe.h"

#include <stdlib.h>

#include "ft_error.h"
#include "gui_task.h"

#include "test_button.h"
#include "test_display.h"

/*----- Macros -------------------------------------------------------*/

#define BUTTON_PLAY 0x02
#define BUTTON_EXIT 0x0d

/*----- Typedefs -----------------------------------------------------*/

typedef enum {
    STATE_INIT,
    STATE_INFO,
    STATE_MALLOC,
    STATE_PRINT,
    STATE_DISPLAY,
    STATE_BUTTON,
    STATE_KNOB,
    STATE_SHUTDOWN,
    STATE_WAIT,

    STATE_ERROR
} e_template_task_state;

/*----- Static variable definitions ----------------------------------*/

static bool g_test_confirmed;

/*----- Extern variable definitions ----------------------------------*/

extern uint8_t *_heap_start;
extern uint8_t *_heap_end;

/*----- Static function prototypes -----------------------------------*/

static t_status _init(void);
static t_status _print_info(void);

static void _button_callback(uint8_t button, bool state);

static t_status _test_print(void);
static t_status _test_malloc(void);
static t_status _test_shutdown(void);

/*----- Extern function implementations ------------------------------*/

void test_task(void) {

    static e_template_task_state state = STATE_INIT;

    switch (state) {

    // Initialise template task.
    case STATE_INIT:
        if (error_check(_init()) == SUCCESS) {
            state = STATE_INFO;
        }
        // Remain in INIT state until initialisation successful.
        break;

    case STATE_INFO:
        if (_print_info() == SUCCESS) {
            state = STATE_MALLOC;
        }
        break;

    case STATE_MALLOC:
        if (_test_malloc() == SUCCESS) {
            state = STATE_PRINT;
        }
        break;

    case STATE_PRINT:
        if (_test_print() == SUCCESS) {
            state = STATE_BUTTON;
        }
        break;

    case STATE_BUTTON:
        if (test_button() == SUCCESS) {
            state = STATE_DISPLAY;
        }
        break;

    case STATE_DISPLAY:
        if (test_display() == SUCCESS) {
            state = STATE_KNOB;
        }
        break;

    case STATE_KNOB:
        state = STATE_SHUTDOWN;
        break;

    case STATE_SHUTDOWN:
        _test_shutdown();
        state = STATE_WAIT;
        break;

    case STATE_WAIT:
        break;

    case STATE_ERROR:
        error_check(UNRECOVERABLE_ERROR);
        break;

    default:
        if (error_check(UNHANDLED_STATE_ERROR) != SUCCESS) {
            state = STATE_ERROR;
        }
        break;
    }
}

bool test_confirmed(void) {
    //
    return g_test_confirmed;
}

void confirm_test(void) {
    //
    g_test_confirmed = true;
}

void reset_test(void) {
    //
    g_test_confirmed = false;
}

/*----- Static function implementations ------------------------------*/

static t_status _init(void) {

    t_status result = TASK_INIT_ERROR;

    ft_register_panel_callback(BUTTON_EVENT, _button_callback);

    ft_printf("Freetribe Test\n");

    result = SUCCESS;
    return result;
}

static t_status _print_info(void) {

    t_status result = ERROR;

    // Print some stuff...

    result = SUCCESS;
    return result;
}

static void _button_callback(uint8_t button, bool state) {

    switch (button) {

    case BUTTON_PLAY:
        if (state) {
            confirm_test();
        }
        break;

    case BUTTON_EXIT:
        if (state) {
            ft_shutdown();
        }

    default:
        break;
    }
}

static t_status _test_print(void) {

    ft_printf("Test print...");
    ft_printf("Print test PASSED.\n");

    // Assume success to move on to next test.
    return SUCCESS;
}

static t_status _test_malloc(void) {

    t_status result = ERROR;

    uint32_t *address = NULL;
    uint32_t length = 0x100;

    ft_printf("Test memory allocation:");

    if ((address = malloc(length))) {

        ft_printf("- Heap start at %#10x", &_heap_start);
        ft_printf("- Heap end at %#10x", &_heap_end);
        ft_printf("- Heap size = %i bytes", &_heap_end - &_heap_start);

        ft_printf("- Allocated %i bytes at %#10x", length, address);
        ft_printf("Memory allocation test PASSED.\n");
        result = SUCCESS;

    } else {
        ft_printf("Memory allocation test FAILED.\n");
    }

    return result;
}

static t_status _test_shutdown(void) {

    ft_printf("Test shutdown:");
    ft_printf("> Press [Exit] to power off...");

    return ERROR;
}

/*----- End of file --------------------------------------------------*/
