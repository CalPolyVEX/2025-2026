#include "vex.h"
#include "controls.h"
#include "devices.h"

vex::controller c(vex::controllerType::primary);

#ifdef CHRIS
void bind_all(){

    c.ButtonL1.pressed([ ] {
        
        intake.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonL2.pressed([ ]{
        intake.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonL2.released([ ]{
        if (!c.ButtonL1.pressing()){
            intake.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonR1.pressed([ ]{
        lift.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonY.pressed([ ]{
        lift.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonY.released([ ]{
        if (!c.ButtonR1.pressing()){
            lift.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonX.pressed([ ]{
        //printf("cb works\n");
        midGoal.set(!midGoal.value());
    });
    c.ButtonA.pressed([ ]{
        loader.set(!loader.value());
    });
    c.ButtonR2.pressed([ ]{
        wing.set(!wing.value());
    });
     
    vex::thread chassis_thread = vex::thread([ ]{
        while (1){
            //printf("bound task\n");
            float throttle = deadband(c.Axis3.value(), 10);
            float turn = deadband(c.Axis1.value(), 10);
            //printf("L pwr: %.2f, R pwr: %.2f\n", to_volt(throttle + turn), to_volt(throttle - turn));
            leftDrive.spin(fwd, to_volt(throttle + turn), vex::volt);
            rightDrive.spin(fwd, to_volt(throttle - turn), vex::volt);
            //printf("L volt: %.2f, R volt: %.2f", chassis.DriveL.voltage(), chassis.DriveR.voltage());
            vex::wait(3, msec);
        }
    });
    
    
}
#endif


//================================================================================================================
//
//================================================================================================================


#ifdef JOSEPH
void bind_all(){

    c.ButtonL1.pressed([ ] {
        
        intake.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonL2.pressed([ ]{
        intake.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonL2.released([ ]{
        if (!c.ButtonL1.pressing()){
            intake.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonR1.pressed([ ]{
        lift.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonY.pressed([ ]{
        lift.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonY.released([ ]{
        if (!c.ButtonR1.pressing()){
            lift.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonX.pressed([ ]{
        //printf("cb works\n");
        midGoal.set(!midGoal.value());
    });
    c.ButtonA.pressed([ ]{
        loader.set(!loader.value());
    });
    c.ButtonR2.pressed([ ]{
        wing.set(!wing.value());
    });
     
    vex::thread chassis_thread = vex::thread([ ]{
        while (1){
            //printf("bound task\n");
            float throttle = deadband(c.Axis3.value(), 10);
            float turn = deadband(c.Axis1.value(), 10);
            //printf("L pwr: %.2f, R pwr: %.2f\n", to_volt(throttle + turn), to_volt(throttle - turn));
            leftDrive.spin(fwd, to_volt(throttle + turn), vex::volt);
            rightDrive.spin(fwd, to_volt(throttle - turn), vex::volt);
            //printf("L volt: %.2f, R volt: %.2f", chassis.DriveL.voltage(), chassis.DriveR.voltage());
            vex::wait(3, msec);
        }
    });
    
    
}
#endif




