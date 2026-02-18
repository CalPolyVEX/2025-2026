#include "vex.h"
#include "autons.h"

#define sleep(ms) vex::wait(ms, vex::msec)
#define GOLD


vex::brain brain;
vex::controller controller;
vex::competition competition;

vex::motor l1 (vex::PORT1, true);
vex::motor l2 (vex::PORT2, true);
vex::motor l3 (vex::PORT3);
vex::motor l4 (vex::PORT4, true);
vex::motor_group leftDriveMotors (l1, l2, l3, l4);

vex::motor r1 (vex::PORT6);
vex::motor r2 (vex::PORT7);
vex::motor r3 (vex::PORT8, true);
vex::motor r4 (vex::PORT9);
vex::motor_group rightDriveMotors (r1, r2, r3, r4);


vex::motor intake1 (vex::PORT11);
vex::motor intake2 (vex::PORT19, true);
vex::motor intakeTop (vex::PORT5, true);
vex::motor_group intake (intake1, intake2);
// vex::motor highGoalScore (vex::PORT15);

vex::pneumatics p1 (brain.ThreeWirePort.A);
vex::pneumatics p2 (brain.ThreeWirePort.B);
vex::pneumatics p3 (brain.ThreeWirePort.C);
vex::pneumatics p4 (brain.ThreeWirePort.D);
vex::pneumatics p5 (brain.ThreeWirePort.E);
vex::pneumatics p6 (brain.ThreeWirePort.F);
vex::pneumatics p7 (brain.ThreeWirePort.G);
vex::pneumatics p8 (brain.ThreeWirePort.H);
vex::pneumatics pneus[8] = {p1, p2, p3, p4, p5, p6, p7, p8};

vex::inertial imu (vex::PORT20);

bool flipThrottle = false;
double flipTime = brain.timer(vex::msec);

vex::distance rightDistance (vex::PORT17); // 5.2 inch offset (now 478mm)
vex::distance frontDistance (vex::PORT18); // 5.6 (now 475mm)

Drive chassis(
ZERO_TRACKER_ODOM,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
leftDriveMotors,

//Right Motors:
rightDriveMotors,

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
imu.index(),
8.271246, // 2.75
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
2.0307, // 2.0214

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
0,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
2.0142, // 2.0035 previously

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
0

);

enum Axis {
    X = 0,
    Y
};

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
    // highGoalScore.stop();
    intake.spin(vex::forward, 12, vex::volt);
}

void intakeReverse()
{
    intake.spin(vex::reverse, 12, vex::volt);
    intakeTop.spin(vex::reverse, 2, vex::volt);
    // highGoalScore.spin(vex::reverse, 12, vex::volt);
}

void intakeStop()
{
    intake.stop();
    intakeTop.stop();
    // highGoalScore.stop();
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
    // highGoalScore.spin(vex::forward, 12, vex::volt);
}

void scoreMiddle()
{
    cancel1 = false;
    intake.spin(vex::reverse, 12, vex::volt);
    // highGoalScore.spin(vex::reverse, 12, vex::volt);
    vex::wait(300, vex::msec);
    // highGoalScore.stop();
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
    controller.rumble("-.");
    
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

// void distanceResetQuadrant3(vex::distance distanceSensor, Axis axis, double offset) {
//     double dist = rightDistance.objectDistance(vex::inches);
//     double x = chassis.get_X_position();
//     double y = chassis.get_Y_position();
//     double h = chassis.get_absolute_heading();
//     chassis.odom_task.stop();

//     if (axis == X) chassis.set_coordinates(-72 + dist + 5.4, y, h);
//     else chassis.set_coordinates(x, -72 + dist + offset, h);
// }

void distanceResetQuadrant3(vex::distance distanceSensor, Axis axis) {
    double x = chassis.get_X_position();
    double y = chassis.get_Y_position();
    double h = chassis.get_absolute_heading();
    double distRight = rightDistance.objectDistance(vex::inches);
    double distFront = frontDistance.objectDistance(vex::inches);
    chassis.odom_task.stop();

    if (axis == X) chassis.set_coordinates(-72 + distRight + 5.6, y, h);
    if (axis == Y) chassis.set_coordinates(x, -72 + distFront + 5.2, h);
}

void autonomous1()
{
    double startTime = brain.timer(vex::msec);
    intakeReverse();
    vex::thread([](){
        sleep(100);
        intakeStop();
    });
    chassis.drive_max_voltage = 7;
    chassis.drive_to_point(-48, -48);
    toggleMatchloader();
    chassis.turn_to_angle(180);
    chassis.drive_max_voltage = 4.75;
    distanceResetQuadrant3(rightDistance, X);
    distanceResetQuadrant3(frontDistance, Y);  
    chassis.turn_to_point(-48, -72);
    intakeForward();
    chassis.drive_timeout = 950;
    chassis.drive_to_point(-48, -72);
    chassis.boomerang_lead = 0.85;
    chassis.drive_timeout = 1250;
    chassis.drive_to_pose(-48, -24, 180);
    scoreHigh();
    chassis.turn_to_angle(180);
    distanceResetQuadrant3(rightDistance, X);
    distanceResetQuadrant3(frontDistance, Y); 
    sleep(1250);
    chassis.drive_timeout = 1500;
    chassis.drive_to_point(-48, -72);
    intakeForward();
    // scoreHigh();
    sleep(2000); 
    // intakeStop();
    // intakeForward();
    sleep(1500);
    chassis.drive_stop(vex::hold);
    distanceResetQuadrant3(rightDistance, X);
    distanceResetQuadrant3(frontDistance, Y);
    chassis.drive_to_point(-48, -48);
    chassis.turn_to_angle(225);
    chassis.drive_max_voltage = 5;
    chassis.drive_timeout = 3000;
    chassis.drive_distance(-57.5, -135);
    scoreMiddle();
    sleep(2000);
    toggleMatchloader();
    chassis.drive_distance(43.125, -135);
    chassis.turn_to_angle(0);
    toggleHook();
    chassis.drive_distance(32);
    sleep(75);
    chassis.drive_stop(vex::hold);
    sleep(50);
    chassis.drive_stop(vex::coast);
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
    vex::wait(200, vex::msec);
    competition.drivercontrol(driverControl);
    controller.Axis1.changed(updateDrivetrainVelocity);
    controller.Axis3.changed(updateDrivetrainVelocity);
    controller.ButtonL1.pressed(intakeForward);
    controller.ButtonL2.pressed(intakeReverse);
    controller.ButtonL2.released(intakeStop);
    controller.ButtonR1.pressed(scoreHigh);
    controller.ButtonR2.pressed(scoreMiddle);
    controller.ButtonR1.released(intakeStop);
    controller.ButtonR2.released(intakeStop); 
    controller.ButtonX.pressed(toggleOdomRetraction);
    controller.ButtonDown.pressed(toggleMatchloader);
    controller.ButtonB.pressed([](){
        p3.set(!p3.value());
    });
    controller.ButtonA.pressed(selectAuto);
    // controller.ButtonY.pressed(checkFlip);
    // controller.ButtonUp.pressed(checkFlip);
}

