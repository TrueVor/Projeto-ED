#include <iostream>
#include <fstream>

using namespace std;

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
    int idBloco;
    InfoBloco cabBloco; 
    pacote dados[80];
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
