#include "vex.h"
#include "autons.h"

#define sleep(ms) vex::wait(ms, vex::msec)
#define GREEN

vex::brain brain;
vex::controller controller;
vex::competition competition;

vex::motor l1 (vex::PORT10, true);
vex::motor l2 (vex::PORT9, true);
vex::motor l3 (vex::PORT8, true);
vex::motor l4 (vex::PORT7);
vex::motor_group leftDriveMotors (l1, l2, l3, l4);

vex::motor r1 (vex::PORT1);
vex::motor r2 (vex::PORT2);
vex::motor r3 (vex::PORT3);
vex::motor r4 (vex::PORT4, true);
vex::motor_group rightDriveMotors (r1, r2, r3, r4);


vex::motor intake1 (vex::PORT6);
vex::motor intake2 (vex::PORT5, true);
vex::motor intakeTop (vex::PORT11, true);
vex::motor_group intake (intake1, intake2);
vex::motor highGoalScore (vex::PORT12);

vex::pneumatics p1 (brain.ThreeWirePort.A);
vex::pneumatics p2 (brain.ThreeWirePort.B);
vex::pneumatics p3 (brain.ThreeWirePort.C);

vex::inertial imu (vex::PORT19);

Drive chassis(
TANK_TWO_ENCODER,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
leftDriveMotors,

//Right Motors:
rightDriveMotors,

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
vex::PORT19,
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
2,
0, // Vertical Dist

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

    if (abs(throttle) < 3) throttle = 0;
    if (abs(steering) < 3) steering = 0;

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
    cancel1 = true;
    cancel2 = true;
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
    vex::wait(125, vex::msec);
    highGoalScore.stop();
    if (cancel1) return;
    intake.spin(vex::forward, 12, vex::volt);
    intakeTop.spin(vex::reverse, 7, vex::volt);
}

void toggleMatchloader()
{
    p2.set(!p2.value());
}

void toggleOdomRetraction()
{
    p3.set(!p3.value());
}

void toggleHook()
{
    p1.set(!p1.value());
}

bool ranPreAuto = false;

void preautonomous()
{
    imu.calibrate();
    waitUntil(!imu.isCalibrating());
    default_constants();
    chassis.set_coordinates(-24, -48, 90);
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

void driverControl()
{
    leftDriveMotors.setStopping(vex::coast);
    rightDriveMotors.setStopping(vex::coast);
}

void selectAuto()
{
    if (ranPreAuto) return;
    ranPreAuto = true;
    preautonomous();
    competition.autonomous(autonomous1);
}

int main() {
    competition.drivercontrol(driverControl);
    controller.Axis1.changed(updateDrivetrainVelocity);
    controller.Axis3.changed(updateDrivetrainVelocity);
    controller.ButtonL1.pressed(intakeForward);
    controller.ButtonL2.pressed(intakeReverse);
    controller.ButtonL2.released(intakeStop);
    controller.ButtonR1.pressed(scoreHigh);
    controller.ButtonR2.pressed([](){
        p1.set(true);
    });
    controller.ButtonR2.released([](){
        p1.set(false);
    });
    controller.ButtonR1.released(intakeStop);
    controller.ButtonDown.pressed(toggleMatchloader);
    controller.ButtonB.pressed(scoreMiddle);
    controller.ButtonB.released(intakeStop);
    controller.ButtonA.pressed(selectAuto);
    vex::wait(50, vex::msec);
}
