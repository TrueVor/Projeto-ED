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
    bool Ordena = true;
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
        } else {
            for (unsigned i = 0; i < cabSS.num; i++) {
                arq.read((char*) &aux, sizeof(Bloco)); // Lê o primeiro bloco
                if(aux.cabBloco.proximo == -1){ // Caso seja o último bloco
                    if(aux.cabBloco.quantidade < limBloco){ // Se o bloco não estiver cheio, inserir os dados
                        aux.dados[aux.cabBloco.quantidade-1] = _p;
                        aux.cabBloco.quantidade += 1;
                    }
                    else { // [INCOMPLETO]
                        if(){ // Caso já tenha um bloco vazio, inserir dados [INCLOMPLETO]

                        } else { // Cria um novo bloco e insere os dados nele [INCLOMPLETO]
                            Ordena = false; // Retira necessidade de ordenar
                        }
                    }
                } else {
                    if(){ // Se a chave do ultimo valor for maior ou igual do que a chave do dado a ser inserido, inserir o dado [INCOMPLETO]
                        if(){ // Se o bloco estiver cheio, dividi-lo em dois [INCOMPLETO]

                        } else { // Do contrário, inserir o dado [INCOMPLETO]

                        }
                    }
                }
            }
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
