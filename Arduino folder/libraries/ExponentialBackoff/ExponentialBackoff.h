#include <Arduino.h>
class ExponentialBackoff {
private:
	int maxRetries;
public:
	double getDelayTime(int);
	ExponentialBackoff(int);
};
