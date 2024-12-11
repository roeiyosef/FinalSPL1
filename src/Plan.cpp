#include "Plan.h"
#include "Facility.h"
#include <iostream>
Plan:: Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
:plan_id(planId),settlement(settlement),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilityOptions(facilityOptions),
life_quality_score(0),economy_score(0),environment_score(0),constructionLimit(static_cast<int>(settlement.getType()) + 1),facilities(),underConstruction(){};

Plan :: Plan(const Plan& other)
:plan_id(other.plan_id),status(other.status),
life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score),
constructionLimit(other.constructionLimit),facilityOptions(other.facilityOptions),settlement(other.settlement){
    this->selectionPolicy = other.selectionPolicy->clone();
    for (int i = 0; i < other.facilities.size(); i++)
    {
        this->facilities.push_back(new Facility(*other.facilities[i]));
    }
    for (int i = 0; i < other.underConstruction.size(); i++)
    {
        this->underConstruction.push_back(new Facility(*other.underConstruction[i]));
    }    
};

Plan::~Plan(){
    delete this->selectionPolicy;
     for (Facility* facility : facilities) {
        delete facility;
    }
    for (Facility* underConstructionFacility : underConstruction) {
        delete underConstructionFacility;
    }
};

Plan:: Plan(Plan&& other) noexcept:plan_id(other.plan_id),status(other.status),
life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score),
constructionLimit(other.constructionLimit),facilityOptions(other.facilityOptions),settlement(other.settlement){
    this->selectionPolicy = other.selectionPolicy->clone();
    for (int i = 0; i < other.facilities.size(); i++)
    {
        this->facilities.push_back(new Facility(*other.facilities[i]));
        delete other.facilities[i];
        other.facilities[i]=nullptr;
    }
    for (int i = 0; i < other.underConstruction.size(); i++)
    {
        this->underConstruction.push_back(new Facility(*other.underConstruction[i]));
        delete other.underConstruction[i];
         other.underConstruction[i]=nullptr;
    }  

    delete other.selectionPolicy;
    other.selectionPolicy = nullptr;
};

const int Plan::getlifeQualityScore() const{return life_quality_score;};
const int Plan::getEconomyScore() const{return economy_score;};
const int Plan::getEnvironmentScore() const{return environment_score;};

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
};

 const string Plan::getPlansSettlmentName() const{
    return settlement.getName();
};

void Plan::step(){
    while (this->status==PlanStatus::AVALIABLE)
    {
        this->addFacility(new Facility(this->selectionPolicy->selectFacility(facilityOptions),this->settlement.getName()));

        if (underConstruction.size() == constructionLimit)
        {
            this->status =PlanStatus::BUSY;
        }
    }
    for (int i = 0; i < underConstruction.size(); i++) {
         if (underConstruction[i]->step() ==FacilityStatus::OPERATIONAL)
         {
             this->life_quality_score = this->life_quality_score + underConstruction[i]->getLifeQualityScore();
             this->economy_score = this->economy_score + underConstruction[i]->getEconomyScore();
             this->environment_score = this->environment_score + underConstruction[i]->getEnvironmentScore();
             facilities.push_back(underConstruction[i]); 
             underConstruction.erase(underConstruction.begin()+i);   
         }
    }
    if (underConstruction.size()<constructionLimit)
    {
        this->status = PlanStatus::AVALIABLE;
    }
    
};

void Plan::printStatus(){
    std:: cout <<"PlanID: " +std::to_string(this->plan_id)  << std::endl;
    std:: cout <<"SettlementName: " + this->settlement.getName()  << std::endl;
    std:: cout <<"PlanStatus: " + statusString(status)<< std::endl;
    std:: cout <<"SelectionPolicy: " +  this->selectionPolicy->toString()  << std::endl;
    std:: cout <<"LifeQualityScore " +std::to_string(this->life_quality_score)  << std::endl;
    std:: cout <<"EconomyScore " + std::to_string(this->economy_score)  << std::endl;
    std:: cout <<"EnviormentScore " + std::to_string(this->environment_score) << std::endl;
    for (Facility * f1 : this->facilities)
    {
        std:: cout <<"FacilityName: " + f1->getName() +"" << std::endl;
        std:: cout <<"FacilityStatus: OPERATIONAL"  << std::endl;
    }
    for (Facility  * f2 : this->underConstruction)
    {
        std:: cout <<"FacilityName: " + f2->getName() +"" << std::endl;
        std:: cout <<"FacilityStatus: UNDER_CONSTRUCTIONS"  << std::endl;
    }
    
};

string Plan::statusString(PlanStatus status) const{
      switch (status) {
        case PlanStatus::AVALIABLE:
            return "AVAILABLE";
        case PlanStatus::BUSY:
            return "BUSY";
      }
};

string Plan::getSelectionPolicy() const{
    return this->selectionPolicy->toString();
};

const vector<Facility*> & Plan::getFacilities() const{return this->facilities;};

void Plan::addFacility(Facility* facility){
    this->underConstruction.push_back(facility);
};

const string Plan::toString() const{
    return 
        "PlanID: " +std::to_string(this->plan_id) + "\n"
        "SettlementName: " + this->settlement.getName() + "\n" +
        "LifeQualityScore: " + std::to_string(this->life_quality_score) + "\n" +
        "EconomyScore: " + std::to_string(this->economy_score) + "\n" +
        "EnviormentScore: " + std::to_string(this->environment_score);
};