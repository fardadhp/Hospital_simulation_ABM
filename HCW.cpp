#include "HCW.h"
#include "Methods.h"

// (de)constructors
HCW::HCW(string& id, double contam, double CompEnter, double CompExit, double PPEComp, double residualContamination_, double probabilityTransmission_) {
    ID = id;
    contamination = contam;
    hygieneComplianceEnter = CompEnter;
    hygieneComplianceExit = CompExit;
    PPECompliance = PPEComp;
    residualContamination = residualContamination_;
    probabilityTransmission = probabilityTransmission_;
};

HCW::~HCW() {
    cout << "Destructing HCW " << ID << endl;
};

//setters
void HCW::setContamination(double contamination_) {
    contamination = contamination_;
};
void HCW::setHygienecomplianceenter(double hygieneComplianceEnter_) {
    hygieneComplianceEnter = hygieneComplianceEnter_;
};
void HCW::setHygienecomplianceexit(double hygieneComplianceExit_) {
    hygieneComplianceExit = hygieneComplianceExit_;
};
void HCW::setPpecompliance(double PPECompliance_) {
    PPECompliance = PPECompliance_;
};
void HCW::setResidualContamination(double residualContamination_) {
    residualContamination = residualContamination_;
};
void HCW::setProbabilityTransmission(double probabilityTransmission_) {
    probabilityTransmission = probabilityTransmission_;
};
void HCW::setPPE(bool PPE_) {
    PPE = PPE_;
};
void HCW::setPPEContamination(bool PPEcontamination_) {
    PPEcontamination = PPEcontamination_;
};

// getters
double HCW::getContamination() {
    return contamination;
};
double HCW::getHygienecomplianceenter() {
    return hygieneComplianceEnter;
};
double HCW::getHygienecomplianceexit() {
    return hygieneComplianceExit;
};
double HCW::getPpecompliance() {
    return PPECompliance;
};
double HCW::getResidualContamination() {
    return residualContamination;
};
double HCW::getProbabilityTransmission() {
    return probabilityTransmission;
};
bool HCW::getPPE() {
    return PPE;
};
bool HCW::getPPEContamination() {
    return PPEcontamination;
};

// Methods
void HCW::washHands(double compliance) {
    // HCW hand hygiene
    double randomi = getRandomNumber();
    if ((contamination > 0.999) & (randomi < compliance)) {
        contamination = residualContamination;
    }
};

void HCW::wearPPE() {
    double randomi = getRandomNumber();
    if (randomi < PPECompliance) {
        PPE = true;
        PPEcontamination = false;
    }
};

void HCW::contactEnv(Patient* patient, bool env_to_hcw, bool hcw_to_env) {
    // hcw contamination
    if ((patient->getRoom()->getContamination() == 1) & env_to_hcw) {
        if (!PPE)
            contamination = 1;
        else
            PPEcontamination = 1;
    } 
    else { // shedding
        if ((((!PPE) & (contamination == 1)) | ((PPE) & (PPEcontamination == 1))) & hcw_to_env)
            patient->getRoom()->setContamination(true);
    }
};

string HCW::getHCWToEnvContactFreq() {
    string options[] = {"before & after", "just before", "just after", "neither"};
    double probs[] = {0.25, 0.25, 0.25, 0.25};
    return options[generateMultinomialSample(probs, 1)[0]]; 
}

void HCW::hcwContaminationFromPatient() {
    if (!PPE)
        contamination = 1;
    else
        PPEcontamination = 1;
}

void HCW::transmissionFromHCWToPatient(Patient* patient) {
    patient->setStatus("UC");
    // cout << "transmission event for " + patient->getID() << endl;
}

void HCW::removePPE() {
    PPE = false;
    PPEcontamination = false;
}

void HCW::interactWithPatient(Patient* patient, bool event_flags[], double p_hcw_to_patient) {
    bool patient_to_hcw = event_flags[0];
    bool env_to_hcw = event_flags[2];
    bool hcw_to_env = event_flags[3];
    if (patient->getCP())
        wearPPE();
    else
        washHands(hygieneComplianceEnter);
    // how many times HCW contact env
    string hcw_env_cont_times = getHCWToEnvContactFreq();
    
    // env <-> hcw at entry
    if ((hcw_env_cont_times.find("before") != string::npos) & (env_to_hcw | hcw_to_env))
        contactEnv(patient, env_to_hcw, hcw_to_env);
    // patient to HCW
    unordered_set<string> risk = {"DC", "UC", "I"};
    if (risk.count(patient->getStatus()) > 0 && patient_to_hcw)
        hcwContaminationFromPatient();
    else {
        // HCW to patient
        double contam = contamination;
        if (PPE)
            contam = PPEcontamination;
        double p = probabilityTransmission * contam;
        bool hcw_to_patient = (p_hcw_to_patient < p);
        risk = {"S", "X"};
        // cout << "checking transmission event: " + to_string(p_hcw_to_patient) + " <? " + to_string(probabilityTransmission) + " * " + to_string(contam) << endl;
        if (hcw_to_patient && risk.count(patient->getStatus()) > 0) {
            transmissionFromHCWToPatient(patient); 
        }
    };
            
    // env <-> hcw at exit
    if ((hcw_env_cont_times.find("after") != string::npos) & (env_to_hcw | hcw_to_env))
        contactEnv(patient, env_to_hcw, hcw_to_env);
    // remove PPE
    if (PPE)
        removePPE();
    else
        washHands(hygieneComplianceExit);
};