#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// **** Training data configuration:
int dimensions			= 2;	// No. of values/elements in an input vector (a data sample).
int training_selector	= 1;	// Datasets for truth tables: 1 == AND; 2 == OR.


int main(void){
	
	// **** Step 1: Creates dataset according to the selected truth table:
	
	int 	number_of_samples = (int) pow(2, dimensions); 	// Number of rows in the truth table.
	float 	dataset[number_of_samples][dimensions];	
		// dataset[<samples and their labels/classes>][<values>] 
		// = truth_table[<rows or cases>][<columns or inputs and output values>].
	
	
	// Fills input values of truth table (input vectors in the dataset):
	for(int col = 0; col < number_of_samples; col++){
		
		// Period to alternate input values (0 and 1):
		int period = (int) pow(2, (dimensions - (col + 1)));
		
		// Fills every value in a row (sample):
		int input_value = 0;
		for(int row = 0; row < number_of_samples; row++){
			dataset[row][col] = (float) input_value;	// Assigns value to truth table cell.
			
			if((row + 1) % period == 0)			// Switches value if period is completed.
				input_value = !input_value;
		}
	}
	
	
	// Fills output value of truth table (labels or classes in the dataset):
	switch(training_selector){
		// AND:
		case 1:
			for(int row = 0; row < number_of_samples; row++){
				int output = (int) dataset[row][0];
				
				for(int col = 1; col < dimensions; col++)
					output &= (int) dataset[row][col];
				
				dataset[row][dimensions] = (float) output;
			}
			break;
		
		// OR:
		case 2:
			for(int row = 0; row < number_of_samples; row++){
				int output = (int) dataset[row][0];
				
				for(int col = 1; col < dimensions; col++)
					output |= (int) dataset[row][col];
				
				dataset[row][dimensions] = (float) output;	
			}
			break;
		
		// Any other case (error):
		default:
			return 1;
	}
	
	
	// Prints dataset (truth table):
	puts("________________________________________________________\n");
	puts("Training data (truth table):\n");
	
	// Truth table's header (variables' names):
	for(int col = 0; col < dimensions; col++)
		printf("| \tX%d \t", col);
		
	printf("| \tY \t|\n");
	
	// Truth table's body (values):
	for(int row = 0; row < number_of_samples; row++){
		for(int col = 0; col <= dimensions; col++)
			printf("| \t%d \t", (int) dataset[row][col]);
		
		printf("|\n");
	}
	
	
	
	
	// **** Step 2: Creates training data arrays from dataset:
	
	float x[number_of_samples][dimensions];	// Array of input vectors.
	float y[number_of_samples];				// Array of labels/classes (desired outputs).
	
	// Assigns values of the truth table to the training data arrays:
	for(int row = 0; row < number_of_samples; row++){
		for(int col = 0; col < dimensions; col++)
			x[row][col] = dataset[row][col]; // Assigns values to array of input vectors.
			
		// Assigns dataset's last column of values to array of labels:
		y[row] = dataset[row][dimensions];
	}
	
	
	
	
	// **** Step 3: Training:
	
	puts("\n________________________________________________________\n");
	puts("Training LDA with LMS...\n");
	
	
	// Stop conditions for training:
	float 	min_error	= (float) 0.01;	// Minimum acceptable error in each training sample.
	int 	max_epochs	= 500;		// Maximum number of epochs/iterations during training.
	
	// Training parameters:
	float	n = 0.5;					// Training rate: Defines the weights' adjustemnt for each epoch.
	float 	error[number_of_samples];	// Error in each training sample.
	float	w[dimensions + 1];			// Model's weights: w = [w1, w2, ..., wp, w0].
	
	
	// Initializes error values with 1:
	for(int row = 0; row < number_of_samples; row++)
		error[row] = 1;
	
	// Initializes weights with random values:
	
	puts("Initializing weights with random values:\n");
	
	srand((unsigned int) time(NULL)); // Seeds the random number generator with current time.
	
	for(int i = 0; i <= dimensions; i++){
		w[i] = (((float)rand()) / ((float)RAND_MAX));
		printf("%.3f \t", w[i]);
	}
	puts("\n");
	
	
	// Trains perceptron with LMS (Least Mean Squares) method:
	
	for(int epoch = 0; epoch < max_epochs; epoch++){
		
		// Epoch: Adjusts the weights according to every training sample and its label:
		for(int sample = 0; sample < number_of_samples; sample++){
			
			// The model predicts the sample's label according to its current weights:
			float r = 0;
			
			for(int i = 0; i < dimensions; i++)
				r += w[i] * x[sample][i]; 		// r = w1*x1 + w2*x2 + ... wp*xp + ...
			r += w[dimensions];					// ... + w0.
			
			
			// Model's activation function (step function):
			int out = (r >= 0) ? 1 : 0;
			
			// Gets error between prediction and real output:
			error[sample] = y[sample] - ((float)out);
			
			// Adjusts new weights accordingly:
			for(int i = 0; i < dimensions; i++)
				w[i] += n * error[sample] * x[sample][i]; // w1, w2, ..., wp.
			
			w[dimensions] += n * error[sample]; // w0.
		}
		
		// Gets number of errors above minimum error:
		int errors_greater_than_min_error = 0;
		for(int sample = 0; sample < number_of_samples; sample++)
			errors_greater_than_min_error += (fabs(error[sample]) >= min_error) ? 1 : 0;
			
		printf("Epoch: %d \t| Number of errors above threshold: %d\n", epoch, errors_greater_than_min_error);
		
		if(errors_greater_than_min_error == 0)	// If the number of errors equals 0. Stops training.
			break;
	}
	
	
	// Training finished. Prints final weights:
	puts("\nTraining finished. These are the resulting weights:\n");
	
	for(int i = 0; i <= dimensions; i++)
		printf("%.3f \t", w[i]);
	
	
	
	
	// **** Step 4: Testing:
	
	puts("________________________________________________________\n");
	puts("Running tests...\n");
	
	int out[number_of_samples];
	int number_of_errors = 0;
	
	for(int sample = 0; sample < number_of_samples; sample++){
			
		// The model predicts the sample's label according to its current weights:
		float r = 0;
		
		for(int i = 0; i < dimensions; i++)
			r += w[i] * x[sample][i]; 		// r = w1*x1 + w2*x2 + ... wp*xp + ...
		r += w[dimensions];					// ... + w0.
		
		// Model's activation function (step function):
		out[sample] = (r >= 0) ? 1 : 0;
		
		// Checks if there was an error between prediction and real output:
		if(out[sample] != y[sample])
			number_of_errors++;
	}
	
	
	// Prints results of all testing samples.
	puts("Results: ");
	 
	for(int sample = 0; sample < number_of_samples; sample++)
		printf("%d ", out[sample]);
		
	printf("\n\nSuccessful predictions: %d/%d\n\n", number_of_samples - number_of_errors, number_of_samples);
	puts("Testing finished.");
	
	return 0;
}