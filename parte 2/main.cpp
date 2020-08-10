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

class BPlus {
    private:
    indice idx[80]; 
    BPlus* pont_tree[81]; //ponteiros usados para páginas que não são folhas
    int pont_seq[81]; //ponteiros usados nas folhas
    bool ehfolha; //indica se a página é folha
    public:
    BPlus();
    void Inserir (unsigned t, unsigned i);
    BPlus* Buscar (unsigned t, unsigned i); //retorna o endereço da página
    void Alterar (unsigned t, unsigned i);
};

int main(){};
