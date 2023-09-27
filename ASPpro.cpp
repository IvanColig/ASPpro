#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
using namespace std;


class CSVDatabase {
public:
    CSVDatabase(const string& datoteka, const int broj_podataka) : datoteka(datoteka), broj_podataka(broj_podataka) {
        mapa.emplace("years", vector<string>());
        mapa.emplace("days", vector<string>());
        mapa.emplace("lengths", vector<string>());
        loadCSV(broj_podataka);
    }

    // Pretraživanje zapisa
    vector<string> searchBy(const string& table, const string& arg) {
        vector<string> vec = mapa.at(table);
        vector<string> results;

        for (int i = 0; i < broj_podataka; i++) {
            if (vec[i] == arg) {
                string result = mapa.at("years")[i] + "," + mapa.at("days")[i] + "," + mapa.at("lengths")[i];
                results.push_back(result);
            }
        }
        return results;
    }

    // Pretraživanje prvih n zapisa
    vector<string> searchNBy(const string& table, const string& arg, int n) {
        vector<string> vec = mapa.at(table);
        vector<string> results;

        for (int i = 0; i < broj_podataka && n > 0; i++) {
            if (vec[i] == arg) {
                string result = mapa.at("years")[i] + "," + mapa.at("days")[i] + "," + mapa.at("lengths")[i];
                results.push_back(result);
                n--;
            }
        }
        return results;
    }

    // Brisanje zapisa
    void deleteBy(const string& table, const string& arg) {
        vector<string>& vec = mapa.at(table);

        for (int i = broj_podataka - 1; i >= 0 ; i--) {
            if (vec[i] == arg) {
                mapa.at("years").erase(mapa.at("years").begin() + i);
                mapa.at("days").erase(mapa.at("days").begin() + i);
                mapa.at("lengths").erase(mapa.at("lengths").begin() + i);
            }
        }
    }


    // Brisanje prvih n zapisa
    void deleteNBy(const string& table, const string& arg, int n) {
    vector<string>& vec = mapa.at(table);
    vector<string>& yearsVec = mapa.at("years");
    vector<string>& daysVec = mapa.at("days");
    vector<string>& lengthsVec = mapa.at("lengths");

    for (int i = broj_podataka - 1; i >= 0 && n > 0; i--) {
        if (vec[i] == arg) {
            yearsVec.erase(yearsVec.begin() + i);
            daysVec.erase(daysVec.begin() + i);
            lengthsVec.erase(lengthsVec.begin() + i);
            n--;
        }
    }
}

    // Dohvaćanje najmanje vrijednosti
    string getMinValueByKey(const string& table) {
        return *min_element(mapa[table].begin(), mapa[table].end());
    }

    // Dohvaćanje najveće vrijednosti
    string getMaxValueByKey(const string& table) {
        return *max_element(mapa[table].begin(), mapa[table].end());
    }

    // Dohvaćanje n najmanjih vrijednosti
    vector<string> getNSmallestValuesByKey(const string& table, int n) {
        vector<string> results;
        vector<string>& vec = mapa[table];
        vector<string> sortedVec = vec;

        sort(sortedVec.begin(), sortedVec.end());

        for (int i = 0; i < n && i < sortedVec.size(); i++) {
        results.push_back(sortedVec[i]);
    } 

        return results;
    }

    // Dohvaćanje n najvećih vrijednosti
    vector<string> getNLargestValuesByKey(const string& table, int n) {
        vector<string> results;
        vector<string>& vec = mapa[table];
        vector<string> rSortedVec = vec;

        sort(rSortedVec.rbegin(), rSortedVec.rend());

        for (int i = 0; i < n && i < rSortedVec.size(); i++) {
            results.push_back(rSortedVec[i]);
        }

        return results;
    }

    // Dodavanje zapisa
    void addRecord(const string& year, const string& day, const string& length) {
        mapa.at("years").push_back(year);
        mapa.at("days").push_back(day);
        mapa.at("lengths").push_back(length);
    }

    // Dodavanje n zapisa
    void addNRecords(const vector<string>& years, const vector<string>& days, const vector<string>& lengths, int n) {
        for (int i = 0; i < n; i++) {
            mapa.at("years").push_back(years[i]);
            mapa.at("days").push_back(days[i]);
            mapa.at("lengths").push_back(lengths[i]);
        }
    }

private:
    string datoteka;
    map<string, vector<string>> mapa;
    int broj_podataka;

    void loadCSV(int broj_podataka) {
        ifstream file(datoteka);
        if (!file.is_open()) {
            throw runtime_error("Unable to open CSV file");
        }

        string line;
        getline(file,line);
        for (int i = 0; i < broj_podataka; i++) {
            getline(file,line);
            istringstream ss(line);
            string token, year, day, length;

            getline(ss, token, ',');
            getline(ss, year, ',');
            getline(ss, day, ',');
            getline(ss, length, ',');
            mapa.at("years").push_back(year);
            mapa.at("days").push_back(day);
            mapa.at("lengths").push_back(length);

        }

        file.close();
    }
};

int main() {
    const string datoteka = "C:\\asp\\BitcoinHeistData.csv";
    const int broj_podataka = 500;

    string godinaPretrazivanja = "2017";
    string godinaBrisanja = "2017";
    string novaGodina = "2019";
    string noviDan = "25";
    string novaDuzina = "200";
    
    vector<string> newYears = {"2019", "2019"};
    vector<string> newDays = {"30", "15"};
    vector<string> newLengths = {"300", "250"};

    int nbrisanja = 2;
    int n = 4;

    CSVDatabase db(datoteka, broj_podataka);

    
    auto start = chrono::high_resolution_clock::now();
    

    
    /*for (const auto& result : db.searchBy("years", godinaPretrazivanja)) {
        cout << result << endl;
    }

    for (const auto& result : db.searchNBy("years", godinaPretrazivanja, n)) {
        cout << result << endl;
    }

    db.deleteBy("years", godinaBrisanja);
    db.deleteNBy("years", godinaBrisanja, nbrisanja);
    
    string minYear = db.getMinValueByKey("years");
    string maxYear = db.getMaxValueByKey("years");
    cout << "Minimalna vrijednost: " << minYear << endl;
    cout << "Maximalna vrijednost: " << maxYear << endl;

    vector<string> nSmallestYears = db.getNSmallestValuesByKey("years", n);
    cout << "N najmanjih vrijednosti:" << endl;
    for (const string& year : nSmallestYears) {
        cout << year << endl;
    }

    vector<string> nLargestYears = db.getNLargestValuesByKey("years", n);
    cout << "N najvećih vrijednosti:" << endl;
    for (const string& year : nLargestYears) {
        cout << year << endl;
    }
    
    db.addRecord(novaGodina, noviDan, novaDuzina);
    db.addNRecords(newYears, newDays, newLengths, newYears.size());
    */

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    
    cout << "Operacija izvršena u " << duration.count() * 1000 << " milisekundi." << endl;

    return 0;
}