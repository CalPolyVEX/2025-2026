#include "vex.h"
#include "autons.h"

#define sleep(ms) vex::wait(ms, vex::msec)
#define GOLD


vex::brain brain;
vex::controller controller;
vex::competition competition;

vex::motor l1 (vex::PORT2, true);
vex::motor l2 (vex::PORT10, true);
vex::motor l3 (vex::PORT6, true);
vex::motor l4 (vex::PORT8);
vex::motor_group leftDriveMotors (l1, l2, l3, l4);

vex::motor r1 (vex::PORT1);
vex::motor r2 (vex::PORT3);
vex::motor r3 (vex::PORT5);
vex::motor r4 (vex::PORT4, true);
vex::motor_group rightDriveMotors (r1, r2, r3, r4);


vex::motor intake1 (vex::PORT20);
vex::motor intake2 (vex::PORT11, true);
vex::motor intakeTop (vex::PORT13, true);
vex::motor_group intake (intake1, intake2);
vex::motor highGoalScore (vex::PORT15);

vex::pneumatics p1 (brain.ThreeWirePort.A);
vex::pneumatics p2 (brain.ThreeWirePort.B);
vex::pneumatics p3 (brain.ThreeWirePort.C);
vex::pneumatics p4 (brain.ThreeWirePort.D);
vex::pneumatics p5 (brain.ThreeWirePort.E);
vex::pneumatics p6 (brain.ThreeWirePort.F);
vex::pneumatics p7 (brain.ThreeWirePort.G);
vex::pneumatics p8 (brain.ThreeWirePort.H);
vex::pneumatics pneus[8] = {p1, p2, p3, p4, p5, p6, p7, p8};

vex::inertial imu (vex::PORT12);

bool flipThrottle = false;
double flipTime = brain.timer(vex::msec);

Drive chassis(
TANK_TWO_ENCODER,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
leftDriveMotors,

//Right Motors:
rightDriveMotors,

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
vex::PORT12,
2.75,
0.75,
360,

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
vex::PORT22,     vex::PORT22,

//LB:      //RB: 
vex::PORT22,     vex::PORT22,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
7,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.0143,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
0,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
2.0035,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
0

);


bool cancel1 = false;
bool cancel2 = false;

void updateDrivetrainVelocity()
{
    int throttle = controller.Axis3.position();
    int steering = controller.Axis1.position();

    // #ifdef GOLD
    //     throttle = -throttle;
    // #endif

    if (flipThrottle) throttle = -throttle;

    if (abs(throttle) < 8) throttle = 0;
    if (abs(steering) < 8) steering = 0;

    leftDriveMotors.spin(vex::forward, (throttle + steering) * 120, vex::voltageUnits::mV);
    rightDriveMotors.spin(vex::forward, (throttle - steering) * 120, vex::voltageUnits::mV);
}

void intakeForward()
{
    cancel1 = true;
    cancel2 = true;
    intakeTop.stop();
    highGoalScore.stop();
    intake.spin(vex::forward, 12, vex::volt);
}

void intakeReverse()
{
    intake.spin(vex::reverse, 12, vex::volt);
    intakeTop.spin(vex::reverse, 2, vex::volt);
    highGoalScore.spin(vex::reverse, 12, vex::volt);
}

void intakeStop()
{
    intake.stop();
    intakeTop.stop();
    highGoalScore.stop();
}

void scoreHigh()
{
    cancel2 = false;
    intake.spin(vex::reverse, 12, vex::volt);
    intakeTop.spin(vex::reverse, 12, vex::volt);
    vex::wait(100, vex::msec);
    if (cancel2) return;
    intake.spin(vex::forward, 12, vex::volt);
    intakeTop.spin(vex::forward, 12, vex::volt);
    highGoalScore.spin(vex::forward, 12, vex::volt);
}

void scoreMiddle()
{
    cancel1 = false;
    intake.spin(vex::reverse, 12, vex::volt);
    highGoalScore.spin(vex::reverse, 12, vex::volt);
    vex::wait(300, vex::msec);
    highGoalScore.stop();
    if (cancel1) return;
    intake.spin(vex::forward, 12, vex::volt);
    intakeTop.spin(vex::reverse, 7, vex::volt);
}

void toggleMatchloader()
{
    p1.set(!p1.value());
}

void toggleOdomRetraction()
{
    p2.set(!p2.value());
}

void toggleHook()
{
    p3.set(!p3.value());
}

void preautonomous()
{
    imu.calibrate();
    waitUntil(!imu.isCalibrating());
    default_constants();
    chassis.set_coordinates(-24, -48, -90);
    controller.rumble("--");
    
    vex::thread t ([]() {
        while (1)
        {
            brain.Screen.clearLine(5);
            brain.Screen.clearLine(12);
            brain.Screen.setCursor(5, 1);
            brain.Screen.print("(%.4f, %.4f)", chassis.get_X_position(), chassis.get_Y_position());
            brain.Screen.setCursor(6, 1);
            brain.Screen.print("%.2f", chassis.get_absolute_heading());
            sleep(200);
        }
    });
}

void autonomous1()
{
    // double startTime = brain.timer(vex::msec);
    intakeReverse();
    vex::thread([](){
        sleep(300);
        intakeStop();
    });
    chassis.drive_max_voltage = 4.8;
    chassis.drive_to_point(-47.25, -48);
    chassis.drive_max_voltage = 6;
    toggleMatchloader();
    chassis.turn_to_angle(180);
    intakeForward();
    chassis.drive_timeout = 1500;
    chassis.drive_max_voltage = 3;
    chassis.drive_distance(9);
    chassis.drive_distance(-1);
    chassis.drive_stop(vex::hold);
    sleep(800);
    chassis.drive_max_voltage = 4.5;
    vex::thread([](){
        sleep(100);
        intakeReverse();
        sleep(400);
        intakeForward();
    });
    chassis.drive_distance(-15);
    vex::thread([](){
        sleep(400);
        scoreHigh();
    });
    scoreHigh();
    sleep(1800);
    intakeStop();
    intakeForward();
    chassis.drive_timeout = 3000;
    chassis.drive_max_voltage = 3;
    chassis.drive_distance(15);
    chassis.drive_distance(-1);
    chassis.drive_stop(vex::hold);
    sleep(2000);
    chassis.drive_max_voltage = 6;
    chassis.drive_to_point(-48, -48);
    toggleMatchloader();
    chassis.turn_to_point(-9, -9, 180);
    chassis.drive_to_point(-8.7, -8.7);
    // intakeReverse();
    scoreHigh();
    sleep(300);
    scoreMiddle();
    sleep(3000); // 2800
    intakeStop();

    toggleOdomRetraction();

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

// typedef void *callback(void);

bool ranPreAuto = false;

void selectAuto()
{
    if (ranPreAuto) return;
    ranPreAuto = true;
    preautonomous();
    competition.autonomous(autonomous1);
}

void driverControl()
{
    leftDriveMotors.setStopping(vex::coast);
    rightDriveMotors.setStopping(vex::coast);
}

void checkFlip()
{
    double curTime = brain.timer(vex::msec);
    if ((curTime - flipTime) < 500) return;
    if (controller.ButtonUp.pressing() && controller.ButtonY.pressing())
    {
        flipThrottle = !flipThrottle;
        flipTime = curTime;
    };
}

int main()
{
    competition.drivercontrol(driverControl);
    controller.Axis1.changed(updateDrivetrainVelocity);
    controller.Axis3.changed(updateDrivetrainVelocity);
    controller.ButtonL1.pressed(intakeForward);
    controller.ButtonL2.pressed(intakeReverse);
    controller.ButtonL2.released(intakeStop);
    controller.ButtonR1.pressed(scoreHigh);
    controller.ButtonB.pressed(scoreMiddle);
    controller.ButtonR1.released(intakeStop);
    controller.ButtonB.released(intakeStop); 
    controller.ButtonX.pressed(toggleOdomRetraction);
    controller.ButtonDown.pressed(toggleMatchloader);
    controller.ButtonR2.pressed([](){
        p3.set(!p3.value());
    });
    // controller.ButtonR2.released([](){
    //     p3.set(false);
    // });
    controller.ButtonA.pressed(selectAuto);
    controller.ButtonY.pressed(checkFlip);
    controller.ButtonUp.pressed(checkFlip);
    vex::wait(50, vex::msec);
}

