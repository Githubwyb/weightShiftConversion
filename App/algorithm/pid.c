#include "pid.h"

pid_struct motor_pwm;

void PID_param_Init(void)
{
    PID_param_Reset(&motor_pwm);

    motor_pwm.kp = 200;
    motor_pwm.ki = 10;
    motor_pwm.kd = 1000;
    motor_pwm.iLimit = 20;
    motor_pwm.oLimit = 800;
}

/**
*@breif:pid计算
*@param desired:期望
*@param measured:实际测量值
*/
double PID_Compulate(pid_struct* pid, double desired, double measured)
{
    pid->error = desired - measured;
    pid->integ += (pid->error * 0.001f);                 /*integ update*/

    if (pid->integ > pid->iLimit)                            /*limit the integ*/
        pid->integ = pid->iLimit;
    else if (pid->integ < -pid->iLimit)
        pid->integ = -pid->iLimit;

    pid->deriv = pid->error - pid->lastError;

    pid->output = pid->kp * pid->error + pid->ki * pid->integ + pid->kd * pid->deriv;

    if (pid->output > pid->oLimit)                           /*limit the output*/
        pid->output = pid->oLimit;
    else if (pid->output < -pid->oLimit)
        pid->output = -pid->oLimit;

    pid->lastError = pid->error;

    return pid->output;
}


/*****************************************************************************/
/**
*@breif:复位pid参数，将pid参数全部置为0
*/
void PID_param_Reset(pid_struct* pid)
{
    pid->error = 0;
    pid->lastError = 0;
    pid->integ = 0;
    pid->deriv = 0;
    pid->iLimit = 0;
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->output = 0;
    pid->oLimit = 0;
}
