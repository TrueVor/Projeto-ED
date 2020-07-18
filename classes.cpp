#include <iostream>
#include <fstream>
#include "classes.h"


using namespace std;

SeqSet::SeqSet() {
    ofstream arq("trab.dat");  //cria o arquivo
    //inicializando o cabeçalho
    cabSS.num = 0;
    cabSS.posPrimeiro = 0;
    cabSS.posProximo = 0; 
    arq.write((char *) &cabSS, sizeof(Cabecalho)); //insere os dados do cabeçalho
    arq.close();
}

Bloco SeqSet::BuscarBloco(pacote& _p) {
    Bloco aux;
    bool achou = false;
    ifstream arq;
    if (arq) {
        arq.read((char*) &cabSS, sizeof(Cabecalho));
        if (cabSS.num == 0) {
             throw runtime_error("Erro na busca: arquivo vazio!");
        }
        //nesse ponto o ponteiro de leitura já vai estar no final do cabeçalho 
        else {
            arq.read((char*) &aux, sizeof(Bloco)); //lê o primeiro Bloco
            unsigned tam = aux.cabBloco.quantidade -1; //pos do ultimo elemento valido do vetor
            if (_p.tamanho < aux.dados[tam].tamanho) { //se o elemento pertencer ao primeiro bloco
                return aux; 
            }
            else {
                while ((achou == (false)) and (aux.cabBloco.proximo != -1)) {
                    arq.seekg(sizeof(Bloco)*aux.cabBloco.proximo); //arruma o ponteiro de leitura
                    arq.read((char*) &aux, sizeof(Bloco)); //passa o bloco do arquivo pra memória
                    tam = aux.cabBloco.quantidade -1;
                    if (_p.tamanho < aux.dados[tam].tamanho) {
                        achou = true;
                        return aux;
                    }
                }
            }
            aux.idBloco = -1; //manda um bloco inválido
            return aux;
        }
    }
}


void SeqSet::Inserir(pacote& _p) {
    Bloco aux;
    int limBloco = 80; // Limite de qntdade de dados para cada bloco
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
        } else { // [INCOMPLETO]
            aux = BuscarBloco(_p);
            if(aux.idBloco == -1){ // Inserir no ultimo bloco
                if(){ // Se o bloco estiver cheio, dividir

                } else { // Do contrário, inserir o dado

                }
            } else { // Inserir no bloco encontrado
                if(){ // Se o bloco estiver cheio, dividir

                } else { // Do contrário, inserir o dado

                }
            }
        }
        arq.close();
        
    }
}
 
void SeqSet::ImprimirSS() {
    ifstream arq;
    Bloco percorre;
    string source, dest, info, prot; //para imprimir os vetores de char
    arq.read((char*) &cabSS,sizeof(Cabecalho));
    cout << "Numero de Blocos: " << cabSS.num << "   Pos do primeiro: " 
    << cabSS.posPrimeiro << "   Pos do proximo disponível: " <<  cabSS.posProximo << endl;
    //lendo o primeiro bloco
    arq.seekg(cabSS.posPrimeiro*sizeof(Bloco)); 
    //imprimindo os dados 
    cout << "BLOCO " << percorre.idBloco << ":" << endl;
    for (unsigned i = 0; i < percorre.cabBloco.quantidade; i++) {
        cout << percorre.dados[i].indice << " ";
        cout << percorre.dados[i].tempo << " ";
        source = percorre.dados[i].origem;
        cout << source << " ";
        dest =  percorre.dados[i].destino;
        cout << dest << " ";
        prot =  percorre.dados[i].protocolo;
        cout << prot << " ";
        cout << percorre.dados[i].tamanho;
        info = percorre.dados[i].infomarcao;
        cout << info << endl;
    }
    for (int i = 1; i < cabSS.num; i++) {
        arq.seekg(percorre.cabBloco.proximo*sizeof(Bloco)+sizeof(Cabecalho));
        arq.read((char*) &percorre, sizeof(Bloco));
        cout << "BLOCO " << percorre.idBloco << ":" << endl;

        for (unsigned i = 0; i < percorre.cabBloco.quantidade; i++) {
            cout << percorre.dados[i].indice << " ";
            cout << percorre.dados[i].tempo << " ";
            source = percorre.dados[i].origem;
            cout << source << " ";
            dest =  percorre.dados[i].destino;
            cout << dest << " ";
            prot =  percorre.dados[i].protocolo;
            cout << prot << " ";
            cout << percorre.dados[i].tamanho;
            info = percorre.dados[i].infomarcao;
            cout << info << endl;
        }
    }
    arq.close();
}
