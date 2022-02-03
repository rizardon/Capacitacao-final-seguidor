/* Esse arquivo eh responsavel pelo interacao do teclado com o robo durante 
 * a simulacao
 */

#ifndef KEY_HPP
#define KEY_HPP

#include "definicao.hpp"
#include <iostream>
// #include "C:/Program Files/Webots/include/controller/cpp/webots/webot_time.hpp"
using namespace webots;
using namespace std;

//Definição das teclas do teclado
int r = 82, q = 81, w = 87, shift_q = 65617, shift_w = 65623, a = 65, s = 83, d = 68,
    shift_a = 65601, shift_s = 65619, shift_d = 65604, space = 32, shift_space = 65568, v = 86;

double linear = 4;
static int key;
int timeStep = 1;
double step = 1;
Keyboard kb;

Field *field_translacao = Presto->getField("translation");
Field *field_rotacao = Presto->getField("rotation");
double translacao[3] = {0.328, 0.008, 0};
double rotation[4] = {-0.9673617857081064, -0.16033996448117435, -0.19621995653296762, 0.0182884};
double linear_reference = 0.3;

void initKey()
{
  kb.enable(timeStep);
}

void readKey()
{
  static double time_key = millis();

  key = -1;
  if (millis() - time_key > 80)
  {
    key = kb.getKey();
    time_key = millis();
  }

  if (key == r)
  { //tecla r restarta o controller
    motor_left->setVelocity(0);
    motor_right->setVelocity(0);
    delay(500);
    field_translacao->setSFVec3f(translacao);
    field_rotacao->setSFRotation(rotation);
    Presto->restartController();
  }
  else if (key == q)
  { //tecla p incrementa o kp
    main_loop.kp += step;
    cout << "Kp da velocidade angular: " << main_loop.kp << endl;
  }
  else if (key == shift_q)
  { //tecla shift p decrementa o kp
    main_loop.kp -= step;
    cout << "Kp da velocidade angular: " << main_loop.kp << endl;
  }
  else if (key == w)
  { //tecla w incrementa a velocidade do robo
    main_loop.kd = main_loop.kd + step;
    cout << "Kd da velocidade angular: " << main_loop.kd << endl;
  }
  else if (key == shift_w)
  { //tecla d decrementa a velocidade do robo
    main_loop.kd = main_loop.kd - step;
    cout << "Kd da velocidade angular: " << main_loop.kd << endl;
  }
  else if (key == space)
  { //barra de espaco incrementa a velocidade
    linear += step;
    cout << "linear: " << linear << endl;
  }
  else if (key == shift_space)
  { //shift barra de espaco faz o robo voltar a andar
    linear -= step;
    cout << "linear: " << linear << endl;
  }
  else if (key == a)
  { // tecla a para alterar o kp do vel_linear
    vel_linear.kp += step/10;
    cout << "Kp da velocidade linear: " << vel_linear.kp << endl;
  }
  else if (key == shift_a)
  {
    vel_linear.kp -= step/10;
    cout << "Kp da velocidade linear: " << vel_linear.kp << endl;
  }
  else if (key == s)
  {
    vel_linear.ki += step/100;
    cout << "Ki da velocidade linear: " << vel_linear.ki << endl;
  }
  else if (key == shift_s)
  {
    vel_linear.ki -= step/100;
    cout << "Ki da velocidade linear: " << vel_linear.ki << endl;
  }
  else if (key == d)
  {
    vel_linear.kd += step/100;
    cout << "Kd da velocidade linear: " << vel_linear.kd << endl;
  }
  else if (key == shift_d)
  {
    vel_linear.kd -= step/100;
    cout << "Kd da velocidade linear: " << vel_linear.kd << endl;
  }
  else if (key == v)
  {
    linear_reference += step;
    cout << "linear de referência: " << linear_reference << endl;
  }
  else if (key != -1)
  {
    cout << "key: " << key << endl;
  }
}

#endif