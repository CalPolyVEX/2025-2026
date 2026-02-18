#include "vex.h"
#include "autons.h"

#define sleep(ms) vex::wait(ms, vex::msec)
#define GREEN

vex::brain brain;
vex::controller controller;
vex::competition competition;

vex::motor l1 (vex::PORT11);
vex::motor l2 (vex::PORT12, true);
vex::motor l3 (vex::PORT13, true);
vex::motor l4 (vex::PORT14, true);
vex::motor_group leftDriveMotors (l1, l2, l3, l4);

vex::motor r1 (vex::PORT16);
vex::motor r2 (vex::PORT7);
vex::motor r3 (vex::PORT19);
vex::motor r4 (vex::PORT20, true);
vex::motor_group rightDriveMotors (r1, r2, r3, r4);

vex::motor intake1 (vex::PORT15);
vex::motor intake2 (vex::PORT8, true);
vex::motor intakeTop (vex::PORT5);
vex::motor_group intake (intake1, intake2);
vex::motor highGoalScore (vex::PORT10);

vex::pneumatics p1 (brain.ThreeWirePort.A);
vex::pneumatics p2 (brain.ThreeWirePort.B);
vex::pneumatics p3 (brain.ThreeWirePort.C);

vex::optical opti (vex::PORT2);
vex::distance dist (vex::PORT9);

vex::inertial imu (vex::PORT4);

Drive chassis(
TANK_TWO_ENCODER,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
leftDriveMotors,

//Right Motors:
rightDriveMotors,

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
imu.index(),
2.75,
0.75,
360, // 360

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
2.0025,
0, // Vertical Dist

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
2.01,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
0

);


bool cancel1 = false;
bool cancel2 = false;

void updateDrivetrainVelocity()
{
    int throttle = controller.Axis3.position();
    int steering = controller.Axis1.position();

    if (abs(throttle) < 3) throttle = 0;
    if (abs(steering) < 3) steering = 0;

    leftDriveMotors.spin(vex::forward, (throttle + steering) * 120, vex::voltageUnits::mV);
    rightDriveMotors.spin(vex::forward, (throttle - steering) * 120, vex::voltageUnits::mV);
}

void intakeForward()
{
    cancel1 = true;
    cancel2 = true;
    intakeTop.spin(vex::forward, 1.5, vex::volt);
    highGoalScore.spin(vex::reverse, 1.5, vex::volt);
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
    p3.set(!p3.value());
}

void toggleOdomRetraction()
{
    p2.set(!p2.value());
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
    chassis.set_coordinates(24, -48, 90);
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

void autonomous1()
{
    double startTime = brain.timer(vex::msec);
    // intakeReverse();
    // vex::thread([](){
    //     sleep(300);
    //     intakeStop();
    // });
    
    chassis.drive_max_voltage = 4.8;
    chassis.drive_to_point(47.25, -48);
    chassis.drive_max_voltage = 6;
    toggleMatchloader();
    chassis.turn_to_point(48, -72);
    intakeForward();
    chassis.drive_timeout = 1500;
    chassis.drive_max_voltage = 10;
    chassis.drive_timeout = 700;
    chassis.drive_to_point(48, -60);
    chassis.drive_stop(vex::hold);
    chassis.drive_with_voltage(6, 6);
    sleep(100); // 800
    chassis.drive_max_voltage = 4.5;
    chassis.drive_timeout = 1500;
    
    vex::thread([](){
        sleep(700);
        toggleMatchloader();
        // intakeReverse(); 
        sleep(100);
        intakeForward();
    });
    // chassis.drive_distance(-15);
    double tempTurnTimeout = chassis.turn_timeout;
    chassis.turn_timeout = 400;
    chassis.turn_to_point(48, -24, 180);
    chassis.turn_timeout = tempTurnTimeout;
    chassis.drive_to_point(48, -24);
    vex::thread([](){
        sleep(500);
        scoreHigh();
    });
    scoreHigh();
    sleep(1800);
    intakeStop();
    intakeForward();
    chassis.drive_timeout = 3000;
    chassis.drive_max_voltage = 6;
    chassis.turn_to_point(48, -72);
    toggleMatchloader(); // Skills
    intakeReverse();
    vex::thread([](){
        sleep(400); // 400
        cancel2 = false;
        intake.spin(vex::reverse, 12, vex::volt);
        intakeTop.spin(vex::reverse, 12, vex::volt);
        vex::wait(100, vex::msec);
        if (cancel2) return;
        intake.spin(vex::forward, 12, vex::volt);
        intakeTop.spin(vex::forward, 4, vex::volt);
        highGoalScore.spin(vex::forward, 4, vex::volt);
    });
    chassis.drive_to_point(48, -60);
    // scoreHigh();
    // chassis.drive_distance(15);
    chassis.drive_with_voltage(6, 6);
    vex::thread([](){
        sleep(800);
        intakeForward();
    });
    sleep(4000);
    chassis.drive_stop(vex::hold);
    chassis.drive_max_voltage = 6;
    chassis.drive_to_point(48, -48);
    toggleMatchloader();
    chassis.turn_to_point(13, -13);
    double original = chassis.heading_max_voltage;
    chassis.heading_max_voltage = 11;

    chassis.drive_settle_error += 0.8;
    chassis.drive_timeout = 4200;
    vex::thread([](){
        sleep(1100);
        intake.spin(vex::reverse, 9, vex::volt);
        intakeTop.spin(vex::reverse, 2, vex::volt);
        highGoalScore.spin(vex::reverse, 12, vex::volt);
    });
    chassis.drive_to_point(12.3, -12.9); // 8.7
    // chassis.turn_to_point(13, -13);
    chassis.heading_max_voltage = original;
    intake.spin(vex::reverse, 7, vex::volt);
    intakeTop.spin(vex::reverse, 2, vex::volt);
    highGoalScore.spin(vex::reverse, 12, vex::volt);
    intakeTop.spin(vex::reverse, 1, vex::volt);
    sleep(3000); // 2800
    intakeStop();

    // chassis.drive_to_point(22, -24);
    // chassis.turn_to_point(0, chassis.get_Y_position());
    // chassis.drive_to_point(0, chassis.get_Y_position());
    // toggleOdomRetraction();
    // chassis.turn_to_angle(180);

    // chassis.drive_with_voltage(5, 5);
    // vex::wait(600, vex::msec);
    // chassis.drive_with_voltage(9, 9);
    // vex::wait(4000, vex::msec);

    chassis.drive_to_point(37.2, -33);
    chassis.turn_to_angle(180);
    toggleHook();
    leftDriveMotors.stop(vex::hold);
    rightDriveMotors.stop(vex::hold);
    chassis.drive_stop(vex::hold);
    vex::wait(50, vex::msec);
    leftDriveMotors.spin(vex::reverse, 5, vex::volt);
    rightDriveMotors.spin(vex::reverse, 5, vex::volt);
    waitUntil(chassis.get_Y_position() > -12);
    rightDriveMotors.spin(vex::fwd, -2, vex::volt);


    vex::wait(75, vex::msec);
    chassis.drive_stop(vex::hold);
    leftDriveMotors.stop(vex::hold);
    rightDriveMotors.stop(vex::hold);
    chassis.drive_stop(vex::coast);

    
    double endTime = brain.timer(vex::msec);
    printf("Autonomous completed in: %.2f seconds\n", (endTime - startTime) / 1000);
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
    vex::wait(200, vex::msec);
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
    controller.ButtonX.pressed(toggleOdomRetraction);
}
