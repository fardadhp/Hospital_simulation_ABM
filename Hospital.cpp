#include "Hospital.h"
#include "Methods.h"

// (de)constructors
Hospital::Hospital(string& ID_, string& name_, int nUnits_, int surveillance_freq_, double probability_admission_testing_, double probablity_surveillance_, int simLength_, int burnIn_, string& stringT0_) {
    ID = ID_;
    name = name_;
    nUnits = nUnits_;
    surveillance_freq = surveillance_freq_;
    probability_admission_testing = probability_admission_testing_;
    probablity_surveillance = probablity_surveillance_;
    simLength = simLength_;
    burnIn = burnIn_;
    T0 = readTimeString(stringT0_);
    date = T0;
    dayCounter = 0;
    cumNumPatients = 0;
    monteCarlo = false;
    interventionType = "N/A";
    interventionEffect = 0;
};
Hospital::~Hospital() {
    for (string& pid : patientIDs)
        delete patients[pid];
    for (string& hid : HCWIDs)
        delete HCWs[hid];
    for (string& rid : roomIDs)
        delete rooms[rid];
    for (string& uid : unitNames)
        delete units[uid];
    // cout << "Destructing Hospital " << name << endl;
}; 

// setters
void Hospital::setName(string& name_) {
    name = name_;
};

void Hospital::setPath(string& path_) {
    path = path_;
};

void Hospital::setCWD(string& cwd_) {
    cwd = cwd_;
};

void Hospital::setNUnits(int nUnits_) {
    nUnits = nUnits_;
};

void Hospital::setSimLength(int simLength_) {
    simLength = simLength_;
};

void Hospital::setBurnIn(int burnIn_) {
    burnIn = burnIn_;
};

void Hospital::setDayCounter(int dayCounter_) {
    dayCounter = dayCounter_;
};

void Hospital::setSurveillance_freq(int surveillance_freq_) {
    surveillance_freq = surveillance_freq_;
};

void Hospital::setHCW_list(vector<string>& HCWIDs_) {
    HCWIDs = HCWIDs_;
};

void Hospital::setpatientIDs(vector<string>& patientIDs_) {
    patientIDs = patientIDs_;
};

void Hospital::setCumNumPatients(int cumNumPatients_) {
    cumNumPatients = cumNumPatients_;
};

void Hospital::setTransmissionParams(unordered_map<string, double>& transmissionParams_) {
    transmissionParams = transmissionParams_;
};

void Hospital::setMonteCarlo(bool monteCarlo_) {
    monteCarlo = monteCarlo_;
};

void Hospital::setInterventionType(string& interventionType_) {
    interventionType = interventionType_;
};

void Hospital::setT0(unordered_map<string, int>& T0_) {
    T0 = T0_;
};

void Hospital::setDate(unordered_map<string, int>& date_) {
    date = date_;
};
void Hospital::setInterventionTime(string& interventionTime_) {
    interventionTime = readTimeString(interventionTime_);
};

void Hospital::setInterventionEffect(double interventionEffect_) {
    interventionEffect = interventionEffect_;
};

void Hospital::setProbablity_surveillance(double probablity_surveillance_) {
    probablity_surveillance = probablity_surveillance_;
};

void Hospital::setProbability_admission_testing(double probability_admission_testing_) {
    probability_admission_testing = probability_admission_testing_;
};

void Hospital::setEvent_queue(vector<unordered_map<string, string>>& event_queue_) {
    event_queue = event_queue_;
};

// getters
string Hospital::getID() {
    return ID;
};

string Hospital::getName() {
    return name;
};

string Hospital::getPath() {
    return path;
}; 

string Hospital::getCwd() {
    return cwd;
};

int Hospital::getNUnits() {
    return nUnits;
};

int Hospital::getSimLength() {
    return simLength;
};

int Hospital::getBurnIn() {
    return burnIn;
};

int Hospital::getDayCounter() {
    return dayCounter;
};

int Hospital::getSurveillance_freq() {
    return surveillance_freq;
};

unordered_map<string, Unit*> Hospital::getUnits() {
    return units;
};

unordered_map<string, HCW*> Hospital::getHcws() {
    return HCWs;
};

unordered_map<string, Room*> Hospital::getRooms() {
    return rooms;
};

vector<string> Hospital::getHCW_list() {
    return HCWIDs;
};

vector<string> Hospital::getpatientIDs() {
    return patientIDs;
};

unordered_map<string, Patient*> Hospital::getPatients() {
    return patients;
};

int Hospital::getCumNumPatients() {
    return cumNumPatients;
};

unordered_map<string, double> Hospital::getTransmissionParams() {
    return transmissionParams;
};

bool Hospital::getMonteCarlo() {
    return monteCarlo;
};

string Hospital::getInterventionType() {
    return interventionType;
};

unordered_map<string, int> Hospital::getT0() {
    return T0;
};

unordered_map<string, int> Hospital::getDate() {
    return date;
};

unordered_map<string, int> Hospital::getInterventionTime() {
    return interventionTime;
};

double Hospital::getInterventionEffect() {
    return interventionEffect;
};

vector<double> Hospital::getTimetoInfectionData() {
    return timeToInfectionData;
};

double Hospital::getProbablity_surveillance() {
    return probablity_surveillance;
};

double Hospital::getProbability_admission_testing() {
    return probability_admission_testing;
};

vector<unordered_map<string, string>> Hospital::getEvent_queue() {
    return event_queue;
};

// Methods
void Hospital::calculateUniformDistBoundaries(double complianceVariability) {
    transmissionParams["nurse_hygiene_compliance_enter_min"] = max(0.0, transmissionParams["nurse_hygiene_compliance_enter_mean"] - transmissionParams["nurse_hygiene_compliance_enter_half_interval"] * complianceVariability);
    transmissionParams["nurse_hygiene_compliance_enter_max"] = min(1.0, transmissionParams["nurse_hygiene_compliance_enter_mean"] + transmissionParams["nurse_hygiene_compliance_enter_half_interval"] * complianceVariability);
    transmissionParams["nurse_hygiene_compliance_exit_min"] = max(0.0, transmissionParams["nurse_hygiene_compliance_exit_mean"] - transmissionParams["nurse_hygiene_compliance_exit_half_interval"] * complianceVariability);
    transmissionParams["nurse_hygiene_compliance_exit_max"] = min(1.0, transmissionParams["nurse_hygiene_compliance_exit_mean"] + transmissionParams["nurse_hygiene_compliance_exit_half_interval"] * complianceVariability);
    transmissionParams["nurse_PPE_compliance_min"] = max(0.0, transmissionParams["nurse_PPE_compliance_mean"] - transmissionParams["nurse_PPE_compliance_half_interval"] * complianceVariability);
    transmissionParams["nurse_PPE_compliance_max"] = min(1.0, transmissionParams["nurse_PPE_compliance_mean"] + transmissionParams["nurse_PPE_compliance_half_interval"] * complianceVariability);      
};

void Hospital::updateAdmissionDistribution() {
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

Patient* Hospital::findPatient(const string& patient_ID) {
    if (patients.count(patient_ID))
        return patients[patient_ID];
    else
        return nullptr;
};

HCW* Hospital::findHCW(const string& HCW_ID) {
    if (HCWs.count(HCW_ID))
        return HCWs[HCW_ID];
    else
        return nullptr;
};

Unit* Hospital::findUnit(const string& unit_name) {
    if (units.count(unit_name))
        return units[unit_name];
    else
        return nullptr;
};

Room* Hospital::findRoom(const string& room_ID) {
    if (rooms.count(room_ID))
        return rooms[room_ID];
    else
        return nullptr;
};

string Hospital::randomizeNewPatientDiseaseStatus() {
    string options[] = {"S", "X", "UC", "I"}; 
    double p[4] = {transmissionParams["admission_S"], transmissionParams["admission_X"], transmissionParams["admission_C"], transmissionParams["admission_I"]};
    vector<int> trials = generateMultinomialSample(p, 1);
    return options[trials[0]];
};

bool Hospital::testPatient() {
    double randomi = ((double) rand() / (RAND_MAX));
    if (randomi < probability_admission_testing * transmissionParams["test_sensitivity"])
        return 1;
    return 0;
};

bool Hospital::evaluateContactPrecautionsNeed(Patient* patient) {
    string status_ = patient->getStatus();
    bool CP = false;
    if (status_ == "I")
        CP = true;
    else if (status_ == "UC") {
        if (testPatient()) {
            patient->setStatus("DC");
            CP = true;
        } 
    }

    if (interventionType == "universal_contact_precautions") {
        unordered_map<string, int> admissionDate = patient->getAdmissionDate();
        if (timeDiff(interventionTime, admissionDate, "minute") >= 0) {
            CP = true;
        }
    }

    return CP;
};

void Hospital::assignPatientToRoom(Patient* patient, string& roomName) {
    Room* myRoom = new Room();
    if (roomName == "NA")
        roomName = to_string(generateRandomIntegers(0,1000,1)[0]);
    if (!rooms.count(roomName)) {
        myRoom = new Room(roomName, false, transmissionParams["terminal_room_disinfection_efficacy"], transmissionParams["pathogen_natural_clearance_from_dry_surfaces"]); 
        rooms[roomName] = myRoom;
        roomIDs.push_back(roomName);
    }
    else
        myRoom = findRoom(roomName);
    patient->setRoom(myRoom);
};

Patient* Hospital::makeNewPatient(unordered_map<string, string>& adt_row) {
    bool check = find(begin(patientIDs), end(patientIDs), adt_row["MaskedMRN"]) != end(patientIDs);
    string admission_type = "";
    if (check)
        admission_type = "readmission";
    else
        admission_type = "admission";
    // patient"s disease status
    string status_ = randomizeNewPatientDiseaseStatus();
    bool CP = false;
    if (admission_type == "admission") {
        // cout << "Line 756: " + adt_row["in_time"] + " or " + adt_row["out_time"] << endl;
        string _in_time = adt_row["in_time"];
        Patient* newpatient = new Patient(adt_row["MaskedMRN"], adt_row["department_name_new"], adt_row["in_time"], status_, adt_row["out_time"], CP, transmissionParams);
        if (patients.count(adt_row["MaskedMRN"])) {
            cout << "Can't create new Patient. Patient already exists." << endl;
            exit(-1);
        }
        patients[adt_row["MaskedMRN"]] = newpatient;
        patientIDs.push_back(adt_row["MaskedMRN"]);
        cumNumPatients++;
    }
    Patient* newpatient = findPatient(adt_row["MaskedMRN"]);
    newpatient->setAdmissionDate(adt_row["in_time"]);
    newpatient->setStatus(status_);
    newpatient->setDischargeDate(adt_row["out_time"]);
    unordered_map<string, int> admissionDate = newpatient->getAdmissionDate();
    unordered_map<string, int> dischargeDate = newpatient->getDischargeDate();
    newpatient->setLOS(timeDiff(admissionDate, dischargeDate, "day"));
    // newpatient->setCP(CP);
    newpatient->setCP(evaluateContactPrecautionsNeed(newpatient));
    newpatient->setActive(true);
    assignPatientToRoom(newpatient, adt_row["adt_room"]);
    

    return newpatient;
};

void Hospital::dischargePatient(unordered_map<string, string>& event) {
    if (event["event_type"] == "discharge") {
        patientIDs.erase(remove(patientIDs.begin(), patientIDs.end(), event["MaskedMRN"]), patientIDs.end());
        patients.erase(event["MaskedMRN"]);
    } 
    else {
        Patient* thisPatient = findPatient(event["MaskedMRN"]);
        thisPatient->getRoom()->disinfect();
        thisPatient->setActive(false);
        thisPatient->setStatus("NA");
    }
};

void Hospital::createUnits() {
    vector<vector<string>> unitsData = readCSVFileMultipleColumns(cwd + "/data/units_parameters.csv");
    for (int i = 0; i < nUnits; ++i) {
        Unit* unit = new Unit(unitsData[i], transmissionParams);
        units[unitsData[i][1]] = unit;
        unitNames.push_back(unitsData[i][1]);
    }   
};

void Hospital::createOutputFolders() {
    if (path.length() == 0) {
        time_t t = time(0);   // get time now
        tm* now = localtime(&t);
        path = cwd + "/output/" + to_string(now->tm_year + 1900) + "_" + to_string(now->tm_mon + 1) + "_" + to_string(now->tm_mday) + "_" + to_string(now->tm_hour) + "_" + to_string(now->tm_min);
    };
    vector<string> paths = {(cwd + "/output"), path};
    for (int i = 0; i < 2; ++i) {
        const filesystem::path folder{paths[i]};
        if (!filesystem::exists(folder)) {
            cout << "Creating folder: " << folder << endl;
            filesystem::create_directory(folder);
        }
    }
};

void Hospital::setup() {
    // transmission parameters
    transmissionParams = constructTransmissionParamsMap(cwd + "/data/transmission_parameters.csv");
    // time to infection
    timeToInfectionData = readCSVFileSingleColumns(cwd + "/data/time_to_infection.csv");
    updateAdmissionDistribution();
    calculateUniformDistBoundaries(1);
    // create units
    createUnits();       
    // set intervention(s)
    activateIntervention("N/A");
    // create output folders
    createOutputFolders();
};

void Hospital::activateIntervention(const string& intervention_name) {
    vector<vector<string>> interventions = readCSVFileMultipleColumns(cwd + "/data/intervention_parameters.csv");
    vector<string> intervention;
    bool interventionFlag = false;
    if (intervention_name != "N/A") {
        for (auto& interv : interventions) {
            if (interv[0] == intervention_name) {
                intervention = interv;
                interventionFlag = true;
                break;
            }
        }
    } 
    else {
        for (auto interv : interventions) {
            if (interv[4] == "1") {
                intervention = interv;
                interventionFlag = true;
                break;
            }
        }
    }

    if (interventionFlag) {
        if (intervention.size() > 0) {
            interventionType = intervention[0];
            string _interventionTime = intervention[3];
            interventionEffect = stod(intervention[2]);
            if (_interventionTime.length() > 3)
                interventionTime = readTimeString(_interventionTime);
            else
                interventionTime = addDays(T0, stoi(_interventionTime));
        }
    }
};

void Hospital::decolonization(vector<Patient*>& targetPatients) {
    for (auto& patient : targetPatients) {
        double randomi = getRandomNumber();
        if (randomi < interventionEffect) {
            patient->setStatus("S");
            patient->setCP(false);
        }
    }
};

void Hospital::surveillance() {
    bool surveillance_check = ((surveillance_freq > 0) && (timeDiff(T0, date, "day") % surveillance_freq < 1));
    double randomi = getRandomNumber();
    bool surveillance_compliance = (randomi < probablity_surveillance * transmissionParams["test_sensitivity"]);
    if (surveillance_check && surveillance_compliance) {
        for (string& p : patientIDs) {
            if ((patients[p]->getActive()) && (patients[p]->getStatus() == "UC")) {
                patients[p]->setStatus("DC");
                patients[p]->setCP(true);
            }
        }
    }
};

void Hospital::endHCWShift(string& time, string& HCW_ID) {
    if (time == "YES") {
        HCWIDs.erase(remove(HCWIDs.begin(), HCWIDs.end(), HCW_ID), HCWIDs.end());
        HCWs.erase(HCW_ID);
    }
};

void Hospital::reset() {
    HCWs.clear();
    HCWIDs.clear();
    patients.clear();
    patientIDs.clear();
    cumNumPatients = 0;
    date = T0;
    dayCounter = 0;
    for (string& u : unitNames) {
        units[u]->resetDailyColonizationCount();
        units[u]->resetDailyInfectionCount();
    };
};

void Hospital::startDay() { 
    // event queue
    event_queue.clear();
    cout << "***************** day " << dayCounter << endl;
    // cout << "reading event queue" << endl;
    event_queue = readEventQueueAsMap(cwd + "/data/event_queue_splitted/event_queue_all_2017_2018_cpp_day_" + to_string(dayCounter) + ".csv");      
    // if (dayCounter > 19) {
    // cout << "# events = " << event_queue.size() << endl;
    // }
    for (string& u : unitNames) {
        units[u]->setDailyColonizationCount(0);
        units[u]->setDailyInfectionCount(0);
    }
    // reset patients" daily counters + daily prob of developing an infection
    for (string& pid : patientIDs) {
        if (patients[pid]->getActive()) {
            string before = patients[pid]->getStatus();
            patients[pid]->colonizationToInfection(timeToInfectionData);
            string after = patients[pid]->getStatus();
            if ((before != "I") && (after == "I")) {
                findUnit(patients[pid]->getUnitName())->incrementDailyInfectionCount(1);
            }
        } 
    };
    // natural clearance
    for (string& rid : roomIDs) {
        rooms[rid]->naturalClearance();
    };
    // interventions
    if ((interventionType.find("target_decolonization") != string::npos)) {
        vector<Patient*> colonized_patients;
        for (string& pid : patientIDs) {
            if (patients[pid]->getStatus() == "DC") {
                colonized_patients.push_back(patients[pid]);
            }
        }
        decolonization(colonized_patients);
    }
};


void Hospital::simulateDay() {
    // if (dayCounter > 19) {
    // cout << "starting simulationg day.." << endl;
    // }
    // iterate over events
    int eventCounter = 0;
    for (auto& event : event_queue) {
        // if (dayCounter > 19) {
        // cout << "event " << eventCounter << ": " << event["event_type"] << " * ";
        // }
        eventCounter++;
        Unit* unit = findUnit(event["department_name_new"]);
        if (event["event_type"] == "admission") {
            makeNewPatient(event); 
        } 
        else {
            if ((event["event_type"] == "discharge") || (event["event_type"] == "transfer")) {
                dischargePatient(event);
            } else {
                // contact
                string params[4] = {"probability_HCW_contamination_from_colonized_patient",
                                    "probability_transmission_from_contaminated_hcw_to_susceptible_patient",
                                    "probability_hcw_contamination_from_contaminated_env",
                                    "probability_env_contamination_from_contaminated_hcw"};
                double event_probabilities[4] = {0.0, 0.0, 0.0, 0.0};
                for (int i = 0; i < 4; ++i)
                    event_probabilities[i] = unit->getTransmissionParams()[params[i]];
                double probs[4] = {getRandomNumber(), getRandomNumber(), getRandomNumber(), getRandomNumber()};
                event_probabilities[0] *= unit->getTransmissionParams()["shedding_increase_factor_for_infected"];
                event_probabilities[1] *= unit->getTransmissionParams()["increase_factor_for_highly_susceptible"];
                bool event_flags[4] = {false, false, false, false};
                for (int i = 0; i < 4; ++i) {
                    if (probs[i] < event_probabilities[i])
                        event_flags[i] = true;
                }
                if (event_flags[0] || event_flags[1] || event_flags[2] || event_flags[3]) {
                    // find patient
                    Patient* patient = findPatient(event["MaskedMRN"]);
                    if (patient == nullptr) // when patient's admission record not in the data
                        patient = makeNewPatient(event);
                    // find HCW
                    HCW* hcw = findHCW(event["HCW_ID"]);
                    if (hcw == nullptr) {
                        double nurse_hygiene_compliance_enter = getRandomUniform(unit->getTransmissionParams()["nurse_hygiene_compliance_enter_min"], transmissionParams["nurse_hygiene_compliance_enter_max"]);
                        double nurse_hygiene_compliance_exit = getRandomUniform(unit->getTransmissionParams()["nurse_hygiene_compliance_exit_min"], transmissionParams["nurse_hygiene_compliance_exit_max"]);
                        double nurse_PPE_compliance = getRandomUniform(unit->getTransmissionParams()["nurse_PPE_compliance_min"], transmissionParams["nurse_PPE_compliance_max"]);
                        hcw = new HCW(event["HCW_ID"], 1.0, nurse_hygiene_compliance_enter, nurse_hygiene_compliance_exit, nurse_PPE_compliance, unit->getTransmissionParams()["residual_contamination_post_hand_washing"], unit->getTransmissionParams()["probability_transmission_from_contaminated_hcw_to_susceptible_patient"]);
                        HCWs[event["HCW_ID"]] = hcw;
                        HCWIDs.push_back(event["HCW_ID"]);
                    };
                    // HCW-patient interaction
                    if (patient->getActive()) {
                        string before = patient->getStatus();
                        hcw->interactWithPatient(patient, event_flags, probs[1]);
                        string after = patient->getStatus();
                        unordered_set<string> lst = {"UC","DC","I"};
                        bool check1 = (lst.count(before) > 0);
                        bool check2 = (lst.count(after) > 0);
                        if (!check1 && check2)
                            unit->incrementDailyColonizationCount(1);
                        /* HCW transmission (prob[1]) has to be re-evaluated due to the residual contamination probability and compliance probability
                        which vary by HCW (as we don"t want to retireve the HCW agent to save time) */
                    }
                };
                endHCWShift(event["out_time"], event["HCW_ID"]);
            };  
        }
    };
    // daily events 
    surveillance();
    for (string& pid : patientIDs) {
        if (patients[pid]->getActive()) {
            Unit* unit = findUnit(patients[pid]->getUnitName());
            string before = patients[pid]->getStatus();
            patients[pid]->contactEnv(); 
            string after = patients[pid]->getStatus();
            unordered_set<string> lst = {"UC","DC","I"};
            bool check1 = (lst.count(before) > 0);
            bool check2 = (lst.count(after) > 0);
            if (!check1 && check2)
                unit->incrementDailyColonizationCount(1);
        }
    } 
};

void Hospital::endDay() {
    // cout << "ending simulationg day.." << endl;
    date = addDays(date, 1);
    dayCounter++;
    // cout << "count patients: " << cumNumPatients << endl;
    // cout << "count patients: " << patients.size() << endl;
    // cout << "count HCWs: " << HCWs.size() << endl;
};

void Hospital::simulate() {
    int totalSimLength = simLength + burnIn;
    for (int i = 0; i < totalSimLength; ++i) {
        startDay();
        simulateDay();
        endDay();
    }
    writeOutputs();
};

void Hospital::writeOutputs() {
    for (string& uid : unitNames) {
        vector<vector<string>> data = {{"ICU", "Colonization", "infection"}};
        for (int i = burnIn; i < simLength; ++i)
            data.push_back({units[uid]->getName(), to_string(units[uid]->getDailyColonizationCount(i)), to_string(units[uid]->getDailyInfectionCount(i))});
        string pathname = path + "/" + units[uid]->getID() + "_data.csv";
        writeToCSV(pathname, data);
        data.clear();
    }
};