
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"


void data_gen(float * data_x, float * data_y, int numValues);

double getTime(void);

int main(){

	const int dataPoints = 16; //total number of data points, should be a multiples of 4

	int i; // loop counter
	double startTime, cpuDuration, dfeDuration; // timer values

	//allocate memory for input data and coefficients
	float *x = calloc(dataPoints, sizeof(float)); // input data x
	float *y = calloc(dataPoints, sizeof(float)); // input data y
	float *a_cpu = calloc(dataPoints, sizeof(float)); // coefficients a (intercept), from CPU
	float *b_cpu = calloc(dataPoints, sizeof(float)); // coefficients b (slope), from CPU
	float *a_dfe = calloc(dataPoints, sizeof(float)); // coefficients a (intercept), from DFE
	float *b_dfe = calloc(dataPoints, sizeof(float)); // coefficients b (slope), from DFE

	//generating input data
	printf("Generating input data...\n");
	data_gen(x, y, dataPoints);

	//CPU computing
	printf("Computing on CPU...\n");
	float sumX = 0;  // sum of x_i
	float sumX2 = 0; // sum of (x_i)^2
	float sumXY = 0; // sum of (x_i * y_i)
	float sumY = 0;  // sum of y_i
	float meanX = 0; // mean value of x
 	float meanY = 0; // mean value of y

 	startTime = getTime();

	for(i = 0; i < dataPoints; i++){
		sumX  +=  x[i];
		sumX2 +=  x[i]*x[i];
		sumXY +=  x[i]*y[i];
		sumY  +=  y[i];

		meanX = sumX / (i+1);
		meanY = sumY / (i+1);

		if(i==0){
			b_cpu[i] = 0; //cannot compute slope for first point, set to 0 instead
		}
		else{
			b_cpu[i] = (sumXY - sumX * meanY) / (sumX2 - sumX * meanX); //compute slope
		}

		a_cpu[i] = meanY - b_cpu[i] * meanX; //compute intercept
	}

	cpuDuration = getTime() - startTime;

	//DFE computing
	printf("Computing on DFE...\n");

	startTime = getTime();
	LinearRegression(dataPoints, x, y, a_dfe, b_dfe);
	dfeDuration = getTime() - startTime;

	printf("DFE Done\n");

	//output results
	for(i = 0; i < dataPoints; i++){
		printf("CPU: y = %f + %f x\n", a_cpu[i], b_cpu[i]);
		printf("DFE: y = %f + %f x\n", a_dfe[i], b_dfe[i]);
	}

	printf("CPU compute time: %g s\n", cpuDuration);
	printf("DFE compute time: %g s\n", dfeDuration);

	//deallocate memory
	free(x);
	free(y);
	free(a_cpu);
	free(b_cpu);
	free(a_dfe);
	free(b_dfe);

    return 0;
}

// generate input data for regression
void data_gen(float * data_x, float * data_y, int numValues){
	printf("Input data is a linear function: 0.5 *x - 27 with randon noise added.\n");
	int i;
	for(i = 0; i<numValues; i++){
		data_x[i] = (float)i;
		data_y[i] = 0.5  * (float)i - 27 + (float)rand()/RAND_MAX - 0.5; // linear function with some random variations
	}
}

//time measurement
double getTime(void) {
   struct timeval time;
   gettimeofday(&time, NULL);
   return time.tv_sec + 1e-6 * time.tv_usec;
}

