#include "Settlement.h"

Settlement:: Settlement(const string &name, SettlementType type):name(name),type(type){};
const string  & Settlement::getName() const{return name;};
SettlementType Settlement::getType() const{return type;};
const string Settlement::toString() const{return "settlement<"+name+"><"+std::to_string(static_cast<int>(type))+">";};
