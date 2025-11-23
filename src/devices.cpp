#include "vex.h"


vex::triport threeWirePort22 = vex::triport(PORT22);
vex::pneumatics midGoal = vex::pneumatics(threeWirePort22.H);
vex::pneumatics loader = vex::pneumatics(threeWirePort22.G);
vex::pneumatics wing = vex::pneumatics(threeWirePort22.F);

vex::motor L1 = vex::motor(PORT11, true);
vex::motor L2 = vex::motor(PORT12, false);
vex::motor L3 = vex::motor(PORT13, true);
vex::motor L4 = vex::motor(PORT14, true);
vex::motor_group leftDrive = vex::motor_group(L1, L2, L3, L4);
vex::motor R1 = vex::motor(PORT20, false);
vex::motor R2 = vex::motor(PORT19, true);
vex::motor R3 = vex::motor(PORT18, false);
vex::motor R4 = vex::motor(PORT17, false);
vex::motor_group rightDrive = vex::motor_group(R1, R2, R3, R4);
vex::motor intake = vex::motor(PORT3, false);
vex::motor hood = vex::motor(PORT4, true);

vex::optical frontOpti = vex::optical(PORT20);



