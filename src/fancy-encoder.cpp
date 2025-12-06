#include "vex.h"
using namespace vex;


int32_t FancyEncoder::value() {
    return encoder::value() * (90.0 / 2048.0);
}

double FancyEncoder::rotation(rotationUnits units) {
    return encoder::rotation(units) * (90.0 / 2048.0);
}

double FancyEncoder::velocity(velocityUnits units) {
    return encoder::velocity(units) * (90.0 / 2048.0);
}
