#include <vector>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unistd.h>
#include <iostream>
#include "Robo.h"
extern "C"
{
#include "extApi.h"
}

using namespace std;

Robo::Robo()
{
  this->clientID = 0;
}

Robo::Robo(int clientID, double Kp, double Ki, double Kd, float velocidade)
{
  this->clientID = clientID;
  // inicialização dos motores
  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicLeftJoint", (simxInt *)&this->leftmotorHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle do motor esquerdo nao encontrado!\n");
  else
    printf("Conectado ao motor esquerdo!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicRightJoint", (simxInt *)&this->rightMotorHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle do motor direito nao encontrado!\n");
  else
    printf("Conectado ao motor direito!\n");

  //inicialização das rodas
  if (simxGetObjectHandle(clientID, (const simxChar *)"LineTracer", (simxInt *)&this->BaseHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da base nao encontrado!\n");
  else
    printf("Conectado a base!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicLeftWheel", (simxInt *)&this->RodaEsqHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da roda esquerda nao encontrado!\n");
  else
    printf("Conectado a roda esquerda!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicRightWheel", (simxInt *)&this->RodaDirHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da roda direita nao encontrado!\n");
  else
    printf("Conectado a roda direita!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicSlider", (simxInt *)&this->RodaCasterHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da roda caster nao encontrado!\n");
  else
    printf("Conectado a roda caster!\n");

  //inicialização da base
  if (simxGetObjectHandle(clientID, (const simxChar *)"FS", (simxInt *)&this->SFHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle do sensor force nao encontrado!\n");
  else
    printf("Conectado a sensor force!\n");

  string sensorNome[5] = {"LeftSensor2", "LeftSensor", "MiddleSensor", "RightSensor", "RightSensor2"};

  // inicialização dos sensores (remoteApi)
  for (int i = 0; i < 5; i++)
  {
    if (simxGetObjectHandle(clientID, (const simxChar *)sensorNome[i].c_str(), (simxInt *)&this->sensorHandle[i], (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    {
      cout << "Handle do sensor " << sensorNome[i] << " nao encontrado!" << endl;
    }
    else
    {
      cout << "Handle do sensor " << sensorNome[i] << " encontrado!" << endl;
      while (simxGetVisionSensorImage(this->clientID, this->sensorHandle[i], this->res, &this->image, 0, simx_opmode_streaming) != simx_return_ok)
      {
        continue;
      }
    }
  }

  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  this->velocidadeBase = velocidade;
}

Robo::Robo(double Kp, double Ki, double Kd, float velocidade)
{
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  this->velocidadeBase = velocidade;
}

void Robo::InicializaConexao(int clientID)
{
   this->clientID = clientID;
  // inicialização dos motores
  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicLeftJoint", (simxInt *)&this->leftmotorHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle do motor esquerdo nao encontrado!\n");
  else
    printf("Conectado ao motor esquerdo!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicRightJoint", (simxInt *)&this->rightMotorHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle do motor direito nao encontrado!\n");
  else
    printf("Conectado ao motor direito!\n");

  //inicialização das rodas
  if (simxGetObjectHandle(clientID, (const simxChar *)"LineTracer", (simxInt *)&this->BaseHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da base nao encontrado!\n");
  else
    printf("Conectado a base!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicLeftWheel", (simxInt *)&this->RodaEsqHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da roda esquerda nao encontrado!\n");
  else
    printf("Conectado a roda esquerda!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicRightWheel", (simxInt *)&this->RodaDirHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da roda direita nao encontrado!\n");
  else
    printf("Conectado a roda direita!\n");

  if (simxGetObjectHandle(clientID, (const simxChar *)"DynamicSlider", (simxInt *)&this->RodaCasterHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle da roda caster nao encontrado!\n");
  else
    printf("Conectado a roda caster!\n");

  //inicialização da base
  if (simxGetObjectHandle(clientID, (const simxChar *)"FS", (simxInt *)&this->SFHandle, (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    printf("Handle do sensor force nao encontrado!\n");
  else
    printf("Conectado a sensor force!\n");

  string sensorNome[5] = {"LeftSensor2", "LeftSensor", "MiddleSensor", "RightSensor", "RightSensor2"};

  // inicialização dos sensores (remoteApi)
  for (int i = 0; i < 5; i++)
  {
    if (simxGetObjectHandle(clientID, (const simxChar *)sensorNome[i].c_str(), (simxInt *)&this->sensorHandle[i], (simxInt)simx_opmode_oneshot_wait) != simx_return_ok)
    {
      cout << "Handle do sensor " << sensorNome[i] << " nao encontrado!" << endl;
    }
    else
    {
      cout << "Handle do sensor " << sensorNome[i] << " encontrado!" << endl;
      while (simxGetVisionSensorImage(this->clientID, this->sensorHandle[i], this->res, &this->image, 0, simx_opmode_streaming) != simx_return_ok)
      {
        continue;
      }
    }
  }
}

void Robo::Le_Sensores(bool PrintResposta)
{
  for (int i = 0; i < 5; i++)
  {
    if (simxGetVisionSensorImage(this->clientID, this->sensorHandle[i], this->res, &image, 0, simx_opmode_streaming) == simx_return_ok)
    {
      int sum = 0;
      for (int j = 0; j < this->res[0] * this->res[1]; j++)
      {
        sum += (int)image[j];
      }
      sum = sum / (this->res[0] * this->res[1]);

      this->sensorRes[i] = sum;
      if (this->sensorRes[i] > 60)
      {
        this->log_sensorRes[i] = 1;
      }
      else
      {
        this->log_sensorRes[i] = 0;
      }

      if (PrintResposta)
      {
        cout << "Sensor[" << i << "]: " << this->log_sensorRes[i] << endl;
      }
    }
  }
}

void Robo::Calcula_Erro(bool PrintResposta)
{
  this->perdeu_linha = false;
  if (( this->log_sensorRes [0] == 0) && (this->log_sensorRes [1] == 0) && (this->log_sensorRes [2] == 0) && (this->log_sensorRes [3] == 0) && (this->log_sensorRes [4] == 1 )) erro = 4;

  else if ((this->log_sensorRes [0] == 0) && (this->log_sensorRes [1] == 0) && (this->log_sensorRes [2] == 0) && (this->log_sensorRes [3] == 1) && (this->log_sensorRes [4] == 1)) erro = 3;

  else if ((this->log_sensorRes [0] == 0) && (this->log_sensorRes [1] == 0) && (this->log_sensorRes [2] == 0) && (this->log_sensorRes [3] == 1) && (this->log_sensorRes [4] == 0)) erro = 2;

  else if ((this->log_sensorRes [0] == 0) && (this->log_sensorRes [1] == 0) && (this->log_sensorRes [2] == 1) && (this->log_sensorRes [3] == 1) && (this->log_sensorRes [4] == 0)) erro = 1;

  else if ((this->log_sensorRes [0] == 0) && (this->log_sensorRes [1] == 0) && (this->log_sensorRes [2] == 1) && (this->log_sensorRes [3] == 0) && (this->log_sensorRes [4] == 0)) erro = 0;

  else if ((this->log_sensorRes [0] == 0) && (this->log_sensorRes [1] == 1) && (this->log_sensorRes [2] == 1) && (this->log_sensorRes [3] == 0) && (this->log_sensorRes [4] == 0)) erro = - 1;

  else if ((this->log_sensorRes [0] == 0) && (this->log_sensorRes [1] == 1) && (this->log_sensorRes [2] == 0) && (this->log_sensorRes [3] == 0) && (this->log_sensorRes [4] == 0)) erro = -2;

  else if ((this->log_sensorRes [0] == 1) && (this->log_sensorRes [1] == 1) && (this->log_sensorRes [2] == 0) && (this->log_sensorRes [3] == 0) && (this->log_sensorRes [4] == 0)) erro = -3;

  else if ((this->log_sensorRes [0] == 1) && (this->log_sensorRes [1] == 0) && (this->log_sensorRes [2] == 0) && (this->log_sensorRes [3] == 0) && (this->log_sensorRes [4] == 0)) erro = -4;

  else
  {
    this->perdeu_linha = true;
  }

  if(PrintResposta)
  {
    cout << "Erro: " << erro << endl;
  }
}

void Robo::Calcula_CorrecaoPID(bool PrintResposta)
{
  /*if((this->sensorRes[0] + this->sensorRes[1] + this->sensorRes[2] + this->sensorRes[3] + this->sensorRes[4]) == 0)
        this->sensorRes[0] = 1;
    
    this->saida = (1000*this->sensorRes[0] + 2000*this->sensorRes[1] + 3000*this->sensorRes[2] + 4000*this->sensorRes[3] + 5000*this->sensorRes[4]) / (this->sensorRes[0] + this->sensorRes[1] + this->sensorRes[2] + this->sensorRes[3] + this->sensorRes[4]);
    

  this->erro = this->referencia - this->saida;*/
  //Variação de tempo infinitesimal
  this->deltaTime = (double)(clock() - this->tempoAnt) * 1000 / CLOCKS_PER_SEC;
  tempoAnt = clock();
  //
  this->somaTime += deltaTime;

  this->P = this->erro;

  if(this->erro == 0)
  {
    this->I = 0;  
  }

  this->I = this->I + this->erro;

  this->D = (this->erro - this->erro_ant);
  
  this->erro_ant = erro;

  this->correcao = this->Kp*this->P + this->Ki*deltaTime*this->I + this->Kd*this->D/this->deltaTime;

  if (PrintResposta)
  {
    //cout << "Saida: " << this->saida << endl;
    cout << "P: " << this->Kp*this->P << " I: " << this->Ki*deltaTime*this->I << " D: " << this->Kd*this->D/this->deltaTime << endl;
    cout <<"Correcao: " << correcao << endl;
  }
  if(this->perdeu_linha)
  {
    this->erro_total += 10;
  }
  else
  {
    this->erro_total += abs(this->erro);
  }
  
}

void Robo::AjustaNavegacao(bool PrintResposta)
{
  this->velLeft = this->velocidadeBase + this->correcao;
  this->velRight = this->velocidadeBase - this->correcao;

  if (PrintResposta)
  {
    cout << "Velocidade Esq.: " << this->velLeft << " - Velocidade Dir.: " << this->velRight << endl;
  }

  simxSetJointTargetVelocity(clientID, this->leftmotorHandle, (simxFloat)this->velLeft, simx_opmode_streaming);
  simxSetJointTargetVelocity(clientID, this->rightMotorHandle, (simxFloat)this->velRight, simx_opmode_streaming);
}

void Robo::Calcula_Fitness(bool PrintResposta)
{
  this->fitness = (this->velocidadeBase/(1 + this->erro_total))*100;
  if(PrintResposta)
  {
    cout << "\n--------------" << endl;
    cout << "Erro Total: " << this->erro_total << endl;
    cout << "FITNESS: " << this->fitness << endl;
    cout << "----------------" << endl;
  }
}

void Robo::Reset()
{
  simxSetJointTargetVelocity(this->clientID, this->leftmotorHandle, (simxFloat)0, simx_opmode_streaming);
  simxSetJointTargetVelocity(this->clientID, this->rightMotorHandle, (simxFloat)0, simx_opmode_streaming);
  
  //simxPauseSimulation(clientID,simx_opmode_streaming);
  //sleep(2);   

  float base_orient[3] = {0,0,0};
  simxSetObjectOrientation(this->clientID,this->BaseHandle,-1,base_orient,simx_opmode_oneshot);

  base_orient[1] = -90;
  simxSetObjectOrientation(this->clientID,this->BaseHandle,-1,base_orient,simx_opmode_oneshot);

  float RodaEsq_position[3] = {0,0,0.0013717};
  simxSetObjectPosition(this->clientID,this->RodaEsqHandle, this->leftmotorHandle, RodaEsq_position, simx_opmode_oneshot);

  float RodaDir_position[3] = {0,0,-0.0013717};
  simxSetObjectPosition(this->clientID,this->RodaDirHandle, this->rightMotorHandle, RodaDir_position, simx_opmode_oneshot);

  float RodaCaster_position[3] = {0,0,-0.007};
  simxSetObjectPosition(this->clientID,this->RodaCasterHandle, this->SFHandle, RodaCaster_position, simx_opmode_oneshot);

  sleep(2);
  float base_position[3] = {0.14355,0,0.027632};
  simxSetObjectPosition(this->clientID,this->BaseHandle, -1, base_position, simx_opmode_oneshot);

  simxSetObjectPosition(this->clientID,this->RodaEsqHandle, this->leftmotorHandle, RodaEsq_position, simx_opmode_oneshot);

  simxSetObjectPosition(this->clientID,this->RodaDirHandle, this->rightMotorHandle, RodaDir_position, simx_opmode_oneshot);

  simxSetObjectPosition(this->clientID,this->RodaCasterHandle, this->SFHandle, RodaCaster_position, simx_opmode_oneshot);

  sleep(2);
}

void Robo::GetInfo()
{
  cout << "Velocidade: " << this->velocidadeBase << endl;
  cout << "Kp: " << Kp << " Ki: " << Ki << " Kd: " << Kd << endl;
}

Robo Robo::Crossover(Robo parceiro)
{
  double Kp_n, Ki_n, Kd_n, vel_n;
  Kp_n = (this->Kp + parceiro.Kp)/2;
  Ki_n = (this->Ki + parceiro.Ki)/2;
  Kd_n = (this->Kd + parceiro.Kd)/2;
  vel_n = (this->velocidadeBase + parceiro.velocidadeBase)/2;

  Robo filho = Robo(Kp_n, Ki_n, Kd_n, vel_n);
  return filho;
}

void Robo::Mutacao(float taxa_mutacao)
{
  float r = double((rand() % 100)/100.0);
  //cout << "mut_rate: " << taxa_mutacao << endl;
  //cout << "r: " << r << endl;
  const float f = 0.5;
  double velocidade_acresc = double((500 + rand() % (2000 - 500+1))/100.0);
  //cout << "vel: " << velocidade_acresc << endl;
  double Kp_acresc = double((1 + rand() % (1000 - 1+1))/100.0);
  //cout << "kp: " << Kp_acresc << endl;
  double Ki_acresc = double((10 + rand() % (100 - 10+1))/10000.0);
  //cout << "ki: " << Ki_acresc << endl;
  double Kd_acresc = double((10 + rand() % (100 - 10+1))/10000.0);
  //cout << "kd: " << Kd_acresc << endl;
  if(r > f)
  {
    this->Kp += Kp_acresc*taxa_mutacao;
    this->Kd += Kd_acresc*taxa_mutacao;
    this->Ki += Ki_acresc*taxa_mutacao;
    this->velocidadeBase += velocidade_acresc*taxa_mutacao;
  }
  else
  {
    this->Kp -= Kp_acresc*taxa_mutacao;
    this->Kd -= Kd_acresc*taxa_mutacao;
    this->Ki -= Ki_acresc*taxa_mutacao;
    this->velocidadeBase -= velocidade_acresc*taxa_mutacao;
  }
  
}