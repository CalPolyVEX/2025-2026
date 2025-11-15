#include "vex.h"
#include "devices.h"
#include <vex_task.h>
//using namespace vex;
vex::competition Competition;

int main()
{
        
    vex::wait(200, vex::msec); // triports initializing
    // vexcodeInit();

    
    // while (1)
    // {
    //     Brain.Screen.clearScreen();
    //     Brain.Screen.printAt(20, 40, "encoder pos: %lf", test.position(vex::rotationUnits::deg) * (90.0 / 2048.0));
    //     vex::wait(100, msec);
    // }

    // Set up callbacks for autonomous and driver control periods.
    // Competition.autonomous(autonomous);
    // Competition.drivercontrol(usercontrol);
}
