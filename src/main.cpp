#include "vex.h"
#include "devices.h"
#include "chassis.h"
//#include <vex_task.h>
#include "controls.h"
#include "colors.h"
#include "autons.h"

//using namespace vex;
vex::competition Competition;

#define sleep(ms) vex::wait(ms, vex::msec)

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
 * Function before autonomous. Initializes the imu and prints coords.
 */
void preautonomous()
{
    vex::inertial imu = chassis.Gyro;
    imu.calibrate();
    waitUntil(!imu.isCalibrating());
    default_constants();
    chassis.set_coordinates(-24, -48, -90);
    c.rumble("--");
    
    vex::thread t ([]() {
        while (1)
        {
            Brain.Screen.clearLine(5);
            Brain.Screen.clearLine(12);
            Brain.Screen.setCursor(5, 1);
            Brain.Screen.print("(%.4f, %.4f)", chassis.get_X_position(), chassis.get_Y_position());
            Brain.Screen.setCursor(6, 1);
            Brain.Screen.print("%.2f", chassis.get_absolute_heading());
            sleep(200);
        }
    });
}


bool ranPreAuto = false;

void selectAuto()
{
    if (ranPreAuto) return;
    ranPreAuto = true;
    preautonomous();
    Competition.autonomous(autonomous1);
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
    leftDrive.setStopping(vex::coast);
    rightDrive.setStopping(vex::coast);
    
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
    default_constants();

    
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




