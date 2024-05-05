#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <typeinfo>
#include <time.h>
#include <sys/stat.h>
#include <direct.h>
#include <filesystem>
#include <variant>

using namespace std;


vector<int> generateRandomIntegers(int lowerBound, int upperBound, int n);

double getRandomNumber();

double getRandomUniform(double lowerBound, double upperBound);

vector<int> generateMultinomialSample(double p[], int n);

unordered_map<string, int> readTimeString(string& timeString);

int timeDiff(unordered_map<string, int>& t1, unordered_map<string, int>& t2, const string& unit);

unordered_map<string, int> addDays(unordered_map<string, int>& date, int days);

void writeToCSV(const string& filename, const vector<vector<string>>& data);

// vector<Event*> readEventQueueAsObject(const string& eqPath);

vector<unordered_map<string, string>> readEventQueueAsMap(const string& eqPath);

vector<double> readCSVFileSingleColumns(const string& path);

vector<vector<string>> readCSVFileMultipleColumns(const string& path);

unordered_map<string, double> constructTransmissionParamsMap(const string& cwd);

#endif