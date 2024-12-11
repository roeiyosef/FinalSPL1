#include "Simulation.h"
#include <iostream>
using std::vector;

using namespace std;

Simulation* backup = nullptr;

int main(int argc, char** argv){
   // cout << "moses" << endl;
    if(argc!=2){
       cout << "usage: simulation <config_path>" << endl;
       return 0;
   }
   string configurationFile = argv[1];
   Simulation simulation(configurationFile);
   simulation.start();
   if(backup!=nullptr){
  delete backup;
 	backup = nullptr;
 }
 
 return 0;
}