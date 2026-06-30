/**
 * @file    perceptron.ino (project: perceptron in Arduino IDE)
 * @brief   Example program for implementing (training and testing) a perceptron
 * 			    neuron in c and language.
 *
 * @details This example shows our take on the perceptron in ESP32 practice.
 *
 * @author  Abdiel Alejandro Rodríguez Coronado <https://github.com/alexrc14>.
 * @author	Héctor Said Herrera Niño 				    <https://github.com/Hector-117>.
 * @date    2026-06-29 (last updated).
 * @version 1.0
 *
 * @license None
 */


/* ******************************************************************** */
/* **************************** LIBRARIES ***************************** */

// Standard libraries:
#include <stdint.h>
#include <stdbool.h>

// Custom libraries:
#include "timing.h"
#include "perceptron_library.h"
#include "HAL.hpp"
#include "PRINT.h"
#include "BSP.hpp"




/* ******************************************************************** */
/* ********************* PERCEPTRON CONFIGURATION ********************* */

#define TRAINING_SELECTOR 1
// 1 == AND; 2 == OR; 3 == NAND; 4 == NOR;
// 5 == XOR; 6 == XNOR; 7 == Custom.

#define CUSTOM_TRAINING_DATA_SELECTOR 1
// 0 == User selected; n == Program selected n.

// Custom training datasets (program selected):
// Note:	custom_training_data_selector = Number of training dataset selected.
// Note:	First columns of training datasets' arrays: xi (input vector).
//			  Last column of training datasets' arrays:	yi (desired output / label).




/* ******************************************************************** */
/* ************************ PIN CONFIGURATION ************************* */

// LED that shows system output in binary: {0, 1}.
#define LED_OUTPUT_PIN	13	// System output LED.

// LED that shows system mode: {OFF, RUN}.
//#define LED_MODE_PIN	9	  // System mode LED.

// Button that changes system mode:
#define BUTTON_PIN		1	// System mode button.


// System inputs' pins (sensors/potentiometers):
#define NUMBER_OF_POTS 5
#define POTS_THRESHOLD 2048

#define POT_1_PIN 2
#define POT_2_PIN 4
#define POT_3_PIN 5
#define POT_4_PIN 6
#define POT_5_PIN 7




/* ******************************************************************** */
/* ******************* GLOBAL VARIABLES AND OBJECTS ******************* */

// **** Perceptron model variables:

perceptron_model_t model = {0};

double custom_training_data_1[4][3] = { // Custom training dataset 1.
  {0, 0, 0},
  {0, 1, 1},
  {1, 0, 1},
  {1, 1, 0},
};




// **** Global variables:

// Training data configuration:
uint32_t  dimensions = NUMBER_OF_POTS;
uint8_t   training_selector = TRAINING_SELECTOR;
uint8_t   custom_training_data_selector = CUSTOM_TRAINING_DATA_SELECTOR;

bool button_pressed_flag = false;

uint8_t system_mode = 0;
// system_mode == 0 	-> OFF
// system_mode == 1 	-> ON (RUN)

double output = 0;

double pots_values[NUMBER_OF_POTS] = {0};


#if NUMBER_OF_POTS == 2
uint8_t pots_pins[NUMBER_OF_POTS] = {
  POT_1_PIN, POT_2_PIN
};

#elif NUMBER_OF_POTS == 3
uint8_t pots_pins[NUMBER_OF_POTS] = {
  POT_1_PIN, POT_2_PIN, POT_3_PIN
};

#elif NUMBER_OF_POTS == 4
uint8_t pots_pins[NUMBER_OF_POTS] = {
  POT_1_PIN, POT_2_PIN, POT_3_PIN, 
  POT_4_PIN
};

#elif NUMBER_OF_POTS == 5
uint8_t pots_pins[NUMBER_OF_POTS] = {
  POT_1_PIN, POT_2_PIN, POT_3_PIN, 
  POT_4_PIN, POT_5_PIN
};

#endif




// **** Global objects:

// System inputs:

button_t button;
potentiometer_t pots[NUMBER_OF_POTS];

// System outputs:

//led_t led_system_mode; -> led_builtin
led_t led_system_output;




/* ******************************************************************** */
/* ************************** MAIN FUNCTIONS ************************** */

// **** Hardware setup:

void setup(){
  Serial.begin(115200);

  if(NUMBER_OF_POTS < 2 || NUMBER_OF_POTS > 5){
		printf("\nError: NUMBER_OF_POTS must be between 2 and 5.\n\n"); 
		while(true);
  }

  // Initializes button object:
  hal_button_init(&button, BUTTON_PIN, INPUT_PULLUP_MODE, true);

  // Initializes LED objects:
  bsp_init(); // Initializes system mode LED = builtin LED = GPIO 2
  hal_led_init(&led_system_output, LED_OUTPUT_PIN, false);

  // Initializes potentiometers pins as ADC objects:
	for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    hal_adc_init(&pots[i], pots_pins[i], false);
	
  return;
}


// **** Main system loop:

void loop(){
		
  // **** System start/stop controlled by button:
  if(hal_button_was_pressed(&button)){
    ++system_mode;
    
    switch(system_mode){
      case 0:	// Mode: OFF.
        bsp_turn_off_builtin_led(); print_welcome_message(); break;

      case 1:	// Mode: RUN.
        bsp_turn_on_builtin_led(); break;

      default:
        // **** 7th Step [VERY IMPORTANT]: Free allocated memory used by perceptron model.
        perceptron_free(&model);
        bsp_turn_off_builtin_led();
        hal_led_turn_off(&led_system_output);
        system_mode = 0;
        break;
    }
  }
  
  // **** System in RUN mode:

  // Model training:
  if(system_mode == 1){
    
    // **** 1st Step: Create truth table. This data will be used for training:
    
    uint32_t samples_number;	// No. of rows for training data.
    double** training_samples;
    
    // Selection of training data:
    
    if(training_selector < 7){ 	// Logic gates' truth tables as training data.
      samples_number = (uint32_t) pow(2, (double) dimensions); // Num. rows for truth table.
      training_samples = perceptron_get_truth_table(dimensions, training_selector);
    }
    else if(custom_training_data_selector == 0){	// User selected training data (user input).
      printf("Unfinished code in this option.");
      while(true) delay(10);
      // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MISSING CODE
    }
    else if(custom_training_data_selector == 1){ // Custom training dataset 1.
      samples_number 	= sizeof(custom_training_data_1) / sizeof(custom_training_data_1[0]);
      dimensions		= sizeof(custom_training_data_1[0]) / sizeof(custom_training_data_1[0][0]) - 1;
      
      // Creates dinamically allocated array and assigns its pointers to training_samples:
      training_samples = (double**) malloc((size_t)samples_number * sizeof(double*));
      
      for(uint32_t i = 0; i < samples_number; i++)
        training_samples[i] = (double*) malloc((size_t) (dimensions + 1) * sizeof(double));
      
      // Assigns values from custom_training_data_1 to training_samples:
      for(uint32_t row = 0; row < samples_number; row++)	// All samples.
        for(uint32_t col = 0; col <= dimensions; col++)	// xi and yi (<= instead of <).
          training_samples[row][col] = custom_training_data_1[row][col];
    }
    else{
      samples_number = 0;
      training_samples = NULL;
      while(true) delay(10);
    }
    
    puts("________________________________________________________\n");
    puts("Training data (truth table):\n");
    
    perceptron_print_data_table(training_samples, dimensions, samples_number);
    
    
    
    
    // **** 2nd Step: Create training data arrays from truth table:
    
    double x[samples_number][dimensions];	// Creates the array of input vectors.
    double y[samples_number];				// Creates the array of labels (desired outputs).
    
    // Assigns the values of the truth table to the training data arrays:
    for(uint32_t row = 0; row < samples_number; row++){
      for(uint32_t col = 0; col < dimensions; col++)
        x[row][col] = training_samples[row][col];	// Assings values to array of input vectors.
      
      // Assigns training_samples's last column of values (2nd index = dimensions) to array of labels:
      y[row] = training_samples[row][dimensions];
    }
    
    perceptron_free_data_table(training_samples, samples_number);	// Frees allocated memory for truth table.
    
    
    
    
    // **** 3rd Step: Creation and initialization of perceptron model:
    
    perceptron_init(&model, dimensions, STEP);
    
    
    // **** 4th Step: Training (Least Mean Squares method):
    
    // Cycle control variables:
    double min_error 	= 0.01;	// Minimum acceptable error in each training sample.
    uint32_t max_epochs	= 500;	// Maximum number of iterations/epochs.
    
    // Parameters:
    double n = 0.5;					// Training rate.
    
    // Trains model:
    perceptron_train(
      &model, 
      dimensions, 
      samples_number, 
      &x[0][0], 
      y, 
      LMS, 
      min_error, 
      max_epochs, 
      n, 
      true
    );
    
    
    // **** 5th Step: Testing:
    perceptron_test(
      &model, 
      dimensions, 
      samples_number, 
      &x[0][0], 
      y, 
      true
    );
    
    
    // **** 6th step: Inference:
    printf("\nStarting inference...\n\n");
    
    system_mode = 2;
  }

  // Model inference:
  if(system_mode == 2){
    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++){
      if(hal_adc_read(&pots[i]) >= POTS_THRESHOLD)
        pots_values[i] = 1;
      else
        pots_values[i] = 0;

      printf("%d\t", hal_adc_read(&pots[i]));
    }
    printf("\n");
    
    output = perceptron_predict(&model, &pots_values[0], dimensions);
    
    hal_led_set_state(&led_system_output, (bool)output);
  
    delay(100);
  }
  
  delay(20);
  return;
}
