#include "main.h"
#include "pros/rtos.h"
#define akp .4
#define akd 0
#define akt 2.5 //Degrees of wheel turn to turn base 1 degree
#define akm 28.6478897565 //Degrees of wheel turn to 1 inch

Motor left1 (left1Port, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
Motor left2 (left2Port, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
Motor right1 (right1Port, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
Motor right2 (right2Port, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
Motor intake (intakePort, MOTOR_GEAR_BLUE, true, MOTOR_ENCODER_DEGREES);

bool voltControl = false, targReach = false;
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

        targReach = -15 < errLeft && errLeft < 15 && -15 < errRight && errRight < 15;

        delay(15);
    }
}

void move(double inches, int time = 0){
    targLeft += inches;
    targRight += inches;
    printf("Move: %f\n", inches);

    delay(500);

    if (time){
        delay(time);
    } else {
        while (!targReach) {
            printf("Moving\n");
            delay(20);
        }
    }
}

void turn(double degrees, int time = 0){
    targLeft += degrees;
    targRight -= degrees;
    printf("Turning: %f\n", degrees);

    delay(500);

    if (time){
        delay(time);
    } else {
        while (!targReach) {
            printf("Turning\n");
            delay(20);
        }
    }
}

void calibration(pathEnumT path){
    Task autonPIDTask (autonPID, (void*)"BALLS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");
    
    switch(path){
        case pathEnum_MOVE:
            move(24);
            break;
        case pathEnum_TURN:
            turn(90);
            break;
        case pathEnum_All:
            move(24);
            turn(90);
            break;
    }
}