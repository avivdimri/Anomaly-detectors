

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    float x;
    float y;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:
    vector<correlatedFeatures> cf;
    float threshold;
public:
    SimpleAnomalyDetector();

    ~SimpleAnomalyDetector() override;

    void learnNormal(const TimeSeries &ts) override;

    vector<AnomalyReport> detect(const TimeSeries &ts) override;

    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
    void setCorrelationThreshold(float threshold){
        this->threshold=threshold;
    }
protected:
    virtual void createFeatured(string f1, string f2, float corrlation,  Point **points, int size,vector<string> &check);
    virtual bool isAnomalous(Point p,correlatedFeatures c);
};

float maxDeviation(Point **points, int size, Line line);

#endif /* SIMPLEANOMALYDETECTOR_H_ */
