#include "timeseries.h"
#include "timeseries.h"
#include <map>
#include "timeseries.h"
#include <fstream>
#include <string>
#include <vector>

void TimeSeries::initFeatures(string &line, char delimiter) {
    int index = 0;
    string feature;
    int nextcommon = line.find(delimiter, index);
    int sizesub = nextcommon - index;
    vector<float> a;
    while (nextcommon != -1) {
        feature = line.substr(index, sizesub);
        this->features.push_back(feature);
        this->table.insert({feature, a});
        index = nextcommon + 1;
        nextcommon = line.find(delimiter, index);
        sizesub = nextcommon - index;
    }
    feature = line.substr(index, line.length() - index);
    this->features.push_back(feature);
    this->table.insert({feature, a});

}

void TimeSeries::fillMatrix(string &line, char delimiter) {
    int index = 0;
    int k = 0;
    float data;
    int nextcommon = line.find(delimiter, index);
    int sizesub = nextcommon - index;
    while (nextcommon != -1) {
        data = stof(line.substr(index, sizesub));
        this->table.find(this->features.at(k))->second.push_back(data);
        index = nextcommon + 1;
        nextcommon = line.find(delimiter, index);
        sizesub = nextcommon - index;
        k++;
    }
    data = stof(line.substr(index, sizesub));
    this->table.find(this->features.at(k))->second.push_back(data);
}

TimeSeries::TimeSeries(const char *CSVfileName) {
    this->filename = CSVfileName;
    fstream file;
    file.open(CSVfileName);
    string line;
    char delimiter = ',';
    bool flag = true;
    while (getline(file, line)) {
        if (flag) {
            initFeatures(line, delimiter);
            flag = false;
        } else {
            fillMatrix(line, delimiter);
        }
    }
}

vector<string> TimeSeries::getFeatures() const {
    return this->features;
}

map<string, vector<float> > TimeSeries::getTable() const {
    return this->table;
}

vector<float> TimeSeries::getVectorByFeature(string feature) const {
    return this->table.at(feature);
}

vector<float> TimeSeries::getRowByIndex(int index) const {
    vector<float> row;
    for (int i = 0; i < this->features.size(); i++) {
        row.at(i) = this->getVectorByFeature(this->features.at(i)).at(index);
    }


}

int TimeSeries::sizeCol() const {
   return this->getVectorByFeature(this->features.at(0)).size();
}
