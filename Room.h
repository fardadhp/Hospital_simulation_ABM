#ifndef ROOM_H
#define ROOM_H

#include <string>
using namespace std;

class Room {
    private:
        string ID;
        bool contamination;
        double disinfectEfficacy;
        double naturalClearanceRate;
    public:
        // (de)constructors
        Room();
        Room(string ID_, bool contamination_, double disinfectEfficacy_, double naturalClearanceRate_);
        ~Room();
        // getters
        string getID();
        bool getContamination();
        double getDisinfectEfficacy();
        double getNaturalClearanceRate();
        //setters;
        void setContamination(bool contamination_);
        void setDisinfectEfficacy(double disinfectEfficacy_);
        void setNaturalClearanceRate(double naturalClearanceRate_);
        // Methods
        void disinfect();
        void naturalClearance();
};

#endif