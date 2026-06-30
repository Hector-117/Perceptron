
/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Custom libraries:
#include "BSP.hpp"




/* ******************************************************************** */
/* ******************* GLOBAL VARIABLES AND OBJECTS ******************* */

extern Adafruit_NeoPixel led_builtin(NUMPIXELS, LED_BUILTIN_PIN, NEO_GRB + NEO_KHZ800);
uint8_t led_builtin_state[3] = {0};




/* ******************************************************************** */
/* ********************** FUNCTIONS' DEFINITIONS ********************** */

// **** Functions for GPIO function validation:

// GPIOs configured as inputs:

bool is_valid_digital_input(uint8_t pin){
    if(!((pin >= 1 && pin <= 18)    ||
	    (pin == 21)                 ||
	    (pin >= 33 	&& pin <= 42)   ||
	    (pin >= 45 	&& pin <= 48))){
        
        return false;
    }
    return true;
}

bool is_valid_adc_input(uint8_t pin){
    if(!(pin >= 1 && pin <= 16)){
        return false;
    }
    return true;
}

// GPIOs configured as outputs:

bool is_valid_digital_output(uint8_t pin){
    if(!((pin >= 1 && pin <= 18)    ||
	    (pin == 21)                 ||
	    (pin >= 33 	&& pin <= 42)   ||
	    (pin >= 45 	&& pin <= 48))){
        
        return false;
    }
    return true;
}

bool is_valid_pwm_output(uint8_t pin){
    if(!((pin >= 1 && pin <= 16)    ||
	    (pin >= 33 	&& pin <= 42)   ||
	    (pin >= 45 	&& pin <= 48))){
        
        return false;
    }
    return true;
}




// **** Functions for builtin hardware components:

void bsp_init(void){
    led_builtin.begin();
    bsp_set_rgb_color(0, 0, 0);

    return;
}

void bsp_turn_on_builtin_led(void){
    bsp_set_rgb_color(MAX_LED_BRIGHTNESS, MAX_LED_BRIGHTNESS, MAX_LED_BRIGHTNESS);
    return;
}

void bsp_turn_off_builtin_led(void){
    bsp_set_rgb_color(0, 0, 0);
    return;
}

void bsp_toggle_builtin_led(void){
    if(led_builtin_state[0] > 0 || led_builtin_state[1] > 0 || led_builtin_state[2] > 0)
        bsp_turn_on_builtin_led();
    else
        bsp_turn_off_builtin_led();

    return;
}

void bsp_set_rgb_color(uint8_t r, uint8_t g, uint8_t b){
  led_builtin.setPixelColor(0, led_builtin.Color(r, g, b));
  led_builtin_state[0] = r;
  led_builtin_state[1] = g;
  led_builtin_state[2] = b;
  led_builtin.show();
  return;
}