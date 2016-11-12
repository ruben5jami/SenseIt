#ifndef LAYER
#define LAYER

#include <Arduino.h>
#include <ICommunicationable.h>
#include <LinkedList.h>
#include <WatchDog.h>
#include <CommonValues.h>

template <class T>
//in green house project,'T' stands for Message

/*
base class that represent a layer in the project basic stuff that all layers do and need to impement.
*/
class Layer {
	public:
		virtual ~Layer() {}
		Layer() {
			watchDog.enable(CommonValues::watchDogSecondsUntilReset);
		}
		virtual void initLayer(int) = 0;
		virtual void analyze() = 0;
		virtual void decodeMessage(T&) = 0;
		virtual void prepareMessage(T&, int) = 0;
		void addCommunication(ICommunicationable* type) {
			communicationList.add(type);
		}
		void removeCommunication(int index) {
			for (int i = 0; i < communicationList.size(); i++) {
				if (i == index) {
					communicationList.remove(i);
					break;
				}
			}
		}	
		LinkedList<ICommunicationable*>& getCommunicationList() {
			return communicationList;
		}
		int getLoopTime() {
			return this->loopTime;
		}
		WatchDog& getWatchDog() {
			return this->watchDog;	
		}
		void setLoopTime(int time) {
			this->loopTime = time;	
		}
		void sendUnsentImportantMessages() {
			Serial.print(F("unsentImportantMessages.size() "));
			Serial.println(unsentImportantMessages.size());
			for (int i = 0; i<unsentImportantMessages.size(); i++) {
				Serial.print(F("unsentImportantMessages.get(i) "));
				boolean isSent = communicationList.get(CommonValues::radioIndex)->sendMessage(unsentImportantMessages.get(i));
				Serial.println(isSent);
				if (isSent) {
					unsentImportantMessages.remove(i);
				}
			}
		}
	protected:
		int loopTime;
		LinkedList<ICommunicationable*> communicationList;
		LinkedList<T> unsentImportantMessages;
		WatchDog watchDog;
};

#endif
