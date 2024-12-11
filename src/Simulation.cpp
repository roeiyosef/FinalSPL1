#include "Simulation.h"
#include <iostream>
#include <fstream>
#include "Auxiliary.h"
#include "Action.h"
extern Simulation *backup;

//Constructor
Simulation::Simulation(const string &configFilePatht) : isRunning(true), planCounter(0),
plans(),actionsLog(),settlements(),facilitiesOptions()
{
    std::cout << "Simulation constructor called!" << std::endl;
    configureFile(configFilePatht);
    backup = nullptr;
};

//Copy Constructor
Simulation::Simulation(const Simulation &other) : isRunning(other.isRunning), planCounter(other.planCounter), facilitiesOptions(other.facilitiesOptions)
{
    for (int i = 0; i < other.actionsLog.size(); i++)
    {
        this->actionsLog.push_back(other.actionsLog[i]->clone());
    }
    for (int i = 0; i < other.settlements.size(); i++)
    {
        this->settlements.push_back(other.settlements[i]);
    }
    for (int i = 0; i < other.plans.size(); i++)
    {
        this->plans.push_back(Plan(other.plans.at(i)));
    }
};

//Copy Assignment Operator
Simulation &Simulation::operator=(const Simulation &other)
{
    if (&other != this)
    {
        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();
        for (int i = 0; i < other.actionsLog.size(); i++)
        {
           this->actionsLog.push_back(other.actionsLog[i]->clone());
        }
            
        if (this->settlements.size() > other.settlements.size())
        {
            for (int i = other.settlements.size(); i < this->settlements.size(); i++)
            {
                delete this->settlements[i];
            }
        }
        settlements.clear();
        for (int i = 0; i < other.settlements.size(); i++)
        {
            this->settlements.push_back(other.settlements[i]);
        }

        plans.clear();
        
        for (int i = 0; i < other.plans.size(); i++)
        {
            this->plans.push_back(Plan(other.plans.at(i)));
        }

        this->isRunning = other.isRunning;
        this->planCounter = other.planCounter;

        facilitiesOptions.clear();
        for (int i = 0; i < other.facilitiesOptions.size(); i++)
        {
            this->facilitiesOptions.push_back(FacilityType(other.facilitiesOptions[i]));
        }
    }
    return *this;
};

//Destructor
Simulation::~Simulation()
{
    for (BaseAction *action : actionsLog)
    {
        delete action;
    }

    plans.clear();
};

void Simulation::start()
{
    std::cout << "The simulation has started" << std::endl;
    open();
};


//Move Constructor
 Simulation:: Simulation(Simulation&&other) noexcept : isRunning(other.isRunning), planCounter(other.planCounter), facilitiesOptions(other.facilitiesOptions){
     for (int i = 0; i < other.actionsLog.size(); i++)
    {
        this->actionsLog.push_back(other.actionsLog[i]->clone());
        delete other.actionsLog[i];
        other.actionsLog[i] = nullptr;
    }
    other.actionsLog.clear();
    for (int i = 0; i < other.settlements.size(); i++)
    {
        this->settlements.push_back(other.settlements[i]);
        other.settlements[i]= nullptr;
    }
    other.settlements.clear();
    for (int i = 0; i < other.plans.size(); i++)
    {
        this->plans.push_back(Plan(other.plans.at(i)));
    }
    other.plans.clear();
    other.facilitiesOptions.clear();

 };
 
//Move Assignment Operator 
Simulation & Simulation::operator=(Simulation&& other) noexcept{
    if (&other != this)
    {
        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();
        for (int i = 0; i < other.actionsLog.size(); i++)
        {
                this->actionsLog.push_back(other.actionsLog[i]->clone());
                delete other.actionsLog[i];
                other.actionsLog[i] = nullptr;
        }
        other.actionsLog.clear();

        if (this->settlements.size() > other.settlements.size())
        {
            for (int i = other.settlements.size(); i < this->settlements.size(); i++)
            {
                delete this->settlements[i];
            }
        }
        settlements.clear();
        for (int i = 0; i < other.settlements.size(); i++)
        {
          this->settlements.push_back(other.settlements[i]);
          other.settlements[i]= nullptr;
        }
        other.settlements.clear();

        plans.clear();
        
        for (int i = 0; i < other.plans.size(); i++)
        {
            this->plans.push_back(Plan(other.plans.at(i)));
        }
        other.plans.clear();

        this->isRunning = other.isRunning;
        this->planCounter = other.planCounter;

        facilitiesOptions.clear();
        for (int i = 0; i < other.facilitiesOptions.size(); i++)
        {
            this->facilitiesOptions.push_back(FacilityType(other.facilitiesOptions[i]));
        }
        other.facilitiesOptions.clear();
    }
    return *this;
};
       
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    plans.push_back(Plan(planCounter, (settlement), selectionPolicy, facilitiesOptions));
    planCounter++;
};

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
    action->act(*this);
};

bool Simulation::addSettlement(Settlement *settlement)
{
    settlements.push_back(settlement);
};

bool Simulation::addFacility(FacilityType facility)
{
    facilitiesOptions.push_back(facility);
};
bool Simulation::isFacilityExists(const string &FacilityName)
{
    for (FacilityType f : facilitiesOptions)
    {
        if (f.getName() == FacilityName)
        {
            return true;
        }
    }
    return false;
};
bool Simulation::isSettlementExists(const string &settlementName)
{
    for (Settlement *st : settlements)
    {
        if (st->getName() == settlementName)
        {
            return true;
        }
    }
    return false;
};
bool Simulation::isPlanExists(const int planID)
{
    if (planID < 0 || planID >= planCounter)
    {
        return false;
    }
    return true;
};

Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (Settlement *st : settlements)
    {
        if (st->getName() == settlementName)
        {
            return (*st);
        }
    }
};

Plan &Simulation::getPlan(const int planID)
{
    return plans[planID];
};

void Simulation::step()
{
    for (int i = 0; i < plans.size(); i++)
    {
        plans[i].step();
    }
};

void Simulation::close()
{
    int index = 0;
    while (index < planCounter)
    {
        std::cout << (plans[index].toString()) << std::endl;
        index++;
    }
    isRunning = false;

    for (Settlement *set : settlements)
    {
        delete set;
    }
};

void Simulation::configureFile(const string &configFilePatht)
{
    std::ifstream file(configFilePatht);
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        else
        {
            
            vector<string> arguments = Auxiliary::parseArguments(line);
            if (arguments[0] == "settlement")
            {
                this->addSettlement(new Settlement(arguments[1], static_cast<SettlementType>(std::stoi(arguments[2]))));
            }
            if (arguments[0] == "facility")
            {
                this->addFacility(FacilityType(arguments[1], static_cast<FacilityCategory>(std::stoi(arguments[2])), (std::stoi(arguments[3])),
                                               (std::stoi(arguments[4])), (std::stoi(arguments[5])), (std::stoi(arguments[6]))));
            }
            if (arguments[0] == "plan")
            {
                if (isSettlementExists(arguments[1]))
                {
                    Settlement &settle = this->getSettlement(arguments[1]);

                    if (arguments[2] == "nve")
                    {
                        addPlan(settle, new NaiveSelection());
                    }
                    if (arguments[2] == "bal")
                    {
                        addPlan(settle, new BalancedSelection(0, 0, 0));
                    }
                    if (arguments[2] == "eco")
                    {
                        addPlan(settle, new EconomySelection());
                    }
                    if (arguments[2] == "env")
                    {
                        addPlan(settle, new SustainabilitySelection());
                    }
                }
            }
        }
    }
};

void Simulation::open()
{
    std::string userInput;

    BaseAction *a;
    while (isRunning)
    {
        std::getline(std::cin, userInput);
        vector<string> arguments = Auxiliary::parseArguments(userInput);
        if (arguments[0] == "step")
        {
            a = new SimulateStep(std::stoi(arguments[1]));
        }
        if (arguments[0] == "plan")
        {
            a = new AddPlan(arguments[1], arguments[2]);
        }
        if (arguments[0] == "settlement")
        {
            a = new AddSettlement(arguments[1], static_cast<SettlementType>(std::stoi(arguments[2])));
        }
        if (arguments[0] == "facility")
        {
            a = new AddFacility(arguments[1], static_cast<FacilityCategory>(std::stoi(arguments[2])), std::stoi(arguments[3]), std::stoi(arguments[4]), std::stoi(arguments[5]), std::stoi(arguments[6]));
        }
        if (arguments[0] == "planStatus")
        {
            a = new PrintPlanStatus(std::stoi(arguments[1]));
        }
        if (arguments[0] == "changePolicy")
        {
            a = new ChangePlanPolicy(std::stoi(arguments[1]), arguments[2]);
        }
        if (arguments[0] == "log")
        {
            a = new PrintActionsLog();
        }
        if (arguments[0] == "close")
        {
            a = new Close();
        }
        if (arguments[0] == "backup")
        {
            a = new BackupSimulation();
        }
        if (arguments[0] == "restore")
        {
            a = new RestoreSimulation();
        }
        this->addAction(a);
        }
};

void Simulation::printActionLog() const
{
    for (int i = 0; i < actionsLog.size() - 1; i++)
    {
        std::cout << actionsLog[i]->toString() << std::endl;
    }
};
