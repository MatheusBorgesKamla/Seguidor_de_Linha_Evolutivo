#include <iostream>
#include "Robo.h"
#include <vector>
#include <time.h>
extern "C"
{
#include "extApi.h"
}

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>

using namespace std;

vector<Robo> inicializa_pop(int pop_tam)
{
    vector<Robo> robos;
    double velocidade, Kp, Ki, Kd;
    
    for (int i = 0; i < pop_tam; i++)
    {
        //Gerando velocidades aleatorias entre 5.00 a 20.00
        velocidade = double((500 + rand() % (2000 - 500+1))/100.0);
        //Gerando Kps aleatorios entre 0.01 a 10
        Kp = double((1 + rand() % (1000 - 1+1))/100.0);
        //Gerando Kis aleatorios entre 0.0001 a 0.1
        Ki = double((10 + rand() % (100 - 10+1))/10000.0);
        //Gerando Kds aleatorios entre 0.0001 a 0.1
        Kd = double((10 + rand() % (100 - 10+1))/10000.0);
        /*if(i == 0)
        {
            Kp = 1;
            Ki = 0.0003;
            Kd = 0.008;
            velocidade = 10;
        }*/
        cout << "Gerando individuo [" << i << "]: " << endl;
        cout << "\tVel: " << velocidade << endl;
        cout << "\tKp: " << Kp << endl;
        cout << "\tKi: " << Ki << endl;
        cout << "\tKd: " << Kd << endl;
        cout << "--------------------" << endl;
        robos.push_back(Robo(Kp, Ki, Kd, velocidade));
    }

    return robos;
}

void GuardaMelhor(vector<Robo> &sobreviventes, vector<Robo> &robos)
{
    double maior_fitness = 0;
    int indice_maior;
    for (int i = 0; i < robos.size(); i++)
    {
        if(robos[i].fitness > maior_fitness)
        {
            maior_fitness = robos[i].fitness;
            indice_maior = i;
        }
    }

    sobreviventes.push_back(robos[indice_maior]);
    robos.erase(robos.begin() + indice_maior);
}

void Torneio(vector<Robo> &sobreviventes, vector<Robo> &robos, int N)
{
    float k = 0.8;
    int cont = 0, ind1, ind2, ind_maior, ind_menor;
    while (cont < N)
    {
        ind1 =  rand() % (robos.size() - 1);
        ind2 = ind1;
        while (ind2 == ind1)
        {
            ind2 = rand() % (robos.size()  - 1);
        }
        //cout << "Ind1: " << ind1 << " Ind2: " << ind2 << endl;
        float r = double((rand() % 100)/100.0);
        //cout << "r: " << r << endl;
        if(robos[ind1].fitness > robos[ind2].fitness)
        {
            ind_maior = ind1;
            ind_menor = ind2;
        }
        else
        {
            ind_maior = ind2;
            ind_menor = ind1;
        }
        
        if(r < k)
        {
            sobreviventes.push_back(robos[ind_maior]);
            robos.erase(robos.begin() + ind_maior);
        }
        else
        {
            sobreviventes.push_back(robos[ind_menor]);
            robos.erase(robos.begin() + ind_menor);
        }
        cont++;
    }

}

vector<Robo> CrossoverPop(vector<Robo> sobreviventes)
{
    vector<Robo> novos_individuos;
    double Kp_n, Ki_n, Kd_n, vel_n;
    //O MELHOR E MANTIDO P/ PROX GERACAO
    novos_individuos.push_back(sobreviventes[0]);
    //PRIMEIRAMENTE TODOS CRUZAM COM O MELHOR
    for (int i = 1; i < sobreviventes.size(); i++)
    {
        //cout << " --------------------- " << endl;
        //sobreviventes[0].GetInfo();
        //sobreviventes[i].GetInfo();
        novos_individuos.push_back(sobreviventes[0].Crossover(sobreviventes[i]));
        //novos_individuos[i].GetInfo();
        //cout << " --------------------- " << endl;
    }
    //cout << "Tam Sobrevivents: " << sobreviventes.size() << " Tam Novos Indv: " << novos_individuos.size() << endl;
    //CADA SOBREVIVENTE (SEM CONSIDERAR O MELHOR) CRUZA COM O PROX SOBREVIVENTE NO VETOR

    for (int i = 1; i < sobreviventes.size(); i++)
    {
        if(i+1 < sobreviventes.size())
        {
            novos_individuos.push_back(sobreviventes[i].Crossover(sobreviventes[i+1]));
        }
        else
        {
            novos_individuos.push_back(sobreviventes[i].Crossover(sobreviventes[1]));
        }
    }
    //cout << "Tam Sobrevivents: " << sobreviventes.size() << " Tam Novos Indv: " << novos_individuos.size() << endl;

    return novos_individuos;
}

void MutaPop(vector<Robo>& populacao, float mutationRate)
{
    for (int i = 1; i < populacao.size(); i++)
    {
        populacao[i].Mutacao(mutationRate);
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL)); //garantindo aleatoriedade na hora de gerar populacao


    //Armazena o tempo da simulação atual e uma outra auxiliar para armazenar instantes anteriores
    float SimulationTime_ant = 0;
    float SimulationTime = 0;

    //Tamanho da populacao (quantidade de individuos)
    const int pop_tam = 36;
    //Taxa de mutacao
    const int mutationRate = 0.1;
    //Numero de geracoes
    const int N_geracoes = 30;
    //Declarando individuos (robos)
    vector<Robo> robos;
    //INICIALIZANDO POPULAÇÃO
    robos = inicializa_pop(pop_tam);
    //char c;
    //cin >> c;

    //Robo robo(clientID, 1, 0.0003, 0.008, 10);

    for (int j = 0; j < N_geracoes; j++)
    {
        ofstream checkpoint_file;
        //Declarando vetor que guardará os sobreviventes após cada geração
        vector<Robo> sobreviventes;
        double fitness_med = 0;

        for (int i = 0; i < robos.size(); i++)
        {
            //Variaveis para conexao do servidor
            string serverIP = "127.0.0.1";
            int serverPort = 19999;
            int clientID = simxStart((simxChar *)serverIP.c_str(), serverPort, true, true, 2000, 5);
            if (clientID != -1)
            {
                printf("Servidor conectado!\n");
            }
            else
            {
                printf("Servidor nao conectado!");
                break;
            }
            //RODA SIMULAÇÃO PARA O INDIVÍDUO A FIM DE OBTER O ERRO TOTAL PARA O ROBO
            robos[i].InicializaConexao(clientID);

            int aux = 1;
            while(aux != 0)
            {
                aux = simxGetFloatSignal(clientID, "SimulationTime", (simxFloat *)&SimulationTime, simx_opmode_streaming);
            }
            SimulationTime_ant = SimulationTime;
            while ((simxGetConnectionId(clientID) != -1) && (SimulationTime < SimulationTime_ant+(1*20)))
            {
                simxGetFloatSignal(clientID, "SimulationTime", (simxFloat *)&SimulationTime, simx_opmode_streaming);
                robos[i].Le_Sensores(false);
                robos[i].Calcula_Erro(false);
                robos[i].Calcula_CorrecaoPID(false);
                robos[i].AjustaNavegacao(false);
                cout << "Tempo Simulacao: " << SimulationTime << endl;
                robos[i].GetInfo();
                cout << "Individuo: " << i << " Geracao: " << j << endl;
                cout << "----------------------" << endl;
                extApi_sleepMs(5);
            }

            //CALCULA A APTIDÃO DO INDÍVIDUO E RESETA O ROBÔ PARA O PRÓXIMO
            //robos[i].erro_total = double((100 + rand() % (1000 - 100+1)));
            robos[i].GetInfo();
            robos[i].Calcula_Fitness(true);
            fitness_med += robos[i].fitness;
            robos[i].Reset();
            simxFinish(clientID);

        }
        fitness_med = fitness_med/robos.size();
        //cout << "Tam robos: " << robos.size() << " Tam sobreviventes: " << sobreviventes.size() << endl;
        GuardaMelhor(sobreviventes, robos);
        //cout << "Tam robos: " << robos.size() << " Tam sobreviventes: " << sobreviventes.size() << endl;
        //APÓS TODOS OS INDIVÍDUOS TEREM SUA PONTUAÇÃO DE APTIDÃO É REALIZADO A SELEÇÃO NATURAL PELO MÉTODO TORNEIO DE 2
        Torneio(sobreviventes, robos, 11);
        //cout << "Tam robos: " << robos.size() << " Tam sobreviventes: " << sobreviventes.size() << endl;
        //OS SOBREVIVENTES REALIZAM CROSSOVER A FIM DE GERAR NOVA POPULACAO
        cout << "CROSSOVER: " << endl;
        vector<Robo> nova_pop = CrossoverPop(sobreviventes);
        cout << "Tam sobreviventes: " << sobreviventes.size() << " Tam nova pop: " << nova_pop.size() << endl;
        /*cout << "INDIVIDUOS GERADOS" << endl;
        for (int i = 0; i < nova_pop.size(); i++)
        {
            nova_pop[i].GetInfo();
        }
        //OS INDIVIDUOS GERADOS SOFREM MUTACAO
        cout << "MUTACAO" << endl;*/    
        MutaPop(nova_pop, mutationRate);
        /*for (int i = 0; i < nova_pop.size(); i++)
        {
            nova_pop[i].GetInfo();
        }*/
        robos = nova_pop;
        cout << "\n\n--------- Melhor individuo: ------------ \n" ;
        robos[0].GetInfo();
        cout << "Fitness: " << robos[0].fitness << endl;
        cout << "----------------------------------\n";
        cout << "Fitness Medio: " << fitness_med << " Geracao: " << j << endl;
        string file_name = "../checkpoints/geracao" + to_string(j) + ".txt";
        checkpoint_file.open(file_name);
        for (int i = 0; i < nova_pop.size(); i++)
        {
            checkpoint_file << nova_pop[i].velocidadeBase << " " << nova_pop[i].Kp << " " << nova_pop[i].Ki << " " << nova_pop[i].Kd << "\n";
        }
        checkpoint_file << robos[0].fitness << "\n";
        checkpoint_file << fitness_med << "\n";
        checkpoint_file.close();
    }
    return 0;
}