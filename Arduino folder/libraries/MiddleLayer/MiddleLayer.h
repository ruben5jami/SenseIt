#ifndef MIDDLE_LAYER
#define MIDDLE_LAYER

#include <Layer.h>
#include <Actuatorable.h>

template <class T>

class MiddleLayer :public Layer<T>, public Actuatorable {
	public:

		void addLowerId(int id) {
			lowersIds.add(id);
		}

		void removeLowerId(int id) {
			for (int i = 0; i < lowersIds.size(); ++i) {
				if (lowersIds.get(i) == id) {
					lowersIds.remove(i);
					break;
				}
			}
		}
		/*Layer methods*/
		virtual ~MiddleLayer() {}
		virtual void initLayer(int) = 0;
		virtual void analyze() = 0;
		virtual void decodeMessage(T&) = 0;
		virtual void prepareMessage(T&, int) = 0;


protected:
	LinkedList<int> lowersIds;
};

#endif
