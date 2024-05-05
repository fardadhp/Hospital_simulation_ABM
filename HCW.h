#ifndef HCW_H
#define HCW_H

#include "Patient.h"
#include <string>
using namespace std;

class HCW {
    private:
    string ID;
    double contamination;
    double hygieneComplianceEnter;
    double hygieneComplianceExit;
    double PPECompliance;
    double residualContamination;
    double probabilityTransmission;
    bool PPE = false;
    bool PPEcontamination = false;

    public:
    // (de)constructors
    HCW(string& id, double contam, double CompEnter, double CompExit, double PPEComp, double residualContamination_, double probabilityTransmission_);
    ~HCW();
    
    //setters
    void setContamination(double contamination_);
    void setHygienecomplianceenter(double hygieneComplianceEnter_);
    void setHygienecomplianceexit(double hygieneComplianceExit_);
    void setPpecompliance(double PPECompliance_);
    void setResidualContamination(double residualContamination_);
    void setProbabilityTransmission(double probabilityTransmission_);
    void setPPE(bool PPE_);
    void setPPEContamination(bool PPEcontamination_);

    // getters
    double getContamination();
    double getHygienecomplianceenter();
    double getHygienecomplianceexit();
    double getPpecompliance();
    double getResidualContamination();
    double getProbabilityTransmission();
    bool getPPE();
    bool getPPEContamination();

    // Methods
    void washHands(double compliance);
    void wearPPE();
    void contactEnv(Patient* patient, bool env_to_hcw, bool hcw_to_env);
    string getHCWToEnvContactFreq();
    void hcwContaminationFromPatient();
    void transmissionFromHCWToPatient(Patient* patient);
    void removePPE();
    void interactWithPatient(Patient* patient, bool event_flags[], double p_hcw_to_patient);
};


#endif