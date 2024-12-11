#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);//constructor
        Simulation(const Simulation&other);//copyconstructor
        Simulation & operator=(const Simulation& other);//copy assignment operator
        ~Simulation();//destructor
       //NEED TO IMPLEMENT
        Simulation(Simulation&&other) noexcept;//move contructor
        Simulation & operator=(Simulation&& other) noexcept;//move assignment operator
       
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &FacilityName);
        bool isPlanExists(const int planID);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        void configureFile(const string &configFilePatht);
        void printActionLog() const;
    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs 
        vector<BaseAction*> actionsLog;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
        vector<Plan> plans;
        //string configFilePatht;
};