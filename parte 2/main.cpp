#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>

using namespace std;

//estrtura para armazenar o vetor de índices na página da B+
struct indice {
    unsigned tam;
    unsigned indice;
};

class Pagina {
    friend class BPlus;
    private:
    indice idx[80]; 
    Pagina* pont_tree[81]; //ponteiros usados para páginas que não são folhas
    int pont_seq[81]; //ponteiros usados nas folhas, para acesso ao sequence set
    bool ehfolha; //indica se a página é folha
    unsigned elementos; //guarda a quantidade de elementos váldos no vetor de indice
    public:
    Pagina();
};

Pagina::Pagina() {
    for (int i = 0; i < 80; i++) {
        idx[i] = {};
    }
    for (int i = 0; i < 81; i++) {
        pont_seq[i] = {};
        pont_tree[i] = NULL;
    }
    ehfolha = false;
    elementos = 0;
}

class BPlus {
    private:
    Pagina* raiz;
    public:
    Bplus();
    void Inserir (unsigned t, unsigned i);
    Pagina* Buscar (unsigned c1, unsigned c2); //retorna o endereço da página
    void Alterar (unsigned t, unsigned i);
};

BPlus::BPlus() {
    raiz = NULL;
}

//percorre a arvore e retorna a pagina folha em que o elemento deveria estar
Pagina* BPlus::Buscar(unsigned c1, unsigned c2) {
    Pagina* percorre = raiz;
    if (percorre -> ehfolha) 
        return percorre;
    
    while (percorre -> ehfolha == false ) {
        for (int i = 0; i < percorre -> elementos; i++ ) {
            if (c1 < percorre -> idx[i].tam) {
                percorre = percorre -> pont_tree[i]; //passa para pagina filha
            }

            else if (c1 == percorre -> idx[i].tam) { //caso a chave primaria for igual, compara com a chave secundaria
                if (c2 < percorre -> idx[i].indice) {
                    percorre = percorre -> pont_tree[i];
                }
            }
        }
        if (percorre -> ehfolha == false) { //caso ele seja maior que a ultima posição
            percorre = percorre -> pont_tree [percorre->elementos]; //ponteiro do ultimo elemento
        }
    }
    return percorre;
}

int main(){};

