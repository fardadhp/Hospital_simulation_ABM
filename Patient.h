#ifndef PATIENT_H
#define PATIENT_H

#include "Room.h"
#include <string>
#include <unordered_map>
#include <vector>

class Patient {
    private:
    string ID;
    string unitName;
    unordered_map<string, int> admissionDate, dischargeDate;
    int LOS;
    string status;
    bool CP;
    Room* room;
    bool active;
    unordered_map<string, double> transmissionParams;

    public:
    // (de)constructors
    Patient();
    Patient(string& ID_, string& unitName_, string& admissionDate_, string& status_, string& dischargeDate_, bool CP_, unordered_map<string, double>& transmissionParams_);
    ~Patient();
    
    // setters
    void setUnitName(const string& unitName_);
    void setRoom(Room* room_);
    void setAdmissionDate(string& date);
    void setDischargeDate(string& date);
    void setLOS(int LOS_);
    void setStatus(const string& st);
    void setCP(bool cp_);
    void setActive(bool active_);
    void setTransmParams(unordered_map<string, double>& params_);

    // getters
    string getID();
    string getUnitName();
    Room* getRoom();
    unordered_map<string, int> getAdmissionDate();
    unordered_map<string, int> getDischargeDate();
    int getLOS();
    string getStatus();
    bool getCP();
    bool getActive();
    unordered_map<string, double> getTransmParams();
    // Methods
    double sampleTimeToInfection(vector<double>& timeToInfectionData);
    void infectionTreatment();
    void contactEnv();
    void colonizationToInfection(vector<double>& timeToInfectionData);        
};



#endif