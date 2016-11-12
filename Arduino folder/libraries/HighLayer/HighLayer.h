#ifndef HIGH_LAYER
#define HIGH_LAYER
#include <Layer.h>
#include <ICommunicationable.h>

template <class T>
//in green house project,'T' stands for Message

class HighLayer :public Layer<T> {
	public:

		/*Layer methods*/
		virtual ~HighLayer() {}
		virtual void initLayer(int) = 0;
		virtual void analyze() = 0;
		virtual void decodeMessage(T&) = 0;
		virtual void prepareMessage(T&, int) = 0;
		virtual void checkMiddleLayer() = 0;

};

#endif
