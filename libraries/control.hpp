/* Esse arquivo serve para fazer o controle do robo, tem a 
 * funcao pid que eh chamada em outros lugares, as constantes alteraveis do pid 
 */

#ifndef CONTROL_HPP
#define CONTROL_HPP

struct escolhaDeConstantePID
{
  double kp, ki, kd, dt = 0;
  double lastRun, integrativeSum = 0;
  float lastError,derivativeError = 0;
  double output;
};

//Criação das duas instancias para a curva e linear
escolhaDeConstantePID vel_linear, main_loop;

void initConstants()
{
  vel_linear.kp = 0.5;
  vel_linear.ki = 0.17;
  vel_linear.kd = 0.01;
  main_loop.kp = 24;
  main_loop.ki = 0;
  main_loop.kd = 2;
}

// PID = Proportional Integrative Derivative
void PID(float error, escolhaDeConstantePID * constantePID)
{
  constantePID->dt = 1; 
  constantePID->integrativeSum += constantePID->ki * error * constantePID->dt;
  constantePID->derivativeError = constantePID->kd * (error - constantePID->lastError) / constantePID->dt;
  constantePID->lastError = error;
  constantePID->output = constantePID->kp * error + constantePID->integrativeSum + constantePID->derivativeError;
};

#endif