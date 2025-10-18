#include "vex.h"

class FancyEncoder: public encoder {
public:
    using encoder::encoder;

    /**
     * @brief Gets the value of the encoder.
     * @return Returns an integer that represents the value of the encoder device.
     */
    int32_t value();

    /**
     * @brief Gets the rotation value of the encoder.
     * @return Returns a double that represents the current rotation of the encoder.
     * @param units The measurement unit for the encoder device.
     */
    double rotation(rotationUnits units);

    /**
     * @brief Gets the velocity of the encoder.
     * @return Returns a double that represents the current velocity of the encoder.
     * @param units The measurement unit for the encoder device.
     */
    double velocity(velocityUnits units);
};