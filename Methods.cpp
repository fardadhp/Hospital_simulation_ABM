#include "Methods.h"

vector<int> generateRandomIntegers(int lowerBound, int upperBound, int n) {
    srand(rand() % 100);
    vector<int> randomIntegers;
    for (int i = 0; i < n; ++i) {
        int randomNumberInRange = rand() % (upperBound - lowerBound + 1) + lowerBound;
        randomIntegers.push_back(randomNumberInRange);
    }
    return randomIntegers;
};

double getRandomNumber() {
    srand(rand() % 100);
    double randomi = ((double) rand() / RAND_MAX);
    return randomi;
};

double getRandomUniform(double lowerBound, double upperBound) {
    srand(rand() % 100);
    double randomi = ((double) rand() / RAND_MAX);
    randomi = randomi * (upperBound - lowerBound) + lowerBound;
    return randomi;
};

vector<int> generateMultinomialSample(double p[], int n) {
    vector<int> output;
    for (int ni=0; ni<n; ++ni) {
        double randomi = ((double) rand() / RAND_MAX);
        if (randomi < p[0])
            output.push_back(0);
        else {
            double newP = p[0];
            bool flag = true;
            int i = 1;
            while (flag) {
                newP += p[i];
                if (randomi < newP) {
                    output.push_back(i);
                    flag = false;
                } 
                else
                    i++;
            }
        }
    }
    return output;
};

unordered_map<string, int> readTimeString(string& timeString) {
    unordered_map<string, int> t1;
    if (timeString.size() > 10) {
        timeString = timeString.substr(0, timeString.find(" PM"));
        timeString = timeString.substr(0, timeString.find(" AM"));
        string line;
        string delimiter = " ";
        string date = timeString.substr(0, timeString.find(delimiter));
        string timeUnit[] = {"year", "month", "day"};
        for (int i=0; i<3; ++i) {
            t1[timeUnit[i]] = stoi(date.substr(0, date.find('-')));
            date = date.substr(date.find('-') + 1, date.length());
        }
        string time = timeString.substr(timeString.find(delimiter) + 2, timeString.length());
        string timeUnit2[] = {"hour", "minute", "second"};
        for (int i=0; i<3; ++i) {
            t1[timeUnit2[i]] = stoi(time.substr(0, time.find(':')));
            time = time.substr(time.find(':') + 1, time.length());
        }
    } 
    else {
        string timeUnit[] = {"year", "month", "day"};
        string delimiter = "_";
        for (int i=0; i<3; ++i) {
            t1[timeUnit[i]] = stoi(timeString.substr(0, timeString.find(delimiter)));
            timeString = timeString.substr(timeString.find(delimiter) + 1, timeString.length());
        }
    }
    return t1;
};

int timeDiff(unordered_map<string, int>& t1, unordered_map<string, int>& t2, const string& unit) {
    unordered_map<string, int> diff;
    string piece[] = {"hour", "minute", "second", "month", "day", "year"};
    int numPieces = static_cast<int>(sizeof(piece) / sizeof(piece[0]));
    for (int i = 0; i < numPieces; ++i)
        diff[piece[i]] = t2[piece[i]] - t1[piece[i]];
    int diff_years = diff["year"];
    int diff_months = diff_years * 12 + diff["month"];
    int diff_days = diff_months * 30 + diff["day"];
    int diff_hours = diff_days * 24 + diff["hour"];
    int diff_mins = diff_hours * 60 + diff["minute"];
    int diff_secs = diff_mins * 60 + diff["second"];
    
    int output;
    int idx = 0;
    bool flag = true;
    while (flag){
        if (piece[idx] == unit)
            flag = false;
        else
            idx++;
    }
    idx += 1;
    switch (idx) {
        case 1:
            output = diff_hours;
            break;
        case 2:
            output = diff_mins;
            break;
        case 3:
            output = diff_secs;
            break;
        case 4:
            output = diff_months;
            break;
        case 5:
            output = diff_days;
            break;
        case 6:
            output = diff_years;
            break;
    };
    
    return output;
};

unordered_map<string, int> addDays(unordered_map<string, int>& date, int days) {
    date["day"] += days;
    if (date["day"] > 30) {
        date["month"] += 1;
        date["day"] = 1;
        if (date["month"] > 12) {
            date["year"] += 1;
            date["month"] = 1;
        }
    }
    return date;
};

void writeToCSV(const string& filename, const vector<vector<string>>& data) {
    ofstream file(filename); 
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(-1);
    }
    for (const auto& row : data) {
        for (auto it = row.begin(); it != row.end(); ++it) {
            file << *it; 
            if (next(it) != row.end())
                file << ",";
        }
        file << "\n"; 
    }
    file.close(); 
};

// vector<Event*> readEventQueueAsObject(string& eqPath) {
//     vector<Event*> event_queue;
//     fstream infile(eqPath);
//     char buffer[65536];
//     infile.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
//     string line;
//     vector<string> splittedString;
//     getline(infile, line); // skip the header line
//     while (getline(infile, line)) {
//         splittedString.clear();
//         size_t last = 0, pos = 0;
//         while ((pos = line.find(",", last)) != string::npos) {
//             splittedString.emplace_back(line, last, pos - last);
//             last = pos + 1;
//         }
//         if (last)
//             splittedString.emplace_back(line, last);
//         Event ev = Event(splittedString);
//         event_queue.push_back(&ev);
//     }
// 
//     return event_queue;
// };

vector<unordered_map<string, string>> readEventQueueAsMap(const string& eqPath) {
    vector<unordered_map<string, string>> event_queue;
    unordered_map<string, string> event;
    fstream infile(eqPath);
    char buffer[65536];
    infile.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
    string line, item;
    vector<string> splittedString, keys;
    getline(infile, line); // read header
    size_t last = 0, pos = 0;
    while ((pos = line.find(",", last)) != string::npos) {
        keys.emplace_back(line, last, pos - last);
        last = pos + 1;
    }
    if (last)
        keys.emplace_back(line, last);
    while (getline(infile, line)) {
        splittedString.clear();
        last = 0; pos = 0;
        while ((pos = line.find(",", last)) != string::npos) {
            splittedString.emplace_back(line, last, pos - last);
            last = pos + 1;
        }
        if (last)
            splittedString.emplace_back(line, last);
        int ub = static_cast<int>(splittedString.size());
        for (int i=0; i<ub; ++i) {
            event[keys[i]] = splittedString[i];
        }
        event_queue.push_back(event);
    }

    return event_queue;
};

vector<double> readCSVFileSingleColumns(const string& path) {
    vector<double> data;
    ifstream file;
    string line;
    file.open(path);
    while (getline(file, line)) {
        data.push_back(stoi(line));
    };
    file.close();

    return data;
};

vector<vector<string>> readCSVFileMultipleColumns(const string& path) {
    vector<vector<string>> data;
    fstream infile(path);
    char buffer[65536];
    infile.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
    string line;
    vector<string> splittedString;
    getline(infile, line); // skip the header line
    while (getline(infile, line)) {
        splittedString.clear();
        size_t last = 0, pos = 0;
        while ((pos = line.find(",", last)) != string::npos) {
            splittedString.emplace_back(line, last, pos - last);
            last = pos + 1;
        }
        if (last)
            splittedString.emplace_back(line, last);
        data.push_back(splittedString);
    }

    return data;
};

unordered_map<string, double> constructTransmissionParamsMap(const string& path) {
    unordered_map<string, double> transmissionParams;
    ifstream file;
    string line;
    // read params
    file.open(path);
    getline(file, line); //skip header line
    while (getline(file, line)) {
        istringstream iss(line);
        string parameter, value;
        if (getline(iss, parameter, ',') && getline(iss, value))
            transmissionParams[parameter] = stod(value);
    }
    file.close();

    return transmissionParams;
};