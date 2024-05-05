#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "Unit.h"
#include "Patient.h"
#include "Room.h"
#include "HCW.h"

class Hospital {
    private:
    string ID, name, path, cwd;
    int nUnits, simLength, burnIn, dayCounter, surveillance_freq;        
    unordered_map<string, Unit*> units;
    unordered_map<string, HCW*> HCWs;
    unordered_map<string, Room*> rooms;
    unordered_map<string, Patient*> patients;
    vector<string> HCWIDs, patientIDs, unitNames, roomIDs;
    int cumNumPatients;
    unordered_map<string, double> transmissionParams;
    bool monteCarlo;
    string interventionType;
    unordered_map<string, int> T0, date, interventionTime; 
    double interventionEffect;
    vector<double> timeToInfectionData;
    double probablity_surveillance, probability_admission_testing;
    vector<unordered_map<string, string>> event_queue;

    public:
    // (de)constructors
    Hospital(string& ID_, string& name_, int nUnits_, int surveillance_freq_, double probability_admission_testing_, double probablity_surveillance_, int simLength_, int burnIn_, string& stringT0_);
    ~Hospital();
    
    // setters
    void setName(string& name_);
    void setPath(string& path_);
    void setCWD(string& cwd_) ;
    void setNUnits(int nUnits_) ;
    void setSimLength(int simLength_) ;
    void setBurnIn(int burnIn_);
    void setDayCounter(int dayCounter_);
    void setSurveillance_freq(int surveillance_freq_);
    void setHCW_list(vector<string>& HCWIDs_);
    void setpatientIDs(vector<string>& patientIDs_);
    void setCumNumPatients(int cumNumPatients_);
    void setTransmissionParams(unordered_map<string, double>& transmissionParams_);
    void setMonteCarlo(bool monteCarlo_);
    void setInterventionType(string& interventionType_);
    void setT0(unordered_map<string, int>& T0_);
    void setDate(unordered_map<string, int>& date_);
    void setInterventionTime(string& interventionTime_);
    void setInterventionEffect(double interventionEffect_);
    void setProbablity_surveillance(double probablity_surveillance_);
    void setProbability_admission_testing(double probability_admission_testing_);
    void setEvent_queue(vector<unordered_map<string, string>>& event_queue_);

    // getters
    string getID();
    string getName();
    string getPath(); 
    string getCwd();
    int getNUnits();
    int getSimLength();
    int getBurnIn();
    int getDayCounter();
    int getSurveillance_freq();
    unordered_map<string, Unit*> getUnits();
    unordered_map<string, HCW*> getHcws();
    unordered_map<string, Room*> getRooms();
    vector<string> getHCW_list();
    vector<string> getpatientIDs();
    unordered_map<string, Patient*> getPatients();
    int getCumNumPatients();
    unordered_map<string, double> getTransmissionParams();
    bool getMonteCarlo();
    string getInterventionType();
    unordered_map<string, int> getT0();
    unordered_map<string, int> getDate();
    unordered_map<string, int> getInterventionTime();
    double getInterventionEffect();
    vector<double> getTimetoInfectionData();
    double getProbablity_surveillance();
    double getProbability_admission_testing();
    vector<unordered_map<string, string>> getEvent_queue();

    // Methods
    void calculateUniformDistBoundaries(double complianceVariability);
    void updateAdmissionDistribution();
    Patient* findPatient(const string& patient_ID);
    HCW* findHCW(const string& HCW_ID);
    Unit* findUnit(const string& unit_name);
    Room* findRoom(const string& room_ID);
    string randomizeNewPatientDiseaseStatus();
    bool testPatient();
    bool evaluateContactPrecautionsNeed(Patient* patient);
    void assignPatientToRoom(Patient* patient, string& roomName);
    Patient* makeNewPatient(unordered_map<string, string>& adt_row);
    void dischargePatient(unordered_map<string, string>& event);
    void createUnits();
    void createOutputFolders();
    void setup();
    void activateIntervention(const string& intervention_name);
    void decolonization(vector<Patient*>& targetPatients);
    void surveillance();
    void endHCWShift(string& time, string& HCW_ID);
    void reset();
    void startDay();
    void simulateDay();
    void endDay();
    void simulate();
    void writeOutputs();
};

#endif