def get_JAR_Template_chassis_string(
        left_group_string, right_group_string,
        inertial, sideways, forward, drive_setup,
        drive_dia, drive_gear_ratio, 
        sideways_ctr_dist, forward_ctr_dist, 
        forward_dia, sideways_dia):
    s = f"""Drive chassis(

    // Pick your drive setup from the list below:
    // ZERO_TRACKER_NO_ODOM
    // ZERO_TRACKER_ODOM
    // TANK_ONE_FORWARD_ENCODER
    // TANK_ONE_FORWARD_ROTATION
    // TANK_ONE_SIDEWAYS_ENCODER
    // TANK_ONE_SIDEWAYS_ROTATION
    // TANK_TWO_ENCODER
    // TANK_TWO_ROTATION
    // HOLONOMIC_TWO_ENCODER
    // HOLONOMIC_TWO_ROTATION
    // HOLONOMIC_TWO_QUADRATURE
    // TANK_TWO_QUADRATURE
    // Write it here:
    {drive_setup},

    // Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
    // You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

    // Left Motors:
    {left_group_string},

    // Right Motors:
    {right_group_string},

    // Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
    PORT{inertial}, 

    // Input your wheel diameter. (4" omnis are actually closer to 4.125"):
    {drive_dia},

    // External ratio, must be in decimal, in the format of input teeth/output teeth.
    // If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
    // If the motor drives the wheel directly, this value is 1:
    {drive_gear_ratio},

    // Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
    // For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
    360,

    /*---------------------------------------------------------------------------*/
    /*                                  PAUSE!                                   */
    /*                                                                           */
    /*  The rest of the drive constructor is for robots using POSITION TRACKING. */
    /*  If you are not using position tracking, leave the rest of the values as  */
    /*  they are.                                                                */
    /*---------------------------------------------------------------------------*/

    // If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

    // FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
    // LF:      //RF:
    PORT1, -PORT2, //! IRRELEVANT

    // LB:      //RB:
    PORT3, -PORT4, //! IRRELEVANT

    // If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
    // If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
    // If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
    {pin_to_int(forward)}, 

    // Input the Forward Tracker diameter (reverse it to make the direction switch):
    {forward_dia},

    // Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
    // For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
    // This distance is in inches:
    {forward_ctr_dist},

    // Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
    {pin_to_int(sideways)},

    // Sideways tracker diameter (reverse to make the direction switch):
    {sideways_dia},

    // Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
    {sideways_ctr_dist}

);"""

    return s

def pin_to_int(a: str) -> int:
    assert len(a) == 1, "Input must be a single character"
    A = a.upper()
    assert A in 'ABCDEFGH', "Input must be a letter from a to h"
    return ord(A) - ord('A') + 1

