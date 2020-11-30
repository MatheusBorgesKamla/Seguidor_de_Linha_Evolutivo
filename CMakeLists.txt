#Versão CMake necessaria
cmake_minimum_required(VERSION 2.6)
#Nome do projetovolutivo
project(Seguidor_de_Linha_E)
#Procurando por pacotes de configuração
INCLUDE(FindPkgConfig)
#Definindo flags do compilador
IF(CMAKE_COMPILER_IS_GNUCC)
SET(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -std=c++0x -DBUILD_SHARED_LIBS=OFF")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# definicoes da remoteAPI para CPP
add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)
#Referênciando os arquivos .c e .cpp para SOURCES
set(SOURCES
    remoteApi/extApi.c
    #remoteApi/extApiCustom.c
    remoteApi/extApiPlatform.c
    src/Robo.cpp
    src/main.cpp
)
#Referênciando os arquivos .h para HEADERS
set(HEADERS
    remoteApi/extApi.h
    #remoteApi/extApiCustom.h
    remoteApi/extApiInternal.h
    remoteApi/extApiPlatform.h
    include/v_repConst.h
    include/v_repLib.h
    include/Robo.h
)
#Incluindo diretórios
include_directories("include/")
include_directories("remoteApi/")
#Procurando pacotes pthreads
find_package (Threads)
#Gerando executável a partir das referências
add_executable(Executable 
               ${SOURCES}
	       ${HEADERS})
#Linkando bibliotecas de threads e da OpenCV	       
target_link_libraries (Executable ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS Executable RUNTIME DESTINATION bin)