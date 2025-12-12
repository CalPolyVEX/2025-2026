#include "vex.h"
#include "devices.h"
#include "chassis.h"
#include <vex_task.h>
#include "controls.h"
#include "colors.h"
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
        
        Brain.Screen.setPenColor(white);
        Brain.Screen.setFillColor(black);
        //Brain.Screen.printAt(5, 20, "JAR Template v1.2.0");
        Brain.Screen.printAt(5, 20, "Battery Percentage: %d", Brain.Battery.capacity());
        //Brain.Screen.printAt(5, 40, "%d", Brain.Battery.capacity());
        Brain.Screen.printAt(5, 40, "Chassis Heading Reading:");
        Brain.Screen.printAt(5, 60, "%.2f", chassis.get_absolute_heading());
        Brain.Screen.printAt(5, 80, "Selected Auton:");
        //Brain.Screen.printAt(5, 130, "Horizontal: %.2f, Vertical: %.2f", chassis.get_SidewaysTracker_position(), chassis.get_ForwardTracker_position());
        //Brain.Screen.printAt(5, 150, "x: %.2f, y: %.2f, h: %.2f", chassis.get_X_position(), chassis.get_Y_position(), chassis.get_absolute_heading());
        
        
        
        switch (current_auton_selection)
        {
        case 0:
            // RED
            Brain.Screen.printAt(5, 100, "Red Auton");
            Brain.Screen.setPenColor(red);
            Brain.Screen.setFillColor(red);
            break;
        case 1:
            // BLUE
            Brain.Screen.printAt(5, 100, "Blue Auton");
            Brain.Screen.setPenColor(blue);
            Brain.Screen.setFillColor(blue);
            break;
        case 2:
            Brain.Screen.printAt(5, 100, "Skills");
            break;
        case 3:
            Brain.Screen.printAt(5, 100, "Auton 4");
            break;
        case 4:
            Brain.Screen.printAt(5, 100, "Auton 5");
            break;
        case 5:
            Brain.Screen.printAt(5, 100, "Auton 6");
            break;
        case 6:
            Brain.Screen.printAt(5, 100, "Auton 7");
            break;
        case 7:
            Brain.Screen.printAt(5, 100, "Auton 8");
            break;
        }
        if (Brain.Screen.pressing())
        {
            while (Brain.Screen.pressing()){

            }
            current_auton_selection++;
        } else if (current_auton_selection == 8) {
            current_auton_selection = 0;
        }
        
        Brain.Screen.drawCircle(380, 60, 40);

        wait(200, msec);
        Brain.Screen.clearScreen();
        
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
        red_auto();
        break;
    case 1:
        blue_auto();
        break;
    case 2:
        skills();
        break;
    case 3:
        //swing_test();
        break;
    case 4:
        full_test();
        break;
    case 5:
        odom_test();
        break;
    case 6:
        //tank_odom_test();
        break;
    case 7:
        //holonomic_odom_test();
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


void color_sort(vex::color reject, bool reject_high){
    if (!sort.isNearObject()) return;
    double hue = sort.hue();
    vex::color seen;
    if (hue < 240 && hue > 220){ // blue
        seen = vex::color::blue;
    } 
    if (hue > 0 && hue < 25){
        seen = vex::color::red;
    }
    if (seen == reject){
        if (reject_high){
            roller.spin(vex::fwd, -100/8.3, vex::volt);
            #ifdef JOSEPH
                hood.spin(vex::fwd, -100/8.3, vex::volt);
            #endif
            wait(200, msec);
        } else {
            roller.spin(vex::fwd, 100/8.3, vex::volt);
        }
        wait(200, msec);

        if (reject_high){
            roller.spin(vex::fwd, 100/8.3, vex::volt);
        } else {
            roller.spin(vex::fwd, -100/8.3, vex::volt);
        }
        
        #ifdef JOSEPH
            if (reject_high){
                hood.spin(vex::fwd, 0.0, vex::volt);
            } else {
                hood.spin(vex::fwd, 100.0/8.3, vex::volt);
            }
            
        #endif
    }
}


void usercontrol(void) {
  // User control code here, inside the loop
    

    while (1){
        
        
        //printf("driving\n");
        Brain.Screen.printAt(20, 10, "(%.2f, %.2f)", chassis.get_X_position(), chassis.get_Y_position());
        Brain.Screen.printAt(20, 30, "Heading: %.2f", chassis.get_absolute_heading());

        
        vex::wait(30, msec); // jsut so this doesnt exit
    }
}



//
// Main will set up the competition functions and callbacks.
//
int main()
{
    alliance = vex::red;
    
    vex::wait(200, msec); // triports initializing
    vexcodeInit();

    sort.setLightPower(100);
    sort.setLight(vex::ledState::on);
    sort.integrationTime(5.0);

    bind_all();
    chassis.Gyro.calibrate();
    while(chassis.Gyro.isCalibrating());
    c.rumble("--");
    chassis.set_coordinates(0.0, 0.0, 0.0);

    
    if (alliance == vex::red){
        opponent = vex::blue;
    } else {
        opponent = vex::red;
    }

    vex::thread([ ] {
        while (1) {
            color_sort(opponent, !scoring_high);
            //printf("you suck\n");
            wait(20, msec);
        }
    });
    

    // Set up callbacks for autonomous and driver control periods.
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);

    // Run the pre-autonomous function.
    //pre_auton();
    
    
    // Prevent main from exiting with an infinite loop.
    while (true)
    {
        //printf("in main\n");
        vex::wait(100, msec);
    }
}




