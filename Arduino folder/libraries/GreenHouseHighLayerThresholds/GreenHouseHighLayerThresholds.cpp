#include "GreenHouseHighLayerThresholds.h"

GreenHouseHighLayerThresholds::GreenHouseHighLayerThresholds() {

}

GreenHouseHighLayerThresholds::GreenHouseHighLayerThresholds(int id) {
    this->id = id;
    this->values = LinkedList<ThresholdsValue>();
}

void GreenHouseHighLayerThresholds::updateValue(String name , float minValue , float maxValue, String last_updated) {
  int i;
  boolean found = false;
  ThresholdsValue data;
  for (i = 0 ; i < values.size() ; i++) {
    if (values.get(i).name == name) {
      data = values.get(i);
      data.minValue = minValue;
      data.maxValue = maxValue;
      data.last_updated = last_updated;
      values.set(i,data);
      found = true;
    }
  }

  if (!found) {
    data.name = name;
    data.minValue = minValue;
    data.maxValue = maxValue;
    data.last_updated = last_updated;
    values.add(data);
  }
}

int GreenHouseHighLayerThresholds::getId() {
  return id;
}

void GreenHouseHighLayerThresholds::setId(int id) {
  this->id = id;
}

int GreenHouseHighLayerThresholds::getValuesSize() {
  return values.size();
}

ThresholdsValue GreenHouseHighLayerThresholds::getValue(int i) {
  return values.get(i);
}

ThresholdsValue GreenHouseHighLayerThresholds::getValueByName(String name) {
    int i;
    ThresholdsValue tValue;
    boolean found = false;
    for (i = 0 ; i < values.size() ; i++) {
        if (values.get(i).name == name) {
            tValue = values.get(i);
            found = true;
        }
    }
    if (!found) {
      tValue.name = "null";
    }

    return tValue;
}
