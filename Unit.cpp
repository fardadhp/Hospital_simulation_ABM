#include "Unit.h"
#include <iostream>

// (de)constructors
Unit::Unit(vector<string>& arg, unordered_map<string, double>& transmissionParams_) {
    ID = arg[0];
    name = arg[1];
    capacity = stoi(arg[2]);
    transmissionParams = transmissionParams_;
};

Unit::~Unit() {
    // cout << "Destructing ICU " << name << endl;
};

// setters
void Unit::setName(string& name_) {
    name = name_;
};

void Unit::setCapacity(int capacity_) {
    capacity = capacity_;
};

void Unit::setDailyColonizationCount(int c_) {
    dailyColonizationCount.push_back(c_);
};

void Unit::setDailyInfectionCount(int c_) {
    dailyInfectionCount.push_back(c_);
};

void Unit::setTransmissionParams(unordered_map<string, double>& transmissionParams_) {
    transmissionParams = transmissionParams_;
};


// getters
string Unit::getID() {
    return ID;
};

string Unit::getName() {
    return name;
}; 

int Unit::getCapacity() {
    return capacity;
};

int Unit::getDailyColonizationCount(int i_) {
    return dailyColonizationCount[i_];
};

int Unit::getDailyInfectionCount(int i_) {
    return dailyInfectionCount[i_];
};

unordered_map<string, double> Unit::getTransmissionParams() {
    return transmissionParams;
};

// Methods
void Unit::setup() {
    updateAdmissionDistribution();
    calculateUniformDistBoundaries(1);
};

void Unit::resetDailyColonizationCount() {
    dailyColonizationCount.clear();
};

void Unit::incrementDailyColonizationCount(int c_) {
    int inx = static_cast<int>(dailyColonizationCount.size());
    inx -= 1;
    dailyColonizationCount[inx] += c_;
};

void Unit::calculateUniformDistBoundaries(double ComplianceVariability) {
    transmissionParams["nurse_hygiene_compliance_enter_min"] = max(0.0, transmissionParams["nurse_hygiene_compliance_enter_mean"] - transmissionParams["nurse_hygiene_compliance_enter_half_interval"] * ComplianceVariability);
    transmissionParams["nurse_hygiene_compliance_enter_max"] = min(1.0, transmissionParams["nurse_hygiene_compliance_enter_mean"] + transmissionParams["nurse_hygiene_compliance_enter_half_interval"] * ComplianceVariability);
    transmissionParams["nurse_hygiene_compliance_exit_min"] = max(0.0, transmissionParams["nurse_hygiene_compliance_exit_mean"] - transmissionParams["nurse_hygiene_compliance_exit_half_interval"] * ComplianceVariability);
    transmissionParams["nurse_hygiene_compliance_exit_max"] = min(1.0, transmissionParams["nurse_hygiene_compliance_exit_mean"] + transmissionParams["nurse_hygiene_compliance_exit_half_interval"] * ComplianceVariability);
    transmissionParams["nurse_PPE_compliance_min"] = max(0.0, transmissionParams["nurse_PPE_compliance_mean"] - transmissionParams["nurse_PPE_compliance_half_interval"] * ComplianceVariability);
    transmissionParams["nurse_PPE_compliance_max"] = min(1.0, transmissionParams["nurse_PPE_compliance_mean"] + transmissionParams["nurse_PPE_compliance_half_interval"] * ComplianceVariability);      
};

void Unit::resetDailyInfectionCount() {
    dailyInfectionCount.clear();
};

void Unit::incrementDailyInfectionCount(int c_) {
    int inx = static_cast<int>(dailyInfectionCount.size());
    inx -= 1;
    dailyInfectionCount[inx] += c_;
};

void Unit::updateAdmissionDistribution() {
    double admission_C = transmissionParams["admission_prevalence"];
    double admission_I = 0;
    double admission_X = transmissionParams["highly_susceptible_ratio"] * (1 - admission_C - admission_I);
    double admission_S = 1 - admission_C - admission_I - admission_X;
    double summ = admission_S + admission_X + admission_C + admission_I;
    admission_S /= summ;
    admission_X /= summ;
    admission_C /= summ;
    transmissionParams["admission_S"] = admission_S;
    transmissionParams["admission_X"] = admission_X;
    transmissionParams["admission_C"] = admission_C;
    transmissionParams["admission_I"] = admission_I;
};

void Unit::replaceParameters(vector<vector<string>>& mc_params, vector<double>& rvs) {
    vector<string> transm_mc_params;
    int ub = static_cast<int>(mc_params.size());
    for (int i=0; i < ub; ++i) {
        mc_params[i].push_back(to_string(rvs[i]));
        if (mc_params[i][1] == "ICU")
            transmissionParams[mc_params[i][2]] = rvs[i]; 
        if (mc_params[i][2] == "HCW_hygiene_compliance_variability_from_mean")
            calculateUniformDistBoundaries(rvs[i]);
    }
    updateAdmissionDistribution();
};