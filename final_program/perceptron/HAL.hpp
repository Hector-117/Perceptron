#ifndef HAL_HPP
#define HAL_HPP

#ifdef __cplusplus
extern "C" {
#endif

/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>




/* ******************************************************************** */
/* ********************* CONFIGURATION CONSTANTS ********************** */

// Button default debounce constant:
#define BTN_DEFAULT_DEBOUNCE_TIME_MS 200

// ADC default ADC resolution:
#define ADC_DEFAULT_RESOLUTION 12




/* ******************************************************************** */
/* ************************ CUSTOM DATA TYPES ************************* */

// **** Enumerations:

typedef enum{
	OUTPUT_MODE,
	OUTPUT_PWM_MODE,
	INPUT_MODE,
	INPUT_PULLUP_MODE,
	INPUT_PULLDOWN_MODE,
	INPUT_ANALOG_MODE
} gpio_mode_e;


// **** Structs:

// Input types:
typedef struct{
	uint8_t 	_pin;
	bool		_is_initialized;
    bool        _inverted_logic;
	bool 		_state;
	bool 		_past_state;
	uint16_t	_debounce_time_ms;
	uint32_t	_last_press_time_ms;
} digital_input_t;

typedef struct{
	uint8_t 	_pin;
	bool		_is_initialized;
    bool        _inverted_logic;
	uint16_t 	_state;
} adc_input_t;


// Output types:
typedef struct{
	uint8_t _pin;
	bool	_is_initialized;
    bool    _inverted_logic;
	uint8_t _state;
} digital_output_t;

typedef struct{
	uint8_t _pin;
	bool	_is_initialized;
    bool    _inverted_logic;
	uint8_t _state;
} pwm_output_t;



// **** Alias for data types:

// Inputs:
typedef digital_input_t     button_t;
typedef adc_input_t         potentiometer_t;

// Outputs:
typedef digital_output_t    led_t;




/* ******************************************************************** */
/* ******************* GLOBAL VARIABLES AND OBJECTS ******************* */

extern uint8_t global_adc_resolution;




/* ******************************************************************** */
/* ********************** FUNCTIONS' PROTOTYPES *********************** */

// **** Functions for button object:

// Function to configure GPIO as button:
void hal_button_init(
	button_t *btn, 
	uint8_t pin, 
	gpio_mode_e pull_mode, 
	bool invert_logic
);

// Functions to set and read button state:
bool hal_button_get_state(button_t *btn);
bool hal_button_get_past_state(button_t *btn);
void hal_button_set_past_state(button_t *btn);

// Function to identify if button was pressed (transition sensitive):
bool hal_button_was_pressed(button_t *btn);

// Functions to change button configuration:
void hal_button_change_debounce_time(button_t *btn, uint16_t new_debounce_time_ms);




// **** Functions for LED object:

// Function to configure GPIO as LED:
void hal_led_init(led_t *led, uint8_t pin, bool invert_logic);

// Functions to set and read LED state:
uint8_t hal_led_get_state(led_t *led);
void hal_led_set_state(led_t *led, uint8_t state);

// Functions to turn on, turn off, and toggle LED:
void hal_led_turn_on(led_t *led);
void hal_led_turn_off(led_t *led);
void hal_led_toggle(led_t *led);




// **** Functions for potentiometer object:

// Function to configure GPIO as a potentiometer (set an ADC channel):
void hal_adc_init(
	adc_input_t *adc_object, 
	uint8_t pin,
	bool invert_logic
);

// Functions to set and read ADC channel state:
uint16_t hal_adc_read(adc_input_t *adc_object);

// Functions to change ADC configuration:
void hal_adc_set_resolution(uint8_t new_global_adc_resolution);
void hal_adc_set_channel_attenuation(
	adc_input_t *adc_object, 
	uint8_t adc_channel_attenuation
);




#ifdef __cplusplus
}
#endif

#endif /* HAL_HPP */