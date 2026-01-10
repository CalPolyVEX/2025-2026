#include "vex.h"


vex::triport threeWirePort22 = vex::triport(PORT22);
vex::pneumatics loader = vex::pneumatics(threeWirePort22.G);
vex::pneumatics wing = vex::pneumatics(threeWirePort22.F);

vex::motor L1 = vex::motor(PORT10, true);
vex::motor L2 = vex::motor(PORT9, true);
vex::motor L3 = vex::motor(PORT8, true);
vex::motor L4 = vex::motor(PORT7, false);
vex::motor_group leftDrive = vex::motor_group(L1, L2, L3, L4);
vex::motor R1 = vex::motor(PORT4, true);
vex::motor R2 = vex::motor(PORT3, false);
vex::motor R3 = vex::motor(PORT2, false);
vex::motor R4 = vex::motor(PORT1, false);
vex::motor_group rightDrive = vex::motor_group(R1, R2, R3, R4);
vex::motor intake1 = vex::motor(PORT6, false);
vex::motor intake2 = vex::motor(PORT5, true);
vex::motor_group intake = vex::motor_group(intake1, intake2);
vex::motor roller = vex::motor(PORT11, false);
vex::motor hood = vex::motor(PORT12, true);

vex::optical sort = vex::optical(PORT18);



