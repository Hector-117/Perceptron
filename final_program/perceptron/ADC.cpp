
/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Custom libraries:
#include "HAL.hpp"
#include "BSP.hpp"




/* ******************************************************************** */
/* ******************* GLOBAL VARIABLES AND OBJECTS ******************* */

uint8_t global_adc_resolution = ADC_DEFAULT_RESOLUTION;




/* ******************************************************************** */
/* ********************** FUNCTIONS' DEFINITIONS ********************** */

// **** Functions for potentiometer object:

// Function to configure GPIO as a potentiometer (set an ADC channel):
void hal_adc_init(adc_input_t *adc_object, uint8_t pin,
    bool invert_logic){
        
    if(!is_valid_adc_input(pin)){
        printf("\nError: Invalid pin (GPIO %d) for ADC input.\n", (int) pin);
        return;
    }
    
    pinMode(pin, INPUT);
    analogReadResolution(global_adc_resolution);    // 12 bits: 0 - 4095.
    analogSetPinAttenuation(pin, ADC_11db);         // 0 - 3.1 V.

    adc_object->_pin            = pin;
    adc_object->_is_initialized = true;
    adc_object->_inverted_logic = invert_logic;
    adc_object->_state          = analogRead(pin);

}

// Functions to set and read ADC channel state:
uint16_t hal_adc_read(adc_input_t *adc_object){
    if(!adc_object->_is_initialized) return 0;

    adc_object->_state = (adc_object->_inverted_logic) ? (pow(2, global_adc_resolution) - 1) - analogRead(adc_object->_pin) : analogRead(adc_object->_pin);
    return adc_object->_state;
}


// Functions to change ADC configuration:

void hal_adc_set_resolution(uint8_t new_global_adc_resolution){
    if(new_global_adc_resolution <= ADC_DEFAULT_RESOLUTION){
        global_adc_resolution = new_global_adc_resolution;
        analogReadResolution(global_adc_resolution);
    }

    return;
}

void hal_adc_set_channel_attenuation(adc_input_t *adc_object, 
	uint8_t adc_channel_attenuation){

    if(!adc_object->_is_initialized) return;

    if(adc_channel_attenuation <= ADC_11db)
        analogSetPinAttenuation(adc_object->_pin, (adc_attenuation_t)adc_channel_attenuation);

    return;
}

