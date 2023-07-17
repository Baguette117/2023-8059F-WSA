#include "main.h"
#define akp .4
#define akd 0

Motor left1 (left1Port, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
Motor left2 (left2Port, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
Motor right1 (right1Port, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
Motor right2 (right2Port, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
Motor intake (intakePort, MOTOR_GEAR_BLUE, true, MOTOR_ENCODER_DEGREES);

bool voltControl = false;
double targLeft = 0, targRight = 0, errLeft, errRight, prevErrLeft = 0, prevErrRight = 0, derivLeft, derivRight;

void autonPID(void *ignore){
    left1.tare_position();
    right1.tare_position();

    while (true) {
        if (!voltControl){
            errLeft = targLeft - left1.get_position();
            errRight = targRight - right1.get_position();

            derivLeft = prevErrLeft - errLeft;
            derivRight = prevErrRight - errRight;

            left1.move(errLeft*akp + derivLeft*akd);
            left2.move(errLeft*akp + derivLeft*akd);
            right1.move(errRight*akp + derivRight*akd);
            right2.move(errRight*akp + derivRight*akd);

            prevErrLeft = errLeft;
            prevErrRight = errRight;
        } else {
            delay(5);
        }

        delay(15);
    }
}