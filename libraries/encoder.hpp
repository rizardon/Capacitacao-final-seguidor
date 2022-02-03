/* Encoder é um sensor o qual consegue identificar o quanto o robô andou.
 * A partir dessa distância e sabendo o tempo é possível calcular a 
 * velocidade e aceleração do robô, por exemplo.
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP

#include "C:/Program Files/Webots/include/controller/cpp/webots/PositionSensor.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Robot.hpp"
#include <iostream>

using namespace webots;
using namespace std;

#define RAIO 1.25 // o raio está em centímetros. O raio da roda do Presto no Webots é 1.2
class Encoder
{
    public:
    // a definição é feita antes para que os sensores possam ser utilizados em todos os métodos
    PositionSensor *position_sensor_left;   // define o sensor esquerdo
    PositionSensor *position_sensor_right;  // define o sensor direito
    Robot* robot;
    Encoder(Robot* _robot, string name_sensor[2])
    {
        // definição dos sensores de posição / encoders
        robot = _robot;
        position_sensor_left = robot->getPositionSensor(name_sensor[0]);
        position_sensor_right = robot->getPositionSensor(name_sensor[1]);
        position_sensor_left->enable(TIMESTEP);
        position_sensor_right->enable(TIMESTEP);
    }

    double getVelocity()
    {  
        double val_sensor_right = getValueRight();  // define o valor do sensor direito
        double val_sensor_left = getValueLeft();   // define o valor do sensor esquerdo

        double time = robot->getTime(); 
        double space = (val_sensor_left + val_sensor_right)/2;  // define o espaço percorrido como media do valor dos dois sensores 

        // static faz inicializar a variável apenas uma vez
        static double last_time = 0, last_space = 0, diff_space, diff_time, velocity;

        if(time - last_time > 0.05)
        {

            diff_time = time - last_time;  
            diff_space = space - last_space;
            velocity = diff_space / diff_time; // calcula a velocidade = ds/dt

            last_time = time; 
            last_space = space;
        }
        return velocity; // a unidade de medida é cm/s
    } 

    // retorna a leitura dos encoders em centímetros
    double getValueLeft()
    { 
        return ((position_sensor_left->getValue() * RAIO)*(-1)); //ele devolve o valor em radiano, assim multiplicar com o raio devolve em cm
    }
    double getValueRight()
    {
        return ((position_sensor_right->getValue() * RAIO)*(-1));
    }
};

#endif