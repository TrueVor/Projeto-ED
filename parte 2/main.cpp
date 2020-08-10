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

class BPlus {
    private:
    Pagina* raiz;
    public:
    void Inserir (unsigned t, unsigned i);
    Pagina* Buscar (unsigned t, unsigned i); //retorna o endereço da página
    void Alterar (unsigned t, unsigned i);
};

int main(){};
