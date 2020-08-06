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

Bloco::Bloco() {
    cabBloco.proximo = -1;
    cabBloco.quantidade = 0;
    idBloco = 0;
}



class SeqSet {
    private:
    Cabecalho cabSS;  
    Bloco* auxiliar;
    public:
    SeqSet();
    void Inserir(pacote& _p);  
    Bloco BuscarBloco (unsigned _t); 
    void LerArquivo();
    void EscreverNoArquivo(Bloco& _b);
};

SeqSet::SeqSet() {
    ofstream arq("trab.dat");  //cria o arquivo
    //inicializando o cabeçalho
    cabSS.num = 0;
    cabSS.posPrimeiro = 0;
    cabSS.posProximo = 0; 
    arq.write((char *) &cabSS, sizeof(Cabecalho)); //insere os dados do cabeçalho
    arq.close();
}

Bloco SeqSet::BuscarBloco(unsigned _t) {
    Bloco aux;
    bool achou = false;
    ifstream arq;
    if (arq){
        arq.read((char*) &cabSS, sizeof(Cabecalho));
        if (cabSS.num == 0) {
             throw runtime_error("Erro na busca: arquivo vazio!");
        }
        else {
            while (achou == false and aux.cabBloco.proximo != -1) {
                arq.seekg(sizeof(Bloco)*cabSS.posPrimeiro); //coloca o ponteiro de leitura no primeiro bloco
                arq.read((char*) &aux, sizeof(Bloco)); //passa o bloco do arquivo pra memória
            }
            return aux;
        }
    }
}


void SeqSet::Inserir(pacote& _p) {
    Bloco aux;
    fstream arq;
    if (arq) {
        arq.read((char*) &cabSS, sizeof(Cabecalho)); //lê o cabeçalho
        if(cabSS.num == 0) {
            cabSS.num = 1;
            cabSS.posPrimeiro = 0;
            cabSS.posProximo = 1;
            //inserindo primeiro dado e modificando cabeçalho do bloco
            aux.dados[0] = _p;
            aux.cabBloco.quantidade = 1;
            aux.cabBloco.proximo = -1;
            arq.write((char*) &aux, sizeof(Bloco));
        }
        arq.close();
        
    }
}
 

void SeqSet::LerArquivo() {  //TESTE
    Bloco novo;
    ifstream arq;
    arq.read((char*) &cabSS, sizeof(Cabecalho)); //le dados do cabeçalho
    cout << "Numero de Blocos: "<< cabSS.num << " Posicao do Primeiro: " << cabSS.posPrimeiro << " Proximo: " << cabSS.posProximo << endl;
    arq.read((char*) &novo, sizeof(Bloco)); //le dados do primeiro bloco
    cout << novo.cabBloco.quantidade << " "<< novo.cabBloco.proximo<<" "<< novo.idBloco << endl;
    for (int i = 0; i < novo.cabBloco.quantidade; i++) {
        cout << novo.dados[i].tamanho << " ";
    }
    cout << endl;
    arq.close();
}



int main(){ //TESTE
    SeqSet teste;
    pacote _p;
    teste.Inserir(_p);
    teste.LerArquivo();
}









    
    
