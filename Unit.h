#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Unit{
    private:
    string ID;
    string name;
    int capacity;
    vector<int> dailyColonizationCount, dailyInfectionCount;
    unordered_map<string, double> transmissionParams;

    public:
    // (de)constructors
    Unit(vector<string>& arg, unordered_map<string, double>& transmissionParams_);
    ~Unit();
    
    // setters
    void setName(string& name_);
    void setCapacity(int capacity_);
    void setDailyColonizationCount(int c_);
    void setDailyInfectionCount(int c_);
    void setTransmissionParams(unordered_map<string, double>& transmissionParams_);
    
    // getters
    string getID();
    string getName(); 
    int getCapacity();
    int getDailyColonizationCount(int i_);
    int getDailyInfectionCount(int i_);
    unordered_map<string, double> getTransmissionParams();

    // Methods
    void setup();
    void resetDailyColonizationCount();
    void incrementDailyColonizationCount(int c_);
    void resetDailyInfectionCount();
    void incrementDailyInfectionCount(int c_);
    void calculateUniformDistBoundaries(double ComplianceVariability);
    void updateAdmissionDistribution();
    void replaceParameters(vector<vector<string>>& mc_params, vector<double>& rvs);
};

#endif