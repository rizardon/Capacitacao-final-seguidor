/* Define as variáveis usadas no código
 */

#ifndef DEFINICAO_HPP
#define DEFINICAO_HPP
 
#include "C:/Program Files/Webots/include/controller/cpp/webots/Robot.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Motor.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Supervisor.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Keyboard.hpp"
#include "../../libraries/irsensors.hpp"
#include "../../libraries/encoder.hpp"
#include "../../libraries/control.hpp"
#include "webots_time.hpp"

#define COMPRIMENTO_PISTA 1150

//definição do robot
Supervisor *robot = new Supervisor();

// definição os encoders
string name_sensor[2] = {"encoder_left", "encoder_right"};
Encoder encoders = Encoder(robot, name_sensor);

// definição dos nomes dos sensores ir
string ir_right[1] = {"ir_right"};
string ir_left_right[1] = {"ir_left_right"};
string ir_left_left[1] = {"ir_left_left"};
string ir_frontal[8] = {"ir0", "ir1", "ir2", "ir3", "ir4", "ir5", "ir6", "ir7"}; 

// definição dos objetos de sensores.
IrSensor right_sensor = IrSensor(1, ir_right, robot);
IrSensor left_right_sensor = IrSensor(1, ir_left_right, robot);
IrSensor left_left_sensor = IrSensor(1, ir_left_left, robot);
IrSensor frontal_sensors = IrSensor(8, ir_frontal, robot);

Motor *motor_left = robot->getMotor("motor_left");
Motor *motor_right = robot->getMotor("motor_right");
Node *Presto = robot->getFromDef("Presto");

// definição dos objetos do mapeamento
int mapping_path = 0;


#endif 