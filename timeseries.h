
#include <map>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
    const char *filename;
    vector<string> features;
    map<string, vector<float>> table;
public:

    TimeSeries(const char *CSVfileName);

    void initFeatures(string &line, char delimiter);

    void fillMatrix(string &line, char delimiter);

    vector<string> getFeatures() const;

    map<string, vector<float> > getTable() const;

    vector<float> getVectorByFeature(string feature) const;

    vector<float> getRowByIndex(int index) const;

    int sizeCol() const;
};


#endif /* TIMESERIES_H_ */
