#include "vex.h"


vex::triport threeWirePort22 = vex::triport(PORT22);
vex::pneumatics midGoal = vex::pneumatics(threeWirePort22.H);
vex::pneumatics loader = vex::pneumatics(threeWirePort22.G);
vex::pneumatics wing = vex::pneumatics(threeWirePort22.F);

vex::motor L1 = vex::motor(PORT20, true);
vex::motor L2 = vex::motor(PORT19, true);
vex::motor L3 = vex::motor(PORT18, true);
vex::motor_group leftDrive = vex::motor_group(L1, L2, L3);
vex::motor R1 = vex::motor(PORT16, false);
vex::motor R2 = vex::motor(PORT15, false);
vex::motor R3 = vex::motor(PORT14, false);
vex::motor_group rightDrive = vex::motor_group(R1, R2, R3);
vex::motor intake = vex::motor(PORT16, false);
vex::motor hood = vex::motor(PORT15, true);

vex::optical frontOpti = vex::optical(PORT20);



