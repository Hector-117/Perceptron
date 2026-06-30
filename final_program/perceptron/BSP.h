#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif


/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

// Custom libraries:
#include "HAL.h"




/* ******************************************************************** */
/* ********************** BOARD COMPONENTS' PINS ********************** */

#define LED_BUILTIN_PIN 2




/* ******************************************************************** */
/* ******************* GLOBAL VARIABLES AND OBJECTS ******************* */

extern led_t led_builtin;




/* ******************************************************************** */
/* ********************** FUNCTIONS' PROTOTYPES *********************** */

void bsp_init(void);

void bsp_turn_on_builtin_led(void);
void bsp_turn_off_builtin_led(void);
void bsp_toggle_builtin_led(void);

bool bsp_get_builtin_led_state(void);
void bsp_set_builtin_led_state(uint8_t state);



#ifdef __cplusplus
}
#endif

#endif /* BSP_H */