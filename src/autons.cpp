#include "vex.h"
#include "controls.h"
/**
 * Resets the constants for auton movement.
 * Modify these to change the default behavior of functions like
 * drive_distance(). For explanations of the difference between
 * drive, heading, turning, and swinging, as well as the PID and
 * exit conditions, check the docs.
 */

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

/**
 * Sets constants to be more effective for odom movements.
 * For functions like drive_to_point(), it's often better to have
 * a slower max_voltage and greater settle_error than you would otherwise.
 */

void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 10;
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
  chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;
}

/**
 * The expected behavior is to return to the start position.
 */

void drive_test(){
  chassis.drive_distance(6);
  chassis.drive_distance(12);
  chassis.drive_distance(18);
  chassis.drive_distance(-36);
}

/**
 * The expected behavior is to return to the start angle, after making a complete turn.
 */

void turn_test(){
  chassis.turn_to_angle(5);
  chassis.turn_to_angle(30);
  chassis.turn_to_angle(90);
  chassis.turn_to_angle(225);
  chassis.turn_to_angle(0);
}



/**
 * A little of this, a little of that; it should end roughly where it started.
 */

void full_test(){
  chassis.drive_distance(24);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(-36);
  chassis.right_swing_to_angle(-90);
  chassis.drive_distance(24);
  chassis.turn_to_angle(0);
}

/**
 * Doesn't drive the robot, but just prints coordinates to the Brain screen 
 * so you can check if they are accurate to life. Push the robot around and
 * see if the coordinates increase like you'd expect.
 */

void odom_test(){
  chassis.set_coordinates(0, 0, 0);
  while(1){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5,20, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(5,40, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(5,60, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(5,80, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(5,100, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    task::sleep(20);
  }
}

#ifdef CHRIS
void red_auto(){

}

void blue_auto(){

}

void skills(){

}


void autonomous1()
{
    // double startTime = brain.timer(vex::msec);
    //intakeReverse();
    vex::thread([](){
        vex::wait(300, msec);
        //intakeStop();
    });
    chassis.drive_max_voltage = 4.8;
    chassis.drive_to_point(-47.25, -48);
    chassis.drive_max_voltage = 6;
    //toggleMatchloader();
    chassis.turn_to_angle(180);
    //intakeForward();
    chassis.drive_timeout = 1500;
    chassis.drive_max_voltage = 3;
    chassis.drive_distance(9);
    chassis.drive_distance(-1);
    chassis.drive_stop(vex::hold);
    //sleep(800);
    chassis.drive_max_voltage = 4.5;
    vex::thread([](){
        //sleep(100);
        //intakeReverse();
        //sleep(400);
        //intakeForward();
    });
    chassis.drive_distance(-15);
    vex::thread([](){
        //sleep(400);
        //scoreHigh();
    });
    //scoreHigh();
    //sleep(1800);
    //intakeStop();
    //intakeForward();
    chassis.drive_timeout = 3000;
    chassis.drive_max_voltage = 3;
    chassis.drive_distance(15);
    chassis.drive_distance(-1);
    chassis.drive_stop(vex::hold);
    //sleep(2000);
    chassis.drive_max_voltage = 6;
    chassis.drive_to_point(-48, -48);
    //toggleMatchloader();
    chassis.turn_to_point(-9, -9, 180);
    chassis.drive_to_point(-8.7, -8.7);
    // intakeReverse();
    //scoreHigh();
    //sleep(300);
    //scoreMiddle();
    //sleep(3000); // 2800
    //intakeStop();

    //toggleOdomRetraction();

    /*
    chassis.swing_timeout = 750;
    chassis.right_swing_to_angle(135);
    chassis.drive_to_point(1.1, -36); // 0.85
    chassis.turn_to_angle(180);
    toggleOdomRetraction();
    chassis.drive_with_voltage(5, 5);
    sleep(5000);
    */

    chassis.drive_stop(vex::coast);
    
    // double endTime = brain.timer(vex::msec);
    // printf("Autonomous completed in: %.2f seconds\n", (endTime - startTime) / 1000);
}


#endif

#ifdef JOSEPH
void red_auto(){

}

void blue_auto(){
  
}

void skills(){

}




void autonomous1()
{
    // double startTime = brain.timer(vex::msec);
    chassis.drive_distance(13.8);
    toggleMatchloader();
    chassis.turn_to_angle(180);
    intakeForward();
    chassis.drive_timeout = 2000;
    chassis.drive_distance(6);
    sleep(300);
    vex::thread([](){
        sleep(100);
        intakeReverse();
        sleep(800);
        intakeForward();
    });
    chassis.drive_distance(-13);
    vex::thread([](){
        sleep(800);
        scoreHigh();
    });
    scoreHigh();
    sleep(2000);
    chassis.drive_distance(13);
    intakeForward();
    sleep(1200);
    chassis.drive_distance(-5.2);
    chassis.turn_to_angle(-46);
    toggleMatchloader();
    chassis.drive_timeout = 3200;
    chassis.drive_distance(20.5);
    chassis.drive_distance(-1);
    intake.spin(vex::reverse, 9.5, vex::volt);
    intakeTop.spin(vex::reverse, 2, vex::volt);
    highGoalScore.spin(vex::reverse, 12, vex::volt);
    // double endTime = brain.timer(vex::msec);
    // printf("Autonomous completed in: %.2f seconds\n", (endTime - startTime) / 1000);
    chassis.drive_stop(vex::coast);
}
#endif