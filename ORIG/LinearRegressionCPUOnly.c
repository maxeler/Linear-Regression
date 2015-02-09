/***************************************************************************
*
* Filename : LinearRegressionCPUOnly.c
* Date: 11 Dec 2014
* 
* Description: CPU Only implementation of a linear regression using least 
* squares. The goal is to find a straight line through n data points that
* minimises the sum of the square of errors (least squares).
* 
* Find a,b for y = b*x + a that minimise the error sum (b*x_i + a - y_i)^2
*
* The coefficients a and b are calulated by:
*
* b = (sum(x_i * y_i) - 1/n*(sum x_i)*(sum y_i)) / (sum(x_i^2) - 1/n*(sum x_i)^2)
* a = 1/n(sum y_i) - a/n (sum x_i)
*
* The regression is perfromed in a continous way, i.e. for each new data
* point, the regression is updated with new a,b value pair.
*
* **************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


void data_gen(float * data_x, float * data_y, int numValues);

double getTime(void); 

int main(){

	const int dataPoints = 16; //total number of data points, should be a multiples of 4

	int i; // loop counter
	double startTime, timeDuration; // timer values
	
	//allocate memory for input data and coefficients
	float *x = calloc(dataPoints, sizeof(float)); // input data x
	float *y = calloc(dataPoints, sizeof(float)); // input data y
	float *a = calloc(dataPoints, sizeof(float)); // coefficients a (intercept)
	float *b = calloc(dataPoints, sizeof(float)); // coefficients b (slope)

	//generating input data
	printf("Generating input data...\n");
	data_gen(x, y, dataPoints);

	//compute regression
	printf("Computing on regression...\n");
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
			b[i] = 0; //cannot compute slope for first point, set to 0 instead
		}
		else{
			b[i] = (sumXY - sumX * meanY) / (sumX2 - sumX * meanX); //compute slope
		}

		a[i] = meanY - b[i] * meanX; //compute intercept
	}

	timeDuration = getTime() - startTime;

	//output Results
	for(i = 0; i < dataPoints; i++){
		printf("regression[%d]: y = %f + %f x\n",i, a[i], b[i]);
	}

	printf("compute time: %g s\n", timeDuration);

	//deallocate memory
	free(x);
	free(y);
	free(a);
	free(b);
	
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
