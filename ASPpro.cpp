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
        vector<string> v1;
        vector<string> v2;
        vector<string> v3;

        for (int i = 0; i < broj_podataka; i++) {
            if (mapa.at(table)[i].compare(arg) != 0) {
                v1.push_back(mapa.at("years")[i]);
                v2.push_back(mapa.at("days")[i]);
                v3.push_back(mapa.at("lengths")[i]);
            }
        }

        mapa["years"] = v1;
        mapa["days"] = v2;
        mapa["lengths"] = v3;
    }
 

    // Brisanje prvih n zapisa
    void deleteNBy(const string& table, const string& arg, int n) {
        vector<string> v1;
        vector<string> v2;
        vector<string> v3;

        for (int i = 0; i < broj_podataka; i++) {
            if (n > 0 && mapa.at(table)[i].compare(arg) == 0) {
                n--;
            }
            else {
                v1.push_back(mapa.at("years")[i]);
                v2.push_back(mapa.at("days")[i]);
                v3.push_back(mapa.at("lengths")[i]);
            }
        }

        mapa["years"] = v1;
        mapa["days"] = v2;
        mapa["lengths"] = v3;
    }

    // Dohvaćanje najmanje vrijednosti
    string getMinValue(const string& table) {
        return *min_element(mapa[table].begin(), mapa[table].end());
    }

    // Dohvaćanje najveće vrijednosti
    string getMaxValue(const string& table) {
        return *max_element(mapa[table].begin(), mapa[table].end());
    }

    // Dohvaćanje n najmanjih vrijednosti
    vector<string> getNSmallestValues(const string& table, int n) {
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
    vector<string> getNLargestValues(const string& table, int n) {
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
    const int broj_podataka = 500000;
    
    string godinaPretrazivanja = "2017";
    string godinaBrisanja = "2017";
    string novaGodina = "2019";
    string noviDan = "25";
    string novaDuzina = "200";
    
    vector<string> newYears = {"2019", "2019"};
    vector<string> newDays = {"30", "15"};
    vector<string> newLengths = {"300", "250"};

    int nbrisanja = 2;
    int n = 3;

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
    
    string minYear = db.getMinValue("years");
    string maxYear = db.getMaxValue("years");
    cout << "Minimalna vrijednost: " << minYear << endl;
    cout << "Maximalna vrijednost: " << maxYear << endl;

    vector<string> nSmallestYears = db.getNSmallestValues("years", n);
    cout << "N najmanjih vrijednosti:" << endl;
    for (const string& year : nSmallestYears) {
        cout << year << endl;
    }

    vector<string> nLargestYears = db.getNLargestValues("years", n);
    cout << "N najvećih vrijednosti:" << endl;
    for (const string& year : nLargestYears) {
        cout << year << endl;
    }
    
    db.addRecord(novaGodina, noviDan, novaDuzina);
    db.addNRecords(newYears, newDays, newLengths, newYears.size());
    */

    /*db.deleteBy("years", godinaBrisanja);
    for (const auto& result : db.searchBy("years", godinaPretrazivanja)) {
        cout << result << endl;
    }*/


    db.deleteNBy("years", "2017", 2);
    for (const auto& result : db.searchNBy("years", "2017", 6)) {
        cout << result << endl;
    }


    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    
    cout << "Operacija izvršena u " << duration.count() * 1000 << " milisekundi." << endl;

    return 0;
}