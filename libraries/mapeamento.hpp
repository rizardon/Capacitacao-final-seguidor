/* Esse arquivo eh responsavel pela execução do mapeamento, que mapeia a pista salvando
 * a distancia de cada trecho no arquivo mapeamento.txt
 */

#ifndef MAPEAMENTO_HPP
#define MAPEAMENTO_HPP

#include "C:/Program Files/Webots/include/controller/cpp/webots/PositionSensor.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Robot.hpp"
#include "encoder.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace webots;
using namespace std;

#define PWM_MAX 2.5
#define VELOCIDADE_NORMAL 1  //isso é novo
#define VELOCIDADE_FREAR 0.5
#define TAMANHO_VETORES 32

// fazer dois vetores que guardem a distância entre dois pontos
double left_encoder_values[TAMANHO_VETORES];
double right_encoder_values[TAMANHO_VETORES];
//double dist_values[TAMANHO_VETORES];  provavelmente vai sair
double diff_left_value, left_last_value, diff_right_value, right_last_value, diff_counter_total = 0, corr_diff_counter = 0;

//unsigned long time_values[TAMANHO_VETORES]; // Coloca o tempo gasto em cada trecho -- essa variavel não é usada
//double left_value, right_value;  essas variáveis não são usadas
//unsigned long last_time_value = 0, diff_time;  essas variáveis não são usadas

string path_txt = "Mapeamento.txt";
string titulos[3] = {"Meus multiplicadores: ", "Velocidade fim do trecho: ", "Comprimento trecho: "};
double *vetores[3];

//string mapping_data;  essa variável não é usada
double posicao_atual;
bool mapeando = false;

double comprimento_trecho[TAMANHO_VETORES];
double meus_multiplicadores[TAMANHO_VETORES];
double velocidade_fim_trecho[TAMANHO_VETORES];

// função initVectors que inicializa os vetores com 0;
void initMappingVector()
{
   for (int i = 0; i < TAMANHO_VETORES; i++)
   {
       left_encoder_values[i] = 0;
       right_encoder_values[i] = 0;
       //foi tirado a variável mapping_data_code_format_LE[] e mapping_data_code_format_RE[]
   }
}

// escreve no arquivo mapeamento.txt
void writeTxt(int tamanho)
{

  vetores[0] = meus_multiplicadores;
  vetores[1] = velocidade_fim_trecho;
  vetores[2] = comprimento_trecho;

  ofstream writeVector(path_txt);
  string output;

  for(int i = 0; i< 3; i++)
  {
    writeVector << titulos[i] << endl;

      for(int j = 0; j < tamanho; j++)
      {
        writeVector << vetores[i][j];
        if(j != tamanho - 1)
            writeVector << ", ";
      }
      writeVector << "\n\n";
  }
  writeVector.close();
}

// função mappingPath que atualiza os vetores dos encoders
void mappingPath(double v_encoder_left, double v_encoder_right, bool marca)
{
    // definir last_values
    diff_left_value = v_encoder_left - left_last_value;
    diff_right_value = v_encoder_right - right_last_value;
    posicao_atual = (diff_left_value + diff_right_value)/2;
    
    if (marca){
        left_last_value = v_encoder_left;
        right_last_value = v_encoder_right;

        left_encoder_values[mapping_path] = diff_left_value;
        right_encoder_values[mapping_path] = diff_right_value;

        //dist_values[mapping_path] = (diff_left_value + diff_right_value)/2;  esse vetor só é usado aqui, pretendo tirar
        
        cout << "Parte: " << mapping_path << endl;
        mapping_path++;
    }
    diff_counter_total = (v_encoder_left + v_encoder_right)/2;
    diff_counter_total = diff_counter_total - corr_diff_counter;
}

// funcao que printa a pista no terminal Webots
void printAllValues()
{
    // foi tirado um looping que passava a variável left_encoder_values[] e right_encoder_values[] para um vetor que era usado apenas nessa função
    //string mapping;  essa variável não é usada
    int trecho_pista;

    cout << "Resultados \nEncoder esquerdo: [" ;
    
    for (trecho_pista = 0; trecho_pista < mapping_path; ++trecho_pista)
    {        
        cout << left_encoder_values[trecho_pista] << ", ";
    }
    cout << "]\nEncoder direito: [" ;
    
    for (trecho_pista = 0; trecho_pista < mapping_path; ++trecho_pista)
    {
        cout <<  right_encoder_values[trecho_pista] << ", " ;
    }
    cout << "]\n";
}

// atualiza o mapeamento com os multiplicadores
void atualizarMapeamento() 
{
    float r = 100.0;
    float d = 10.0;
    double razao_multiplicador = (r+d)/(r-d); //valor de referência(n sei pq é esse)

    for (int trecho_atual = 0 ; trecho_atual <= mapping_path ; trecho_atual++)  //mudei o nome dessa variável para ficar mais claro
    {
        // exclui duas variaveis que estavam redundantes e implementei novas para facilitar o entendimento
        float minimo_atual = min(left_encoder_values[trecho_atual] , right_encoder_values[trecho_atual]);
        float maximo_atual = max(left_encoder_values[trecho_atual] , right_encoder_values[trecho_atual]);
        float minimo_futuro = min(left_encoder_values[trecho_atual+1] , right_encoder_values[trecho_atual+1]);
        float maximo_futuro = max(left_encoder_values[trecho_atual+1] , right_encoder_values[trecho_atual+1]);
        
        if (minimo_atual == 0)
            minimo_atual = maximo_atual/2; 
        if (minimo_futuro == 0)
            minimo_futuro = maximo_futuro/2; 

        float razao_atual = maximo_atual / minimo_atual;
        float razao_futura = maximo_futuro / minimo_futuro;

        //Os multiplicadores
        if(razao_atual < razao_multiplicador)  //falta melhorar essa parte !!!!
            meus_multiplicadores[trecho_atual] = PWM_MAX;
        else
            meus_multiplicadores[trecho_atual] = VELOCIDADE_NORMAL; 
            //Melhorar esse if com varios multiplicadores (0.2 0.5 0.7 1 1.5 etc) -- isso precisaria de testes

        //Velocidade para acabar o trecho
        if(razao_atual < razao_multiplicador && razao_futura > razao_multiplicador)
            velocidade_fim_trecho[trecho_atual] = VELOCIDADE_FREAR;
        else
            velocidade_fim_trecho[trecho_atual] = VELOCIDADE_NORMAL;

        //Comprimento da pista
        comprimento_trecho[trecho_atual] = (left_encoder_values[trecho_atual]  + right_encoder_values[trecho_atual]) / 2;    

        //printa os valores achados
        cout << trecho_atual << " ==> Comprimento do trecho: " << comprimento_trecho[trecho_atual] << "   |   Multiplicador: " << meus_multiplicadores[trecho_atual] << "   |   Velocidade para acabar: " << velocidade_fim_trecho[trecho_atual] << endl;
    }
};

#endif