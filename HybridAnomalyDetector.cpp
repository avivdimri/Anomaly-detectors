
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::createFeatured(string f1, string f2, float corrlation, Point **points, int size,vector<string> &check) {
    if (corrlation > 0.9) {
        return SimpleAnomalyDetector::createFeatured(f1, f2, corrlation, points, size,check);
    }
    if (corrlation > 0.5) {
        correlatedFeatures c;
        c.feature1 = f1;
        c.feature2 = f2;
        c.corrlation = corrlation;
        Circle circle = findMinCircle(points, size);
        c.x = circle.center.x;
        c.y = circle.center.y;
        c.threshold = 1.1 * (circle.radius);
        this->cf.push_back(c);
    }
}

bool HybridAnomalyDetector::isAnomalous(Point p, correlatedFeatures c) {
    if (c.corrlation > 0.9) {
        return SimpleAnomalyDetector::isAnomalous(p, c);
    }
    float distance = getDistance(p, Point(c.x,c.y));
    return (distance > c.threshold);
}

