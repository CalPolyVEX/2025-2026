#include "vex.h"
#include "controls.h"

void bind_all(controller c){
    c.ButtonA.pressed(conveyorEnable);
    c.ButtonA.released(conveyorDisable);
}