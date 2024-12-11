#include "Facility.h"
#include <iostream>
     
     
      FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
        :name(name),category(category),price(price),lifeQuality_score(lifeQuality_score),economy_score(economy_score),environment_score(environment_score){};
     
       FacilityType::FacilityType(const FacilityType &type):name(type.getName()),category(type.getCategory()),price(type.getCost()),lifeQuality_score(type.getLifeQualityScore()),economy_score(type.getEconomyScore()),environment_score(type.getEnvironmentScore()){};
       
        const string & FacilityType::getName() const{return name;};
     
        int FacilityType::getCost() const{return price;};
     
        int FacilityType::getLifeQualityScore() const{return lifeQuality_score;};
     
        int FacilityType::getEnvironmentScore() const{return environment_score;};
     
        int FacilityType::getEconomyScore() const{return economy_score;};
     
        FacilityCategory FacilityType::getCategory() const{return category;};
     
        const string FacilityType::toString() const{return "facility <" + this->name +"> <"+std::to_string(static_cast<int>(category))+"> <" +std::to_string(price)+"> <"+std::to_string(lifeQuality_score)+"> <"+std::to_string(economy_score)+"> <"+std::to_string(environment_score)+">";};


        Facility:: Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
        :FacilityType(name,category,price,lifeQuality_score,economy_score,environment_score),settlementName(settlementName),timeLeft(price),
        status(FacilityStatus::UNDER_CONSTRUCTIONS){
            
        };

        Facility:: Facility(const FacilityType &type, const string &settlementName):FacilityType(type),settlementName(settlementName),status(FacilityStatus::UNDER_CONSTRUCTIONS),timeLeft(price){};
      
        const string & Facility::getSettlementName() const{return settlementName;};
       
        const int Facility::getTimeLeft() const{return timeLeft;};
       
        FacilityStatus Facility::step(){
            timeLeft=timeLeft-1;
            if (timeLeft==0)
            {
                this->setStatus(FacilityStatus::OPERATIONAL);
            }
            return this->status;  
        };
        void Facility::setStatus(FacilityStatus status){this->status=status;};
        const FacilityStatus& Facility::getStatus() const{return this->status;};
        const string Facility::toString() const{return settlementName + " " + std::to_string(timeLeft) + FacilityType::toString();};
        void Facility::LowerTimeLeftByOne(){
            this->timeLeft = this->timeLeft-1;
        }