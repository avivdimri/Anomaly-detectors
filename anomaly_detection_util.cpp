/*
 * animaly_detection_util.cpp
 *
 * Author: write your ID and name here
 */

#include <math.h>
#include "anomaly_detection_util.h"


float avg(float *x, int size) {
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += *x;
        x++;
    }
    return sum / (float) size;
}

// returns the variance of X and Y
float var(float *x, int size) {
    float avg1 = avg(x, size);
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        float a = (*x - avg1);
        a *= a;
        sum += a;
        x++;
    }
    return sum / (float) size;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
    float t[size];
    for (int i = 0; i < size; ++i) {
        t[i] = x[i] * y[i];
    }
    return avg(t, size) - (avg(x, size) * avg(y, size));
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
    float deviationX = sqrt(var(x, size));
    float deviationY = sqrt(var(y, size));
    return cov(x, y, size) / (deviationX * deviationY);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {
    float x[size];
    float y[size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float avg_x = avg(x, size);
    float avg_y = avg(y, size);
    float a = cov(x, y, size);
    a = a / var(x, size);
    float b = avg_y - (a * avg_x);
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    Line l = linear_reg(points, size);
    float y1 = l.f(p.x);
    if (y1 > p.y) {
        return y1 - p.y;
    }
    return p.y - y1;
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    float y1 = l.f(p.x);
    if (y1 > p.y) {
        return y1 - p.y;
    }
    return p.y - y1;
}




