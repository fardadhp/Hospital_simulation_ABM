#include "Methods.h"
#include "Patient.h"
#include <iostream>

// (de)constructors
Patient::Patient(string& ID_, string& unitName_, string& admissionDate_, string& status_, string& dischargeDate_, bool CP_, unordered_map<string, double>& transmissionParams_) {
    ID = ID_;
    unitName = unitName_;
    admissionDate = readTimeString(admissionDate_);
    dischargeDate = readTimeString(dischargeDate_);
    status = status_;
    CP = CP_;
    active = true;
    transmissionParams = transmissionParams_;
    LOS = max(1, timeDiff(admissionDate, dischargeDate, "day"));
};

Patient::Patient() {};

Patient::~Patient() {
    // cout << "Destructing Patient " << ID << endl;
};

// setters
void Patient::setUnitName(const string& unitName_) {
    unitName = unitName_;
};
void Patient::setRoom(Room* room_) {
    room = room_;
};
void Patient::setAdmissionDate(string& date) {
    admissionDate = readTimeString(date);
};
void Patient::setDischargeDate(string& date) {
    dischargeDate = readTimeString(date);
};
void Patient::setLOS(int LOS_) {
    LOS = LOS_;
}
void Patient::setStatus(const string& st) {
    status = st;
};
void Patient::setCP(bool cp_) {
    CP = cp_;
};
void Patient::setActive(bool active_) {
    active = active_;
};
void Patient::setTransmParams(unordered_map<string, double>& params_) {
    transmissionParams = params_;
};

// getters
string Patient::getID() {
    return ID;
};
string Patient::getUnitName() {
    return unitName;
};
Room* Patient::getRoom() {
    return room;
};
unordered_map<string, int> Patient::getAdmissionDate() {
    return admissionDate;
};
unordered_map<string, int> Patient::getDischargeDate() {
    return dischargeDate;
};
int Patient::getLOS() {
    return LOS;
}
string Patient::getStatus() {
    return status;
};
bool Patient::getCP() {
    return CP;
};
bool Patient::getActive() {
    return active;
};
unordered_map<string, double> Patient::getTransmParams() {
    return transmissionParams;
};
// Methods
double Patient::sampleTimeToInfection(vector<double>& timeToInfectionData) {
    int ub = static_cast<int>(timeToInfectionData.size());
    vector<int> idx = generateRandomIntegers(0, ub, 1);
    double t = timeToInfectionData[idx[0]];
    return t;
};

void Patient::infectionTreatment() {
    CP = true;
};

void Patient::contactEnv() {
    // colonization
    if (((status == "S") | (status == "X")) & (room->getContamination() == true)) {
        double p = transmissionParams["probability_environmental_colonization"];
        if (status == "X") {
            p *= transmissionParams["increase_factor_for_highly_susceptible"];
        }
        double randomi = getRandomNumber();
        if (randomi < p) {
            status = "UC";
        }
    } else if (((status == "UC") | (status == "DC")) & (room->getContamination() == 0)) {
    // shedding
        double randomi = getRandomNumber();
        if (randomi < transmissionParams["probability_room_contamination_by_colonized_patient"]) {
            room->setContamination(true);
        }
    }
};

void Patient::colonizationToInfection(vector<double>& timeToInfectionData) {
    if ((status == "UC") | (status == "DC")) {
        double time_of_infection = sampleTimeToInfection(timeToInfectionData);
        if (LOS > time_of_infection) {
            status = "I";
            infectionTreatment();
        }
    }
};