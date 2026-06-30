
/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// ESP-IDF libraries:
#include "esp_timer.h"

// Custom libraries:
#include "HAL.h"




/* ******************************************************************** */
/* ********************** FUNCTIONS' DEFINITIONS ********************** */

// **** Functions for GPIO function validation:

// GPIOs configured as inputs:

bool is_valid_digital_input(uint8_t pin){
    if(!((pin >= 0 && pin <= 19)  ||
	    (pin >= 21 	&& pin <= 23) ||
	    (pin >= 25 	&& pin <= 27) ||
	    (pin >= 32 	&& pin <= 33) ||
	    (pin >= 32 	&& pin <= 39))){
        
        return false;
    }
    return true;
}

bool is_valid_adc_input(uint8_t pin){
    if(!((pin == 2 || pin == 4)     ||
        (pin >= 12  && pin <= 15)   ||
	    (pin >= 25 	&& pin <= 27)   ||
	    (pin >= 32 	&& pin <= 36)   ||
	    (pin == 39))){
        
        return false;
    }
    return true;
}

// GPIOs configured as outputs:

bool is_valid_digital_output(uint8_t pin){
    if(!((pin >= 0 && pin <= 19) ||
	    (pin >= 21 	&& pin <= 23) ||
	    (pin >= 25 	&& pin <= 27) ||
	    (pin >= 32 	&& pin <= 33))){
        
        return false;
    }
    return true;
}

bool is_valid_pwm_output(uint8_t pin){
    return is_valid_digital_output(pin);
}




// **** Functions for button object:

// Function to configure GPIO as button:
void hal_button_init(button_t *btn, uint8_t pin, gpio_mode_e pull_mode, 
	bool invert_logic){
        
    if(!is_valid_digital_input(pin)){
        printf("\nError: Invalid pin (GPIO %d) for digital input.\n", (int) pin);
        return;
    }
    
    switch(pull_mode){
        case INPUT_MODE:            pinMode(pin, INPUT);            break;
        case INPUT_PULLUP_MODE:     pinMode(pin, INPUT_PULLUP);     break;
        case INPUT_PULLDOWN_MODE:   pinMode(pin, INPUT_PULLDOWN);   break;
        default:
            printf("\nError: Invalid gpio/pull mode for digital input (GPIO %d). Please use INPUT_MODE, INPUT_PULLUP_MODE or INPUT_PULLDOWN_MODE.\n", pin);
            return;
    }
    
    btn->_pin               = pin;
    btn->_is_initialized    = true;
    btn->_inverted_logic    = invert_logic;
    btn->_state             = digitalRead(pin);
    btn->_past_state        = btn->_state;
    btn->_debounce_time_ms      = BTN_DEFAULT_DEBOUNCE_TIME_MS;
    btn->_last_press_time_ms    = 0;

    return;
}


// Functions to set and read button state:

bool hal_button_get_state(button_t *btn){
    if(!btn->_is_initialized) return false;

    btn->_state = (btn->_inverted_logic) ? !digitalRead(btn->_pin) : digitalRead(btn->_pin);
    return btn->_state;
}

bool hal_button_get_past_state(button_t *btn){
    if(!btn->_is_initialized) return false;

    return btn->_past_state;
}

void hal_button_set_past_state(button_t *btn){
    if(!btn->_is_initialized) return;

    btn->_past_state = hal_button_get_state(btn);
    return;
}


// Function to identify if button was pressed (transition sensitive):
bool hal_button_was_pressed(button_t *btn){
    if(!btn->_is_initialized) return false;

	uint32_t now = esp_timer_get_time() / 1000;
	
	if(now - btn->_last_press_time_ms < btn->_debounce_time_ms) 
		return false;
	
	
	bool is_pressed = false;
	
	if(hal_button_get_state(btn) == true && hal_button_get_past_state(btn) == false){
		is_pressed = true;
		btn->_last_press_time_ms = now;
	}
	
	hal_button_set_past_state(btn);
	return is_pressed;
}


// Functions to change button configuration:
void hal_button_change_debounce_time(button_t *btn, uint16_t new_debounce_time_ms){
    if(!btn->_is_initialized) return;

    btn->_debounce_time_ms = new_debounce_time_ms;
    return;
}




// **** Functions for LED object:

// Function to configure GPIO as LED:
void hal_led_init(led_t *led, uint8_t pin, bool invert_logic){
    if(!is_valid_digital_output(pin)){
        printf("\nError: Invalid pin (GPIO %d) for digital output.\n", (int) pin);
        return;
    }
    
    pinMode(pin, OUTPUT);
    
    led->_pin               = pin;
    led->_is_initialized    = true;
    led->_inverted_logic    = invert_logic;
    led->_state             = false;

    return;
}


// Functions to set and read LED state:
uint8_t hal_led_get_state(led_t *led){
    if(!led->_is_initialized) return 0;
    
    return led->_state;
}

void hal_led_set_state(led_t *led, uint8_t state){
    if(!led->_is_initialized) return;
    
    led->_state = state;
    digitalWrite(led->_pin, (led->_inverted_logic) ? !((bool)state) : ((bool)state));
    return;
}


// Functions to turn on, turn off, and toggle LED:

void hal_led_turn_on(led_t *led){
    if(!led->_is_initialized) return;
    
    led->_state = true;
    digitalWrite(led->_pin, (led->_inverted_logic) ? false : true);
    return;
}

void hal_led_turn_off(led_t *led){
    if(!led->_is_initialized) return;
    
    led->_state = false;
    digitalWrite(led->_pin, (led->_inverted_logic) ? true : false);
    return;
}

void hal_led_toggle(led_t *led){
    if(!led->_is_initialized) return;
    
    led->_state = (led->_inverted_logic) ? digitalRead(led->_pin) : !digitalRead(led->_pin);
    digitalWrite(led->_pin, led->_state);
    return;
}



