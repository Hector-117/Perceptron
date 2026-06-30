#ifndef BSP_HPP
#define BSP_HPP


/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <Adafruit_NeoPixel.h>




/* ******************************************************************** */
/* ********************** BOARD COMPONENTS' PINS ********************** */

#define LED_BUILTIN_PIN     48
#define MAX_LED_BRIGHTNESS  80
#define NUMPIXELS           1 




/* ******************************************************************** */
/* ******************* GLOBAL VARIABLES AND OBJECTS ******************* */

extern uint8_t led_builtin_state[3];



/* ******************************************************************** */
/* ********************** FUNCTIONS' PROTOTYPES *********************** */


// **** Functions for GPIO function validation:

// GPIOs configured as inputs:
bool is_valid_digital_input(uint8_t pin);
bool is_valid_adc_input(uint8_t pin);

// GPIOs configured as outputs:
bool is_valid_digital_output(uint8_t pin);
bool is_valid_pwm_output(uint8_t pin);


// **** Functions for builtin hardware components:

void bsp_init(void);

void bsp_turn_on_builtin_led(void);
void bsp_turn_off_builtin_led(void);
void bsp_toggle_builtin_led(void);

void bsp_set_rgb_color(uint8_t r, uint8_t g, uint8_t b);


#endif /* BSP_HPP */