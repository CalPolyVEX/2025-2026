#include "vex.h"

vex::triport        threeWirePort22     (vex::PORT22);
vex::pneumatics     lowerGate           (threeWirePort22.H);
vex::pneumatics     matchLoad           (threeWirePort22.G);
vex::encoder        test                (threeWirePort22.A);

vex::motor          L1                  (vex::PORT20, false);
vex::motor          L2                  (vex::PORT19, true);
vex::motor          L3                  (vex::PORT18, false);
vex::motor          L4                  (vex::PORT17, true);
vex::motor_group    leftDrive           (L1, L2, L3, L4);
vex::motor          R1                  (vex::PORT16, false);
vex::motor          R2                  (vex::PORT15, true);
vex::motor          R3                  (vex::PORT14, false);
vex::motor          R4                  (vex::PORT13, true);
vex::motor_group    rightDrive          (R1, R2, R3, R4);
vex::motor          conveyor            (vex::PORT11, false);

vex::optical        frontOpti           (vex::PORT8);

vex::distance       leftDist            (vex::PORT2);