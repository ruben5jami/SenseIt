#include "ExponentialBackoff.h"


double ExponentialBackoff:: getDelayTime(int iteration){
	if (iteration < maxRetries)
		return random(0, pow(2, iteration) - 1)	*51.2;
	else
		return -1;
}

ExponentialBackoff::ExponentialBackoff(int maxRetries) {

	this->maxRetries = maxRetries;
}