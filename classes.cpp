#include <iostream>
#include <fstream>
#include "classes.h"
#include "OrdenaVetor.h"


using namespace std;

Bloco::Bloco() {
    cabBloco.proximo = -1;
    cabBloco.quantidade = 0;
    idBloco = 0;
}

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
                unsigned PosAbs;
                while ((achou == (false)) && (aux.cabBloco.proximo != -1)) {
                    PosAbs = (sizeof(Bloco)*aux.cabBloco.proximo) + sizeof(Cabecalho);
                    arq.seekg(PosAbs); //arruma o ponteiro de leitura
                    arq.read((char*) &aux, sizeof(Bloco)); //passa o bloco do arquivo pra memória
                    tam = aux.cabBloco.quantidade -1;
                    if (_p.tamanho < aux.dados[tam].tamanho) {
                        achou = true;
                        return aux;
                    }
                }
                // O if abaixo garante que o Bloco final seja enviado sem que o idBloco seja editado para -1
                // evitando escrever na memoria o valor idBloco do ultimo bloco como -1
                if(aux.cabBloco.proximo == -1 && achou == false)
                    return aux;
            }
            
            aux.idBloco = -1; // manda um bloco inválido
            return aux;
        }
    }
}


void SeqSet::Inserir(pacote& _p) {
    Bloco aux;
    Bloco aux2; // Utilizado como segundo auxiliar na hora de dividir blocos cheios
    int limBloco = 80; // Limite de qntdade de dados para cada bloco
    fstream arq;
    if (arq) {
        arq.read((char*) &cabSS, sizeof(Cabecalho)); //lê o cabeçalho
        if(cabSS.num == 0) {
            cabSS.num = 1;
            cabSS.posPrimeiro = 0;
            cabSS.posProximo = 1;
            arq.seekp(0);
            arq.write((char*) &cabSS, sizeof(Cabecalho));
            //inserindo primeiro dado e modificando cabeçalho do bloco
            aux.dados[0] = _p;
            aux.cabBloco.quantidade = 1;
            aux.cabBloco.proximo = -1;
            arq.seekp(sizeof(Cabecalho)); // Corrige o ponteiro para a posição relativa 0
            arq.write((char*) &aux, sizeof(Bloco));
        } else {
            aux = BuscarBloco(_p);

            arq.seekg((sizeof(Bloco)*aux.idBloco) + sizeof(Cabecalho)); // arruma o ponteiro de leitura
            arq.read((char*) &aux, sizeof(Bloco)); //passa o bloco do arquivo pra memória
            if(aux.cabBloco.quantidade == limBloco){ // Se o bloco estiver cheio, dividir
                for(int i = 0; i < (limBloco/2); i++){
                    aux2.dados[0+i] = aux.dados[(limBloco/2)+i]; // Move o dado de aux para aux2
                    aux.dados[(limBloco/2)+i] = {}; // Limpa o dado em aux
                }
                // Atualiza os devidos cabeçalhos
                aux2.idBloco = cabSS.posProximo;
                aux2.cabBloco.quantidade = limBloco/2;
                aux2.cabBloco.proximo = aux.cabBloco.proximo;
                cabSS.num += 1;
                cabSS.posProximo = cabSS.num;
                aux.cabBloco.quantidade = limBloco/2;
                aux.cabBloco.proximo = cabSS.posProximo;
                // Confere qual dos dois blocos inserir o valor _p
                if(_p.tamanho <= aux.dados[(limBloco/2)-1].tamanho){
                    aux.dados[limBloco/2] = _p;
                    aux.cabBloco.quantidade += 1;
                    selectionSort(aux.dados, aux.cabBloco.quantidade);
                } else { // Insere no pacote criado
                    aux2.dados[limBloco/2] = _p;
                    aux2.cabBloco.quantidade += 1;
                    selectionSort(aux2.dados, aux2.cabBloco.quantidade);
                }
                // Inserir na memória o Cabeçalho , Bloco aux e Bloco aux2
                arq.seekp(0);
                arq.write((char*) &cabSS, sizeof(Cabecalho));

                arq.seekp((sizeof(Bloco)*aux.idBloco) + sizeof(Cabecalho));
                arq.write((char*) &aux, sizeof(Bloco));

                arq.seekp((sizeof(Bloco)*aux2.idBloco) + sizeof(Cabecalho));
                arq.write((char*) &aux2, sizeof(Bloco));
                
            } else { // Do contrário, apenas inserir o dado no bloco
                aux.dados[aux.cabBloco.quantidade] = _p;
                aux.cabBloco.quantidade += 1;
                selectionSort(aux.dados, aux.cabBloco.quantidade);
                arq.seekp((sizeof(Bloco)*aux.idBloco) + sizeof(Cabecalho)); // Posição Absoluta do Bloco aux
                arq.write((char*) &aux, sizeof(Bloco));
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
