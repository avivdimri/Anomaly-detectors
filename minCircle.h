// id 20632240

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <cmath>
#include <vector>
#include "anomaly_detection_util.h"
using namespace std;

// ------------ DO NOT CHANGE -----------

class Circle {
public:
    Point center;
    float radius;
    Circle(Point c, float r) : center(c), radius(r) {}
};
Circle findMinCircle(Point **points, size_t size, vector<Point> vec = {});
Circle getCircleByThreePoints(const Point &p1, const Point &p2, const Point &p3);
Circle getCircleByTwoPoints(const Point &p1, const Point &p2);
float getDistance(const Point &p1, const Point &p2);

#endif /* MINCIRCLE_H_ */
