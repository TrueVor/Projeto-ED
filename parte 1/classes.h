#include <iostream>
#include <fstream>

using namespace std;

// Pacote de dados
struct pacote  {
    unsigned indice; 
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char infomarcao[1650];
};

//CABEÇALHO DO SEQUENCE SET
struct Cabecalho { 
    unsigned posPrimeiro; //armazena a posição do primeiro bloco.
    unsigned posProximo; //armazena a posição relativa do proximo bloco a ser criado
    int num; //armarzena a quantidade de blocos
};

//CABEÇALHO DE CADA BLOCO
struct InfoBloco {
    unsigned quantidade; //quantidade de elementos validos no bloco;
    int proximo ; //posição relativa do proximo bloco no arquivo
};
    

class Bloco { 
    friend class SeqSet;
    private:
    int idBloco; // Posição Relativa do Bloco
    InfoBloco cabBloco; // Cabeçalho do Bloco
    pacote dados[80]; // Pacote de dados de tamanho máximo de 80 e mínimo de 40
    public:
    Bloco();

};

class SeqSet {
    private:
    Cabecalho cabSS;  
    Bloco* auxiliar;
    public:
    SeqSet();
    void Inserir(pacote& _p);  
    Bloco BuscarBloco(pacote& _p); 
    void ImprimirSS();
    void EscreverNoArquivo(Bloco& _b);
};
