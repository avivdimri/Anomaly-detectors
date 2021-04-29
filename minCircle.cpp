#include "minCircle.h"
float getDistance(const Point &p1, const Point &p2) {
    float dx = pow(p1.x - p2.x, 2);
    float dy = pow(p1.y - p2.y, 2);
    return sqrt(dx + dy);
}

Circle getCircleByTwoPoints(const Point &p1, const Point &p2) {
    float mid_x = (p1.x + p2.x) / 2;
    float mid_y = (p1.y + p2.y) / 2;
    float dx = pow(p1.x - p2.x, 2);
    float dy = pow(p1.y - p2.y, 2);
    float radius = (sqrt(dx + dy)) / 2;
    return Circle(Point(mid_x, mid_y), radius);
}

Circle getCircleByThreePoints(const Point &p1, const Point &p2, const Point &p3) {
    float x_p2 = p2.x - p1.x;
    float y_p2 = p2.y - p1.y;
    float a = pow(x_p2,2) + pow(y_p2, 2);
    float x_p3 = p3.x - p1.x;
    float y_p3 = p3.y - p1.y;
    float b = pow(x_p3, 2) + pow(y_p3, 2);
    float c = x_p2 * y_p3 - y_p2 * x_p3;
    Point center = Point(((y_p3 * a - y_p2 * b) / (2 * c)) + p1.x, ((x_p2 * b - x_p3 * a) / (2 * c)) + p1.y);
    float radius = getDistance(center, p1);
    return Circle(center, radius);
}

// implement
Circle findMinCircle(Point **points, size_t size, vector<Point> vec ) {
    if (vec.size() == 3) {
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                Circle c = getCircleByTwoPoints(vec.at(i), vec.at(j));
                bool flag = true;
                for (int k = 0; k < 3; k++) {
                    if (getDistance(vec.at(k), c.center) > c.radius) {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    return c;
                }
            }
        }
        return getCircleByThreePoints(vec.at(0), vec.at(1), vec.at(2));
    }
    if (size == 0) {
        switch (vec.size()) {
            case 0:
                return Circle(Point(0, 0), 0);
            case 1:
                return Circle(vec.at(0), 0);
            default:
                return getCircleByTwoPoints(vec.at(0), vec.at(1));
        }
    }
    Point p = *points[size - 1];
    Circle c = findMinCircle(points, size - 1, vec);
    if (getDistance(p, c.center) <= c.radius) {
        return c;
    }
    vec.push_back(p);
    return findMinCircle(points, size - 1, vec);
}
