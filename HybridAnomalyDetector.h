

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    void createFeatured(string f1, string f2, float corrlation, Point **points, int size,vector<string> &check) override ;
    bool isAnomalous(Point p, correlatedFeatures c) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
