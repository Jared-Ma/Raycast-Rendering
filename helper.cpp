#include <cmath>
#include "helper.hpp"

float toRadians(float deg) {
    return deg * M_PI / 180;
}

float toDegrees(float rad) {
    return rad * 180 / M_PI;
}

float calcDistance(float x1, float y1, float x2, float y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}