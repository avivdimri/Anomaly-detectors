
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    vector<string> features = ts.getFeatures();
    vector<string> check;
    int size = features.size();
    for (int i = 0; i < size - 1; i++) {
        string f1 = features.at(i);
        vector<float> a = ts.getTable().find(f1)->second;
        float x[a.size()];
        for (int k = 0; k < a.size(); k++) {
            x[k] = a.at(k);
        }
        float maxCorrlation = 0;
        string f2_Max_cor;
        for (int j = i + 1; j < size; j++) {
            vector<float> b = ts.getTable().find(features.at(j))->second;
            float y[b.size()];
            for (int k = 0; k < b.size(); k++) {
                y[k] = b.at(k);
            }
            float cor = abs(pearson(x, y, a.size()));
            if (cor > maxCorrlation) {
                maxCorrlation = cor;
                f2_Max_cor = features.at(j);
            }
        }
        vector<float> m = ts.getTable().find(f1)->second;
        vector<float> b = ts.getTable().find(f2_Max_cor)->second;
        Point **points = new Point *[m.size()];
        for (int k = 0; k < m.size(); k++) {
            points[k] = new Point(m.at(k), b.at(k));
        }
        createFeatured(f1, f2_Max_cor, maxCorrlation, points, m.size(), check);
        for (int k = 0; k < m.size(); k++) {
            delete points[k];
        }
        delete[] points;
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<string> features = ts.getFeatures();
    vector<AnomalyReport> reports;
    vector<correlatedFeatures> correlFeat = this->cf;
    for (const auto &a : correlFeat) {
        for (int i = 0; i < ts.sizeCol(); i++) {
            string feature1 = a.feature1;
            string feature2 = a.feature2;
            Point point = Point(ts.getVectorByFeature(feature1)[i], ts.getVectorByFeature(feature2)[i]);
            if (isAnomalous(point, a)) {
                string description = feature1 += '-' + feature2;
                long timeStep = i + 1;
                reports.push_back(AnomalyReport(description, timeStep));
            }
        }
    }
    return reports;

}

float maxDeviation(Point **points, int size, Line line) {
    float maxdev = 0;
    float deviation = 0;
    for (int i = 0; i < size; ++i) {
        deviation = dev(*points[i], line);
        if (deviation > maxdev) {
            maxdev = deviation;
        }
    }
    return maxdev;
}

void SimpleAnomalyDetector::createFeatured(string f1, string f2, float corrlation, Point **points, int size,
                                           vector<string> &check) {
    if (corrlation > 0.9) {
        correlatedFeatures c;
        c.feature1 = f1;
        c.feature2 = f2;
        c.corrlation = corrlation;
        c.lin_reg = linear_reg(points, size);
        c.threshold = 1.1 * maxDeviation(points, size, c.lin_reg);
        this->cf.push_back(c);
    }

}


bool SimpleAnomalyDetector::isAnomalous(Point p, correlatedFeatures c) {
    bool q = (abs(p.y - c.lin_reg.f(p.x)) > c.threshold);
    if (q) {
        return true;
    }
    return false;
}