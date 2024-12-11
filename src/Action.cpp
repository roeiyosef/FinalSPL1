#include "Action.h"
#include <iostream>
extern Simulation* backup; 

//BASE ACTION
BaseAction:: BaseAction():errorMsg(""),status(ActionStatus::ERROR){};

void BaseAction::complete(){
    status = ActionStatus::COMPLETED; 
};
void BaseAction::error(string errorMsg){
     status = ActionStatus::ERROR; 
     this->errorMsg = errorMsg;
      std:: cout << "ERROR:<" + this->errorMsg+">" << std::endl;
};
const string &BaseAction::getErrorMsg() const{
    return this->errorMsg;
};
ActionStatus BaseAction::getStatus() const{return status;};

//SIMUALATE STEP
SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){};
void SimulateStep::act(Simulation &simulation){
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
};

const string SimulateStep::toString() const{
    if (this->getStatus()==ActionStatus::COMPLETED)
    {
        return "step "+std::to_string((this->numOfSteps))+ " "+"COMPLETED";
    }
    
    return "step "+std::to_string((this->numOfSteps))+ " "+"ERROR";
};
 SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
 };


//ADD PLAN 
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
:settlementName(settlementName),selectionPolicy(selectionPolicy){};

void AddPlan:: act(Simulation &simulation) {
    if (simulation.isSettlementExists(this->settlementName))
    {
        Settlement &set = simulation.getSettlement(this->settlementName);
        SelectionPolicy * sp= nullptr;
        if (selectionPolicy=="nve")
         {
            sp = new NaiveSelection();
         }
         if (selectionPolicy=="bal")
         {
            sp = new BalancedSelection(0,0,0);
         }
         if (selectionPolicy=="eco")
         {
            sp = new EconomySelection();
         }
         if (selectionPolicy=="env")
         {
            sp = new SustainabilitySelection();
         }
        
        if (sp!=nullptr)
        {
           simulation.addPlan(set,sp);
           complete();
        }
        else{
            error("Cannot create this plan 1");
        }    
    }
    else{
            error("Cannot create this plan 2");
        }  
    
};

const string AddPlan::toString() const{
    if (this->getStatus()==ActionStatus::COMPLETED)
    {
        return "plan "+settlementName+" "+selectionPolicy+ " "+"COMPLETED";
    }
    return "plan "+settlementName+" "+selectionPolicy+ " "+"ERROR";
    
};
AddPlan *AddPlan::clone() const {return new AddPlan(*this);};


//AddSettlement 
AddSettlement:: AddSettlement(const string &settlementName,SettlementType settlementType)
:settlementName(settlementName),settlementType(settlementType){};
void AddSettlement::act(Simulation &simulation){
    if (!simulation.isSettlementExists(settlementName))
    {
       simulation.addSettlement(new Settlement(this->settlementName,this->settlementType));
       complete();
    }
    else{
        error("“Settlement already exists");
    }
} ;
AddSettlement *AddSettlement::clone() const {return new AddSettlement(*this);};
const string AddSettlement::toString() const {
    if (this->getStatus()==ActionStatus::COMPLETED)
    {
        return "settlement "+this->settlementName+" "+std::to_string(static_cast<int>(this->settlementType))+ " "+"COMPLETED";
    }
   return "settlement "+this->settlementName+" "+std::to_string(static_cast<int>(this->settlementType))+ " "+"ERROR";
    
};



//AddFacility

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
:facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){};

void AddFacility::act(Simulation &simulation) {
     if (!simulation.isFacilityExists(facilityName))
    {
       simulation.addFacility(FacilityType(this->facilityName,this->facilityCategory,this->price,this->lifeQualityScore,this->economyScore,this->environmentScore));
       complete();
    }
    else{
        error("Facility already exists");
    }
};
AddFacility *AddFacility::clone() const {return new AddFacility(*this);};
const string AddFacility::toString() const{
            if (this->getStatus()==ActionStatus::COMPLETED)
             {
                return "facility "+this->facilityName+ " "+ std::to_string(static_cast<int>(this->facilityCategory))+" "+std::to_string(price)+" "+ std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+" "+"COMPLETED";
             }
            return "facility "+this->facilityName+ " "+ std::to_string(static_cast<int>(this->facilityCategory))+" "+std::to_string(price)+" "+ std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+" "+"ERROR";
} ;


//Print Plan Status
PrintPlanStatus:: PrintPlanStatus(int planId):planId(planId){};
void PrintPlanStatus::act(Simulation &simulation) {
    if (simulation.isPlanExists(this->planId))
    {
        Plan &p = (simulation.getPlan(planId));
        p.printStatus();
        complete();
    }
    else{
        error("Plan doesn't exist");
    }
};

PrintPlanStatus *PrintPlanStatus::clone() const{return new PrintPlanStatus(*this);};
const string PrintPlanStatus::toString() const{
    if (this->getStatus()==ActionStatus::COMPLETED)
             {
                return "planStatus "+std::to_string(this->planId)+" "+"COMPLETED";
             }
            return "planStatus "+std::to_string(this->planId)+" "+"ERROR";
} ;



//Change Policy
ChangePlanPolicy:: ChangePlanPolicy(const int planId, const string &newPolicy)
:planId(planId),newPolicy(newPolicy){};

void  ChangePlanPolicy::act(Simulation &simulation){
   if (simulation.isPlanExists(planId))
   {   
    if (simulation.getPlan(planId).getSelectionPolicy() == newPolicy)
    {
        error("Cannot change selection policy");
    }
    else{   
        if (newPolicy=="nve")
        {
            simulation.getPlan(planId).setSelectionPolicy(new NaiveSelection());
        }
        if (newPolicy=="bal")
        {
            Plan p  =  simulation.getPlan(planId);
            p.setSelectionPolicy(new BalancedSelection(p.getlifeQualityScore(),p.getEconomyScore(),p.getEnvironmentScore()));
        }
        if (newPolicy=="eco")
        {
            simulation.getPlan(planId).setSelectionPolicy(new EconomySelection());
        }
        if (newPolicy=="env")
        {
            simulation.getPlan(planId).setSelectionPolicy(new SustainabilitySelection());
        }
        complete();
        
    }
   }
   else{
        error("Cannot change selection policy");
   }
};
ChangePlanPolicy * ChangePlanPolicy::clone() const{return new ChangePlanPolicy(*this);};
const string  ChangePlanPolicy::toString() const{
     if (this->getStatus()==ActionStatus::COMPLETED)
             {
                return "changePolicy " + std::to_string(planId) + " " +newPolicy + " COMPLETED";
             }
    return "changePolicy " + std::to_string(planId) + " " +newPolicy + " ERROR";
};




//PrintActionsLog
 PrintActionsLog:: PrintActionsLog(){};
void PrintActionsLog::act(Simulation &simulation){
    simulation.printActionLog();
    complete();
};
PrintActionsLog * PrintActionsLog::clone() const{return new PrintActionsLog(*this);} ;
const string PrintActionsLog::toString() const{
    if (this->getStatus()==ActionStatus::COMPLETED){
                return "log COMPLETED";
    }
                return "log ERROR";
};


//CLOSE 
Close:: Close(){};
void Close::act(Simulation &simulation){
        complete();
        simulation.close();
       
} ;
Close * Close::clone() const{return new Close(*this);} ;
const string Close::toString() const {
                return "close COMPLETED";
};



//Backup 
BackupSimulation::BackupSimulation(){};
void BackupSimulation::act(Simulation &simulation){
   if (backup==nullptr)
   {
     backup = new Simulation(simulation);     
   }
   else
   {
     *backup = simulation;
   }
    
    complete();
} ;
BackupSimulation * BackupSimulation::clone() const {return new BackupSimulation(*this);};
const string BackupSimulation::toString() const{
    return "backup COMPLETED";
} ;



//Resotre
RestoreSimulation:: RestoreSimulation(){};
void RestoreSimulation::act(Simulation &simulation){
    if (backup==nullptr)
    {
       error("No backup available");
    }
    else{
        complete();
        simulation = *backup;
        simulation.open();
    }
    
};
RestoreSimulation *RestoreSimulation::clone() const{return new RestoreSimulation(*this);};
const string RestoreSimulation::toString() const{
        if (this->getStatus()==ActionStatus::COMPLETED){
                return "restore COMPLETED";
         }
                return "restore ERROR";
};