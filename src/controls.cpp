#include "vex.h"
#include "controls.h"
#include "devices.h"

#ifdef CHRIS
void bind_all(controller c){

        c.ButtonL1.pressed([ ] {
        
        intake.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonL2.pressed([ ]{
        intake.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonL2.released([ ]{
        if (!controller1.ButtonL1.pressing()){
            intake.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonR1.pressed([ ]{
        hood.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonY.pressed([ ]{
        hood.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonY.released([ ]{
        if (!controller1.ButtonR1.pressing()){
            hood.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonX.pressed([ ]{
        midGoal.set(!midGoal.value());
    });
    c.ButtonA.pressed([ ]{
        loader.set(!loader.value());
    });
    c.ButtonR2.pressed([ ]{
        wing.set(!wing.value());
    });
     
    c.Axis2.changed([ ]{
        chassis.control_arcade();
    });
     
    c.Axis3.changed([ ]{
        chassis.control_arcade();
    });

}
#endif


//================================================================================================================
//
//================================================================================================================


#ifdef JOSEPH
void bind_all(controller c){

    c.ButtonL1.pressed([ ] {
        
        intake.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonL2.pressed([ ]{
        intake.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonL2.released([ ]{
        if (!controller1.ButtonL1.pressing()){
            intake.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonR1.pressed([ ]{
        hood.spin(vex::fwd, 100/8.3, vex::volt);
    });

    c.ButtonY.pressed([ ]{
        hood.spin(vex::fwd, -100/8.3, vex::volt);
    });

    c.ButtonY.released([ ]{
        if (!controller1.ButtonR1.pressing()){
            hood.spin(vex::fwd, 0, vex::volt);
        }
    });

    c.ButtonX.pressed([ ]{
        midGoal.set(!midGoal.value());
    });
    c.ButtonA.pressed([ ]{
        loader.set(!loader.value());
    });
    c.ButtonR2.pressed([ ]{
        wing.set(!wing.value());
    });
     
    c.Axis2.changed([ ]{
        chassis.control_arcade();
    });
     
    c.Axis3.changed([ ]{
        chassis.control_arcade();
    });
    
}
#endif

#undef CHRIS
#undef JOSEPH