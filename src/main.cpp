#include "vex.h"
#include "devices.h"
#include "chassis.h"
#include <vex_task.h>
#include "controls.h"
//using namespace vex;
competition Competition;

/*---------------------------------------------------------------------------*/
/*                             VEXcode Config                                */
/*                                                                           */
/*  Before you do anything else, start by configuring your motors and        */
/*  sensors. In VEXcode Pro V5, you can do this using the graphical          */
/*  configurer port icon at the top right. In the VSCode extension, you'll   */
/*  need to go to robot-config.cpp and robot-config.h and create the         */
/*  motors yourself by following the style shown. All motors must be         */
/*  properly reversed, meaning the drive should drive forward when all       */
/*  motors spin forward.                                                     */
/*---------------------------------------------------------------------------*/


int current_auton_selection = 0;
bool auto_started = false;

/**
 * Function before autonomous. It prints the current auton number on the screen
 * and tapping the screen cycles the selected auton by 1. Add anything else you
 * may need, like resetting pneumatic components. You can rename these autons to
 * be more descriptive, if you like.
 */

void pre_auton()
{
    // Initializing Robot Configuration. DO NOT REMOVE!

    default_constants();

    while (!auto_started)
    {
        Brain.Screen.clearScreen();
        Brain.Screen.printAt(5, 20, "JAR Template v1.2.0");
        Brain.Screen.printAt(5, 40, "Battery Percentage:");
        Brain.Screen.printAt(5, 60, "%d", Brain.Battery.capacity());
        Brain.Screen.printAt(5, 80, "Chassis Heading Reading:");
        Brain.Screen.printAt(5, 100, "%f", chassis.get_absolute_heading());
        Brain.Screen.printAt(5, 120, "Selected Auton:");
        switch (current_auton_selection)
        {
        case 0:
            Brain.Screen.printAt(5, 140, "Auton 1");
            break;
        case 1:
            Brain.Screen.printAt(5, 140, "Auton 2");
            break;
        case 2:
            Brain.Screen.printAt(5, 140, "Auton 3");
            break;
        case 3:
            Brain.Screen.printAt(5, 140, "Auton 4");
            break;
        case 4:
            Brain.Screen.printAt(5, 140, "Auton 5");
            break;
        case 5:
            Brain.Screen.printAt(5, 140, "Auton 6");
            break;
        case 6:
            Brain.Screen.printAt(5, 140, "Auton 7");
            break;
        case 7:
            Brain.Screen.printAt(5, 140, "Auton 8");
            break;
        }
        if (Brain.Screen.pressing())
        {
            while (Brain.Screen.pressing())
            {
            }
            current_auton_selection++;
        }
        else if (current_auton_selection == 8)
        {
            current_auton_selection = 0;
        }
        
        
    }
}

/**
 * Auton function, which runs the selected auton. Case 0 is the default,
 * and will run in the brain screen goes untouched during preauton. Replace
 * drive_test(), for example, with your own auton function you created in
 * autons.cpp and declared in autons.h.
 */

void autonomous(void)
{
    
    auto_started = true;
    switch (current_auton_selection)
    {
    case 0:
        drive_test();
        break;
    case 1:
        drive_test();
        break;
    case 2:
        turn_test();
        break;
    case 3:
        swing_test();
        break;
    case 4:
        full_test();
        break;
    case 5:
        odom_test();
        break;
    case 6:
        tank_odom_test();
        break;
    case 7:
        holonomic_odom_test();
        break;
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
    

    while (1){
        //printf("driving\n");
        
        vex::wait(3, msec);
    }
}

//
// Main will set up the competition functions and callbacks.
//
int main()
{
    
    
    vex::wait(200, msec); // triports initializing
    vexcodeInit();

    bind_all();
    

    // Set up callbacks for autonomous and driver control periods.
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);

    // Run the pre-autonomous function.
    pre_auton();

    
    // Prevent main from exiting with an infinite loop.
    while (true)
    {
        printf("in main\n");
        vex::wait(100, msec);
    }
}
