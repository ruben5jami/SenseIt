#ifndef GREEN_HOUSE_HIGH_LAYER_THRESHOLDS
#define GREEN_HOUSE_HIGH_LAYER_THRESHOLDS

#include <Arduino.h>
#include <LinkedList.h>

struct ThresholdsValue {
  String name;
  float minValue;
  float maxValue;
  String last_updated;
};

class GreenHouseHighLayerThresholds {
private:
  int id;
  LinkedList<ThresholdsValue> values;
public:
  GreenHouseHighLayerThresholds();
  GreenHouseHighLayerThresholds(int);
  void updateValue(String,float,float,String);
  int getId();
  void setId(int);
  int getValuesSize();
  ThresholdsValue getValue(int i);
  ThresholdsValue getValueByName(String name);

};

#endif
