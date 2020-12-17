# Projeto Seguidor de Linha Evolutivo

### Descrição
Repositório utilizado para o projeto desenvolvido na disciplina **SSC0713 - Sistemas Evolutivos e Aplicados à Robótica** que consiste na implementação de um algoritmo genético para a otimização do controlador PID de um robô do tipo seguidor de linha utilizando o simulador de robótica **Coppelia Sim**. O modelo do robô utilizado na simulação possui duas rodas com motores como atuadores e uma roda central boba do tipo caster. Para a detecção da linha, foi utilizado 5 sensores com os quais é realizado a detecção do estado de posição do robô perante a linha, gerando assim um valor de erro o qual o intuito é anular utilizando um controlador PID para o controle dos motores. Dessa forma, o projeto consistiu em realizar a otimização dos parâmetros Kp, Ki e Kd do controlador PID e também da velocidade base do robô utilizando um AG (algoritmo genético). Mais informações sobre o projeto e a implementação são explicados na apresentação em vídeo.


### Requisitos Mínimos:
Os requisitos para conseguir compilar e executar as implementações são:
- CMake Version 2.6
- C11 Standard Version
- Coppelia Sim

### Organização:
A organização atual do repositório segue a seguinte estrutura:
- **include** : diretório em que fica os aquivos .h das classes implementadas.
- **build** : diretório de configuração de ambiente onde se encontra os arquivos gerados pelo CMake, como o exectável binário.
- **src** : diretório em que fica os arquivos .cpp de implementação dos métodos das classes e arquivo main.cpp.
- **Simulação_CoppeliaSim** : diretório utilizado para armazenar a simulaçao do Coppelia.
- **remoteAPI** : diretório que armazena arquivos relacionados a configuração da API remota de interface de comunicação de C/C++ com a simulação.
- **checkpoints** : diretório utilizado para salvar checkpoints do algoritmo genético, salvando os indivíduos sobreviventes para cada geração em uma linha do arquivo txt, salvando também o fitness do melhor indivíduo e também o fitness médio da população daquela geração nas últimas duas linhas.

### Como usar:
- Clone o repositório crie a pasta **build**, onde ficará os arquivos de configuração CMake:
```console
username@user-computer:~$ cd Seguidor_de_Linha_Evolutivo/ && mkdir build
```
- Entre na pasta build:
```console
username@user-computer:~$ cd Seguidor_de_Linha_Evolutivo/build
```
- Execute o comando cmake para configurar o ambiente:
```console
username@user-computer:~/Open_2020/build$ cmake ..
```
- Chame o make para compilar:
```console
username@user-computer:~/Open_2020/build$ make
```
- Abra a simulação **CenarioV6.ttt** no CoppeliaSim

- Aperte em START/RESUME para iniciar a simulação

- O arquivo executável gerado ao compilar se chama **Executable.exe**, para executá-lo (estando na /build ) faça:
```console
username@user-computer:~/Open_2020/build$ ./Executable
```
- Dessa forma o AG irá rodar da geração 0 inicializando so indíviduos aleatoriamente, para começar de uma certa geração faça:
```console
username@user-computer:~/Open_2020/build$ ./Executable ../checkpoints/geracao10.txt 11
```
- Assim seria iniciado da geração 11 lendo os indivíduos gerados na geração 10