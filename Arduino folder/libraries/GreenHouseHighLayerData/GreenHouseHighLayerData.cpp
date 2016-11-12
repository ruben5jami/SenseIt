#include "GreenHouseHighLayerData.h"

GreenHouseHighLayerData::GreenHouseHighLayerData() {
  this->id = 0;
  this->values = LinkedList<DataValue>();
  this->working = true;
}

GreenHouseHighLayerData::GreenHouseHighLayerData(int id) {
    this->id = id;
    this->values = LinkedList<DataValue>();
    this->working = true;
}

void GreenHouseHighLayerData::updateValue(String name, float value, DateTime dateTime) {
  int i;
  boolean found = false;
  for (i = 0 ; i < values.size() ; i++) {
    if (values.get(i).name == name) {
      DataValue data = values.get(i);
      data.value = value;
      data.dateTime = dateTime;
      values.set(i,data);
      found = true;
    }
  }

  if (!found) {
    DataValue data;
    data.name = name;
    data.value = value;
    data.dateTime = dateTime;
    values.add(data);
  }
}

int GreenHouseHighLayerData::getId() {
  return id;
}

void GreenHouseHighLayerData::setId(int id) {
  this->id = id;
}

int GreenHouseHighLayerData::getValuesSize() {
  return values.size();
}

DataValue GreenHouseHighLayerData::getValue(int i) {
  return values.get(i);
}

unsigned long GreenHouseHighLayerData::getLastChecked() {
  return lastChecked;
}

void GreenHouseHighLayerData::setLastChecked(unsigned long lastChecked) {
  this->lastChecked = lastChecked;
}

 boolean GreenHouseHighLayerData::getWorking() {
  return working;
}

void GreenHouseHighLayerData::setWorking(boolean working) {
  this->working = working;
}
