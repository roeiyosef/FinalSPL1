#include "SelectionPolicy.h"
#include <climits>
#include <iostream>
NaiveSelection:: NaiveSelection():lastSelectedIndex(-1){};
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex= (lastSelectedIndex+1)%facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
};

int NaiveSelection::getLast(){return lastSelectedIndex;};
const string NaiveSelection::toString() const{
    return "nve";
};

NaiveSelection * NaiveSelection ::clone() const{
    return new NaiveSelection(*this);
};


BalancedSelection:: BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
:LifeQualityScore(LifeQualityScore),EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){};

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int diff =INT_MAX;
    int index =0;
    for (int i = 0; i < facilitiesOptions.size(); i++)
    {    
       int newlifeQualityScore = this->LifeQualityScore + facilitiesOptions[i].getLifeQualityScore();
       int newEconomyScore = this->EconomyScore + facilitiesOptions[i].getEconomyScore();
       int newEnviormentScore = this->EnvironmentScore+facilitiesOptions[i].getEnvironmentScore(); 
       int maxScore = std::max(newlifeQualityScore,std::max(newEconomyScore,newEnviormentScore));
       int minScore = std::min(newlifeQualityScore,std::min(newEconomyScore,newEnviormentScore));
       int distance = maxScore - minScore;
       if (distance<diff)
       {
            diff = distance;
            index = i;
       } 
    }

    this->LifeQualityScore = this->LifeQualityScore + facilitiesOptions[index].getLifeQualityScore();
    this->EconomyScore = this->EconomyScore + facilitiesOptions[index].getEconomyScore();
    this->EnvironmentScore = this->EnvironmentScore + facilitiesOptions[index].getEnvironmentScore();
    return facilitiesOptions[index];
};
const string BalancedSelection::toString() const{
    return "bal";
};

BalancedSelection * BalancedSelection ::clone() const{
    return new BalancedSelection(*this);
};



EconomySelection:: EconomySelection():lastSelectedIndex(-1){};
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    bool foundEconomy= false;
    int index = lastSelectedIndex;
    if (facilitiesOptions.size()!=0)
    {
        index=(index+1)%facilitiesOptions.size();
    }
    else{
        index=(index+1);
    }
     
    
    while (!foundEconomy)
    {
        if (facilitiesOptions[index].getCategory()==FacilityCategory::ECONOMY)
        {
            lastSelectedIndex = index;
            foundEconomy = true;
        }
        else{
            index=(index+1)%facilitiesOptions.size();
        }
    }
    return facilitiesOptions[lastSelectedIndex];    
};

const string EconomySelection::toString() const{
    return "eco";
};
EconomySelection * EconomySelection ::clone() const{
    return new EconomySelection(*this);
};


SustainabilitySelection:: SustainabilitySelection():lastSelectedIndex(-1){};
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    bool foundEnviorment= false;
    int index = lastSelectedIndex;
    if (facilitiesOptions.size()!=0)
    {
        index=(index+1)%facilitiesOptions.size();
    }
    else{
        index=(index+1);
    }
    while (!foundEnviorment)
    {
        if (facilitiesOptions[index].getCategory()==FacilityCategory::ENVIRONMENT)
        {
            lastSelectedIndex = index;
            foundEnviorment = true;
        }
        else{
            index=(index+1)%facilitiesOptions.size();
        }
    }
    return facilitiesOptions[lastSelectedIndex];    
};

const string SustainabilitySelection::toString() const{
    return "env";
};
SustainabilitySelection * SustainabilitySelection ::clone() const{
    return new SustainabilitySelection(*this);
};
