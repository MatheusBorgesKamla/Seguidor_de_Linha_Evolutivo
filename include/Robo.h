#ifndef ROBO
#define ROBO_H

#include <vector>
#include <string>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <unistd.h>
#include <iostream>
extern "C"
{
#include "extApi.h"
}


using namespace std;

class Robo
{
    private:
        //Vetor que armazena resposta dos sensores infras
        int sensorRes[5];
        //Armazena resposta lógica dos sensores (0 ou 1)
        int log_sensorRes[5];
        //Componentes do controlador PID
        double P = 0, I = 0, D = 0;
        //Erro e saida do sistema de controle
        double erro, erro_ant = 0, correcao;
        uint16_t saida, saida_ant;
        //Variaveis para trabalhar com o tempo a fim de aproximar derivada e integral
        double deltaTime, tempoAnt = 0, somaTime = 0;
        //Valor de referência do sistema de controle
        int referencia = 3000;
        //Handle dos sensores
        int sensorHandle[5];
        //Handles dos motores
        int leftmotorHandle;
        int rightMotorHandle;
        //Sinais dos encoders
        float encoderSignals[2];
        //Handle da base
        int BaseHandle;
        //Handle da roda esq.
        int RodaEsqHandle;
        //Handle da roda esq.
        int RodaCasterHandle;
        //Handle sensor force
        int SFHandle;
        //Handle da roda direita
        int RodaDirHandle;
        //Variáveis de velocidade dos motores
        float velLeft = 0;
        float velRight = 0;
        //Quando o robo perdeu a linha e true
        bool perdeu_linha = false;
    public:
        //Armezena erro total
        double erro_total = 0;
        //Constantes do controle PID
        double Kp, Ki, Kd;
        float velocidadeBase;
        //Armazena a pontuação de adaptação do robô
        double fitness;
        //ID do cliente servidor da simulação
        int clientID;
        //Variáveis para leitura dos sensores infravermelhos
        int res[2];
        simxUChar* image;
        //Método construtor
        Robo();
        Robo(int clientID, double Kp, double Ki, double Kd, float velocidade);
        Robo(double Kp, double Ki, double Kd, float velocidade);
        //Inicializa conexão com a simulação caso não tenha usado o construtor para isso
        void InicializaConexao(int clientID);
        //Função responsável pela leitura dos sensores
        void Le_Sensores(bool PrintResposta);
        //Calcula o erro atual a partir dos sensores IR
        void Calcula_Erro(bool PrintResposta);
        //Calcula a correção a ser realizada através do PID
        void Calcula_CorrecaoPID(bool PrintResposta);
        //Ajusta as velocidades de acordo com o controlador
        void AjustaNavegacao(bool PrintResposta);
        //Calcula o valor de aptidão do robô
        void Calcula_Fitness(bool PrintResposta);
        //Reseta o robô na simulação para a posição inicial
        void Reset();
        //Printa informações robÔ
        void GetInfo();
        //Realiza crossover calculando a média das constantes de PID e velocidade retornando um individuo filho
        Robo Crossover(Robo parceiro);
        //Realiza mutação a partir da taxa de mutação
        void Mutacao(float taxa_mutacao);
};


#endif