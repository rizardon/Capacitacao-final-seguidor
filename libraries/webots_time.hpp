/* No webots não funciona o tempo, esse arquivo traz algumas funções de 
 * tempo do arduino
 */

#ifndef WEBOTS_TIME_HPP
#define WEBOTS_TIME_HPP
#include "C:/Program Files/Webots/include/controller/cpp/webots/Robot.hpp"
#include <chrono>

using namespace std::chrono;

auto startTime = high_resolution_clock::now();
int timeStepSimulation = 0;
Robot *robotTime;

void initTime(Robot *robot, int timeStep)
{
    robotTime = robot;
    timeStepSimulation = timeStep;
}

unsigned long millis()
{
    robotTime->step(timeStepSimulation);
    return (unsigned long)duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count();
}

void delay(unsigned long time)
{
    unsigned long now = millis();
    while (millis() < time + now)
    {
        robotTime->step(timeStepSimulation);
    }
}

#endif