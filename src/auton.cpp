#include "main.h"
#define akp 1.2
#define akd 2.4
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

        targReach = -7.5 < errLeft && errLeft < 7.5 && -7.5 < errRight && errRight < 7.5;

        printf("posLeft: %.2f posRight: %f targLeft: %f targRight: %f, errLeft: %f, errRight: %f\n", left1.get_position(), right1.get_position(), targLeft, targRight, errLeft, errRight);
        delay(15);
    }
}

void move(double inches, int time = 0){
    targLeft += inches*akm;
    targRight += inches*akm;
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

    delay(1000);
}

void turn(double degrees, int time = 0){
    targLeft += degrees*akt;
    targRight -= degrees*akt;
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

    delay(1000);
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

    autonPIDTask.remove();
}

void path1(){
    Task autonPIDTask (autonPID, (void*)"BALLS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");
    
    move(-30, 3);
    move(5, 2);
    move(-30, 3);

    autonPIDTask.remove();
}
