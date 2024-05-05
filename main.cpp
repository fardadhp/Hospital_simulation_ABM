// #define _CRTDBG_MAP_ALLOC
#include "Methods.h" 
#include "Hospital.h"
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string cwd = "C:/Users/FardadHaghpanah/Dropbox (Personal)/ABM 3 CPP";
    string T0 = "2017_06_01";
    int burnIn = 30;
    int days = 360 + burnIn;
    string hospitalDataPath = cwd + "/data/hospitals_data.csv";
    std::vector<std::vector<string>> hospitalData = readCSVFileMultipleColumns(hospitalDataPath);
    string ID_ = hospitalData[0][0];
    string name_ = hospitalData[0][1];
    int nUnits_ = stoi(hospitalData[0][2]);
    int surveillance_freq_ = stoi(hospitalData[0][3]);
    double probability_admission_testing_ = stod(hospitalData[0][4]);
    double probablity_surveillance_ = stod(hospitalData[0][5]);
    Hospital hospital = Hospital(ID_, name_, nUnits_, surveillance_freq_, probability_admission_testing_, probablity_surveillance_, days, burnIn, T0);
    hospital.setCWD(cwd);
    hospital.setup();
    hospital.simulate();
    cout << "Done!" << endl;
    // _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
    // _CrtDumpMemoryLeaks();
    return 0;
};