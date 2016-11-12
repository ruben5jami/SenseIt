#ifndef LOWERLAYER
#define LOWERLAYER

#include <Sensor.h>
#include <Actuator.h>
#include <Layer.h>
#include <Actuatorable.h>
#include <Sensorable.h>


template <class T>
//in green house project,'T' stands for Message, 
class LowerLayer :public Layer<T>,public Actuatorable,public Sensorable<T>{
	public:
		/*Layer methods*/
		virtual ~LowerLayer() {}
		virtual void initLayer(int) = 0;
		virtual void analyze() = 0;
		virtual void decodeMessage(T&) = 0;
		virtual void prepareMessage(T&, int) = 0;

};

#endif
