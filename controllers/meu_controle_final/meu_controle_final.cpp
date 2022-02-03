/* O Objetivo desse arquivo é usar as funcoes principais 
 * para o robo andar, ele basicamente faz a calibração e poe o robo para seguir a 
 * linha usando os PIDs
 */
#define TIMESTEP 3

#include "../../libraries/definicao.hpp"
#include "../../libraries/key.hpp"
#include "../../libraries/mapeamento.hpp"
#include "../../libraries/motors.hpp"
#include "../../libraries/irsensors.hpp"

using namespace webots;
using namespace std;

bool readRight();
bool readLeft();

int main(int argc, char **argv)
{

  initTime(robot, TIMESTEP);
  initMappingVector();
  initConstants();
  initKey();

  // definição dos motores
  motor_left->setPosition(INFINITY);
  motor_right->setPosition(INFINITY);
  // double value_left, value_right;
  motor_left->setVelocity(linear);
  motor_right->setVelocity(-linear); 

  Presto->resetPhysics();

  while (robot->step(TIMESTEP) != -1)
  {

    //Calibrando todos os sensores durante 11s
    double time = robot->getTime();

    while (time <  11)// CALIBRAÇÃO
    {
      robot->step(TIMESTEP);
      right_sensor.calibrateSensors(time);
      left_right_sensor.calibrateSensors(time);
      left_left_sensor.calibrateSensors(time);
      frontal_sensors.calibrateSensors(time);

      time = robot->getTime();
    }
    cout << "Fim da calibração" << endl; 

    vector<IrSensor> sensor_vector = {frontal_sensors, right_sensor, left_right_sensor, left_left_sensor};
    // Escrita da calibração no txt
    fstream calibracaoTxt("Calibracao.txt");
    calibracaoTxt << "Calibracao dos sensores";

    for (int i=0; i<4; i++)
    {
      calibracaoTxt<<endl <<endl << "Sensor "<< sensor_name[i] <<endl;
      calibracaoTxt <<"Maximo: " <<endl;
    
      for (int j=0; j < sensor_vector[i].qtde_sensores; j++)
      {
        calibracaoTxt << sensor_vector[i].maximo[j] << ", ";
      }
      calibracaoTxt<<endl << endl << "Minimo: " <<endl;
      for (int j=0; j < sensor_vector[i].qtde_sensores; j++)
      {
        cout << "minimos: " << sensor_vector[i].minimo[j] << endl;
        calibracaoTxt << sensor_vector[i].minimo[j]  << ", ";
      }
    }

    motor_left->setVelocity(0);
    motor_right->setVelocity(0); 
    linear = 18;

    double erro, input_pid, erro_linear, vel_medida;
    bool freada_final = false;
    double marcacao = 0;

    bool seguir = true;
    while (seguir)
    { 
      robot->step(TIMESTEP);
      key = kb.getKey();

      erro = frontal_sensors.readLine();
      input_pid = (erro - 3500)/3500;  //pq 3.500????
      PID(input_pid, &main_loop);
      vel_medida = abs(encoders.getVelocity());
      erro_linear = linear - vel_medida;
  
      if (vel_medida < 50 and vel_medida > 0)
        PID(erro_linear, &vel_linear);
      else
       cout << "vel_medida: " << vel_medida << endl;

      moveSimple(main_loop.output, vel_linear.output, motor_left, motor_right);

      double leitura_dir = right_sensor.readCalibrated();
      
      double giro_esq_atual = encoders.getValueLeft();
      double giro_dir_atual = encoders.getValueRight();
      
      double leitura_esq_dir = left_right_sensor.readCalibrated();
      double leitura_esq_esq = left_left_sensor.readCalibrated();
      
      bool val_read_left = readLeft(giro_esq_atual, giro_dir_atual, leitura_esq_dir, leitura_esq_esq, mapping_path);
      bool val_read_right = readRight(giro_esq_atual, giro_dir_atual, leitura_dir, mapping_path);

      mappingPath(giro_esq_atual, giro_dir_atual, val_read_left || val_read_right);

      if (diff_counter_total > COMPRIMENTO_PISTA and mapping_path >= 32 and mapeando)
      { 
        cout << "diff_counter_total: " << diff_counter_total << endl;
        printAllValues();
        motor_left->setVelocity(0);
        motor_right->setVelocity(0);
        atualizarMapeamento();     
        writeTxt(mapping_path);
        mapeando = false;
        mapping_path = 0;
        delay(2);
      }

      if (mapeando == false and val_read_left and mapping_path > 32) //mudar esse 32 para uma macro
      {
        marcacao = diff_counter_total;
        freada_final = true;
      }

      if (freada_final and (diff_counter_total - marcacao) > 50)
      {
        motor_left->setVelocity(0);
        motor_right->setVelocity(0);
        seguir = false;
        robot->step(TIMESTEP);
        delay(2);
      }
      corr_diff_counter = diff_counter_total;
      readKey();
      }
  delete robot;
  return 0;
  }  
}
