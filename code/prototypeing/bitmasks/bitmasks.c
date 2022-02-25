#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define powerButton_pin 34 // pin 34 connected to Red button
#define motorUpButton_pin 21 // pin 21 connected to Yellow button
#define motorDownButton_pin 18 // pin 18 connected to Black button
#define fanUpButton_pin 23 // pin 23 connected to Blue button
#define fanDownButton_pin 22 // pin 22 connected to Green button

#define bitwiseCalculation ((1UL << powerButton_pin) | (1UL << motorUpButton_pin) | (1UL << motorDownButton_pin) | (1UL << fanUpButton_pin) | (1UL << fanDownButton_pin))

#define POW_Calculation (pow(2, powerButton_pin) + pow(2, motorUpButton_pin) + pow(2, motorDownButton_pin)\
							 + pow(2, fanUpButton_pin) + pow(2, fanDownButton_pin))

int main(int argc, char const *argv[])
{
	printf("bitwiseCalculation: 0x%016lX\n",bitwiseCalculation);
	printf("POW_Calculation: 0x%016lX\n",POW_Calculation);
	return 0;
}