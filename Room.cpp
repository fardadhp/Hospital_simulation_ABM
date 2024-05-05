#include "Room.h"
#include <iostream>

// (de)constructors
Room::Room() : ID(""), contamination(0), disinfectEfficacy(0), naturalClearanceRate(0) {};
Room::Room(string ID_, bool contamination_, double disinfectEfficacy_, double naturalClearanceRate_) {
    ID = ID_;
    contamination = contamination_;
    disinfectEfficacy = disinfectEfficacy_; 
    naturalClearanceRate = naturalClearanceRate_; 
};

Room::~Room() {
    // cout << "Destructing Room " << ID << endl;
};

// getters
string Room::getID() {
    return ID;
};
bool Room::getContamination() {
    return contamination;
};
double Room::getDisinfectEfficacy() {
    return disinfectEfficacy;
};
double Room::getNaturalClearanceRate() {
    return naturalClearanceRate;
};
//setters
void Room::setContamination(bool contamination_) {
    contamination = contamination_;
};
void Room::setDisinfectEfficacy(double disinfectEfficacy_) {
    disinfectEfficacy = disinfectEfficacy_;
};
void Room::setNaturalClearanceRate(double naturalClearanceRate_) {
    naturalClearanceRate = naturalClearanceRate_;
};
// Methods
void Room::disinfect() {
    double randomi = ((double) rand() / (RAND_MAX));
    if (randomi < disinfectEfficacy) {
        contamination = 0;
    }
}

void Room::naturalClearance() {
    double randomi = ((double) rand() / (RAND_MAX));
    if (randomi < naturalClearanceRate) {
        contamination = 0;
    }
}