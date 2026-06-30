
/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Custom libraries:
#include "HAL.h"
#include "BSP.h"




/* ******************************************************************** */
/* ******************* GLOBAL VARIABLES AND OBJECTS ******************* */

led_t led_builtin = {0};




/* ******************************************************************** */
/* ********************** FUNCTIONS' DEFINITIONS ********************** */

void bsp_init(void){
    hal_led_init(&led_builtin, LED_BUILTIN_PIN, false);
    return;
}

void bsp_turn_on_builtin_led(void){
    hal_led_turn_on(&led_builtin);
    return;
}

void bsp_turn_off_builtin_led(void){
    hal_led_turn_off(&led_builtin);
    return;
}

void bsp_toggle_builtin_led(void){
    hal_led_toggle(&led_builtin);
    return;
}

bool bsp_get_builtin_led_state(void){
    return hal_led_get_state(&led_builtin);
}

void bsp_set_builtin_led_state(uint8_t state){
    hal_led_set_state(&led_builtin, state);
    return;
}