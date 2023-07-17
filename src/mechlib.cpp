#include "main.h"
#define kp .090

void cataPID(void *ignore){
    Motor cataLeft (cataLeftPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
    Motor cataRight (cataRightPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
    Rotation rotation (rotationPort);
    Controller master (CONTROLLER_MASTER);

    double err;
    while (true) {
        if (master.get_digital_new_press(DIGITAL_R2)){
            cataLeft.move(-127);
            cataRight.move(-127);
            delay(750);
        } else if (master.get_digital(DIGITAL_R1)){
            cataLeft.move(-50);
            cataLeft.move(-50);
        } else {
            err = 0 - rotation.get_position();

            cataLeft.move(err*kp);
            cataRight.move(err*kp);
        }

        delay(15);
    }
}