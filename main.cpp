#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "classes.h" // 

using namespace std;

// para saber o tamanho de cada campo, fiz um primeiro teste para
// pegar o tamanho máximo de cada campo do tipo texto
// adicionei mais uma posição para armazenar o término de string '\0'
/*struct pacote {
    unsigned indice; 
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char infomarcao[1650];
};*/

int main(){
    SeqSet Seq;
    std::ifstream arquivo_csv("captura_pacotes.csv");
    if (!arquivo_csv) {
        return EXIT_FAILURE;
    }

    string linha;
    int starting_line = 2076361;  //pos da primeira linha a ser lida
    for (int i = 1; i < starting_line; i += 1) {
        if(!getline(arquivo_csv,linha)) {  
            return EXIT_FAILURE;
        }
    }


    string campo; // variável para obter um campo de cada linha lida
    string delimitador = "\",\""; // delimitador entre os campos
    unsigned posFimCampo; // posição final do campo

    pacote umPacote;

    while(getline(arquivo_csv,linha) && umPacote.indice <  2162875) {
        campo = linha.erase(0,1); // remove primeiro caracter da linha (")

        // obtendo primeiro campo, um inteiro - No.
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.indice = stoul(campo);

        // obtendo segundo campo, um float - Time
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tempo = stof(campo);

        // obtendo terceiro campo, um texto - Source
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.origem, campo.c_str());

        // obtendo quarto campo, um texto - Destination
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.destino, campo.c_str());

        // obtendo quinto campo, um texto - Protocol
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.protocolo, campo.c_str());

        // obtendo sexto campo, um texto - Length
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tamanho = stoul(campo);

        // obtendo último campo, um texto - Info
        // nesse caso, procuro o último aspas, fazendo busca reversa
        posFimCampo = linha.rfind("\"");
        campo = linha.substr(0, posFimCampo);
        strcpy(umPacote.infomarcao, campo.c_str());


        
    }

    return 0;
}
