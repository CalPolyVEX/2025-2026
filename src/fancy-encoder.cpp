#include "vex.h"
using namespace vex;

class FancyEncoder: public encoder {
public:
    int32_t value() {
        return encoder::value() * (90.0 / 2048.0);
    }

    double rotation(rotationUnits units) {
        return encoder::rotation(units) * (90.0 / 2048.0);
    }

    double velocity(velocityUnits units) {
        return encoder::velocity(units) * (90.0 / 2048.0);
    }
};